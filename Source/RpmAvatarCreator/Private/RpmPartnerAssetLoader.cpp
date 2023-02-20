// Copyright © 2023++ Ready Player Me


#include "RpmPartnerAssetLoader.h"

#include "Kismet/KismetRenderingLibrary.h"
#include "Requests/RequestFactory.h"
#include "Extractors/PartnerAssetExtractor.h"

namespace
{
	bool IsAssetFiltered(const FRpmPartnerAsset& Asset, EAvatarBodyType BodyType, EAvatarGender Gender)
	{
		const bool BodyTypeFiltered = !((Asset.AssetType == ERpmPartnerAssetType::Outfit && BodyType == EAvatarBodyType::HalfBody) ||
			(Asset.AssetType == ERpmPartnerAssetType::Shirt && BodyType == EAvatarBodyType::FullBody));
		const bool GenderFiltered = Asset.Gender == Gender || Asset.Gender == EAvatarGender::Undefined;
		return BodyTypeFiltered && GenderFiltered;
	}
}

void URpmPartnerAssetLoader::DownloadAssets(TSharedPtr<FRequestFactory> Factory, EAvatarBodyType BodyType, EAvatarGender Gender)
{
	if (Assets.Num() != 0)
	{
		DownloadIcons(BodyType, Gender);
	}
	else
	{
		RequestFactory = Factory;
		AssetRequest = RequestFactory->CreateAssetRequest();
		AssetRequest->GetCompleteCallback().BindUObject(this, &URpmPartnerAssetLoader::OnAssetsDownloadCompleted, BodyType, Gender);
		AssetRequest->Download();
	}
}

TArray<FRpmPartnerAsset> URpmPartnerAssetLoader::GetFilteredAssets(EAvatarBodyType BodyType, EAvatarGender Gender) const
{
	return Assets.FilterByPredicate([BodyType, Gender](const auto& Asset){ return IsAssetFiltered(Asset, BodyType, Gender); });
}

bool URpmPartnerAssetLoader::AreAssetsReady() const
{
	return IconRequests.Num() == 0 && !AssetRequest.IsValid();
}

FPartnerAssetsDownloadCompleted& URpmPartnerAssetLoader::GetPartnerAssetsDownloadCallback()
{
	return OnPartnerAssetsDownloaded;
}

void URpmPartnerAssetLoader::OnAssetsDownloadCompleted(bool bSuccess, EAvatarBodyType BodyType, EAvatarGender Gender)
{
	if (!bSuccess)
	{
		(void)OnPartnerAssetsDownloaded.ExecuteIfBound(false);
		OnPartnerAssetsDownloaded.Unbind();
		return;
	}
	Assets = FPartnerAssetExtractor::ExtractAssets(AssetRequest->GetContentAsString());
	AssetRequest.Reset();
	if (Assets.Num() == 0)
	{
		(void)OnPartnerAssetsDownloaded.ExecuteIfBound(false);
		OnPartnerAssetsDownloaded.Unbind();
		return;
	}
	DownloadIcons(BodyType, Gender);
}

void URpmPartnerAssetLoader::DownloadIcons(EAvatarBodyType BodyType, EAvatarGender Gender)
{
	for (const auto& Asset : Assets)
	{
		if (IsValid(Asset.IconTexture) || !IsAssetFiltered(Asset, BodyType, Gender))
		{
			continue;
		}
		auto IconRequest = RequestFactory->CreateImageRequest(Asset.Icon);
		IconRequests.Add(Asset.Icon, IconRequest);
		IconRequest->GetCompleteCallback().BindUObject(this, &URpmPartnerAssetLoader::OnIconDownloadCompleted, Asset.Icon);
		IconRequest->Download();
		if (!Asset.Badge.IsEmpty())
		{
			auto BadgeRequest = RequestFactory->CreateImageRequest(Asset.Badge);
			if (!IconRequests.Contains(Asset.Badge))
			{
				IconRequests.Add(Asset.Badge, BadgeRequest);
			}
			BadgeRequest->GetCompleteCallback().BindUObject(this, &URpmPartnerAssetLoader::OnIconDownloadCompleted, Asset.Badge);
			BadgeRequest->Download();
		}
	}
	if (IconRequests.Num() == 0)
	{
		(void)OnPartnerAssetsDownloaded.ExecuteIfBound(true);
		OnPartnerAssetsDownloaded.Unbind();
	}
}

void URpmPartnerAssetLoader::OnIconDownloadCompleted(bool bSuccess, FString Icon)
{
	if (!bSuccess)
	{
		(void)OnPartnerAssetsDownloaded.ExecuteIfBound(false);
		OnPartnerAssetsDownloaded.Unbind();
		// Check if we need to stop downloading other assets
		return;
	}

	UTexture2D* Texture = UKismetRenderingLibrary::ImportBufferAsTexture2D(this, IconRequests[Icon]->GetContent());
	IconRequests.Remove(Icon);
	ImageMap.Add(Icon, Texture);

	if (IconRequests.Num() == 0)
	{
		for (auto& Asset : Assets)
		{
			if (ImageMap.Contains(Asset.Icon))
			{
				Asset.IconTexture = ImageMap[Asset.Icon];
			}
			if (ImageMap.Contains(Asset.Badge))
			{
				Asset.BadgeTexture = ImageMap[Asset.Badge];
			}
		}
		ImageMap.Empty();
		(void)OnPartnerAssetsDownloaded.ExecuteIfBound(true);
		OnPartnerAssetsDownloaded.Unbind();
	}
}
