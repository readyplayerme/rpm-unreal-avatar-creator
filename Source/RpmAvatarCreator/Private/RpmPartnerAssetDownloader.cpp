// Copyright © 2023++ Ready Player Me


#include "RpmPartnerAssetDownloader.h"

#include "Kismet/KismetRenderingLibrary.h"
#include "Requests/RequestFactory.h"
#include "Extractors/PartnerAssetExtractor.h"
#include "Engine/Texture2D.h"

namespace
{
	const FString EYE_MASK_CROP_PARAMS = "?rect=155,152,204,204&w=64";
	const FString ASSET_ICON_PARAMS = "?w=64";

	bool IsAssetFiltered(const FRpmPartnerAsset& Asset, EAvatarBodyType BodyType, EAvatarGender Gender)
	{
		const bool BodyTypeFiltered = !((Asset.AssetType == ERpmPartnerAssetType::Outfit && BodyType == EAvatarBodyType::HalfBody) ||
			(Asset.AssetType == ERpmPartnerAssetType::Shirt && BodyType == EAvatarBodyType::FullBody));
		const bool GenderFiltered = Asset.Gender == Gender || Asset.Gender == EAvatarGender::Undefined;
		return BodyTypeFiltered && GenderFiltered;
	}
}

void URpmPartnerAssetDownloader::SetRequestFactory(TSharedPtr<class FRequestFactory> Factory)
{
	RequestFactory = Factory;
}

void URpmPartnerAssetDownloader::DownloadAssets()
{
	AssetRequest = RequestFactory->CreateAssetRequest(100, CurrentPageIndex + 1);
	AssetRequest->GetCompleteCallback().BindUObject(this, &URpmPartnerAssetDownloader::OnAssetsDownloadCompleted);
	AssetRequest->Download();
}

TArray<FRpmPartnerAsset> URpmPartnerAssetDownloader::GetFilteredAssets(EAvatarBodyType BodyType, EAvatarGender Gender) const
{
	return Assets.FilterByPredicate([BodyType, Gender](const auto& Asset){ return IsAssetFiltered(Asset, BodyType, Gender); });
}

bool URpmPartnerAssetDownloader::AreAssetsReady() const
{
	return IconRequests.Num() == 0 && !AssetRequest.IsValid();
}

FBaseRequestCompleted& URpmPartnerAssetDownloader::GetPartnerAssetsDownloadCallback()
{
	return OnPartnerAssetsDownloaded;
}

FBaseRequestCompleted& URpmPartnerAssetDownloader::GetIconsDownloadCallback()
{
	return OnIconsDownloaded;
}

void URpmPartnerAssetDownloader::OnAssetsDownloadCompleted(bool bSuccess)
{
	if (bSuccess)
	{
		const TArray<FRpmPartnerAsset> AssetChunk = FPartnerAssetExtractor::ExtractAssets(AssetRequest->GetContentAsString());
		if (AssetChunk.Num() != 0)
		{
			Assets.Append(AssetChunk);
			++CurrentPageIndex;
			DownloadAssets();
			return;
		}
	}
	else
	{
		Assets.Empty();
	}
	AssetRequest.Reset();
	(void)OnPartnerAssetsDownloaded.ExecuteIfBound(Assets.Num() != 0);
	OnPartnerAssetsDownloaded.Unbind();
}

void URpmPartnerAssetDownloader::DownloadIcons(EAvatarBodyType BodyType, EAvatarGender Gender)
{
	for (const auto& Asset : Assets)
	{
		if (IsValid(Asset.IconTexture) || !IsAssetFiltered(Asset, BodyType, Gender))
		{
			continue;
		}
		FString IconRequestUrl = Asset.IconUrl;
		IconRequestUrl += Asset.AssetType == ERpmPartnerAssetType::EyeColor ? EYE_MASK_CROP_PARAMS : ASSET_ICON_PARAMS;
		auto IconRequest = RequestFactory->CreateImageRequest(IconRequestUrl);
		IconRequests.Add(Asset.IconUrl, IconRequest);
		IconRequest->GetCompleteCallback().BindUObject(this, &URpmPartnerAssetDownloader::OnIconDownloadCompleted, Asset.IconUrl);
		IconRequest->Download();
		if (!Asset.BadgeUrl.IsEmpty())
		{
			auto BadgeRequest = RequestFactory->CreateImageRequest(Asset.BadgeUrl + ASSET_ICON_PARAMS);
			if (!IconRequests.Contains(Asset.BadgeUrl))
			{
				IconRequests.Add(Asset.BadgeUrl, BadgeRequest);
			}
			BadgeRequest->GetCompleteCallback().BindUObject(this, &URpmPartnerAssetDownloader::OnIconDownloadCompleted, Asset.BadgeUrl);
			BadgeRequest->Download();
		}
	}
	if (IconRequests.Num() == 0)
	{
		(void)OnIconsDownloaded.ExecuteIfBound(true);
		OnIconsDownloaded.Unbind();
	}
}

void URpmPartnerAssetDownloader::OnIconDownloadCompleted(bool bSuccess, FString Icon)
{
	if (!bSuccess)
	{
		(void)OnIconsDownloaded.ExecuteIfBound(false);
		OnIconsDownloaded.Unbind();
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
			if (ImageMap.Contains(Asset.IconUrl))
			{
				Asset.IconTexture = ImageMap[Asset.IconUrl];
			}
			if (ImageMap.Contains(Asset.BadgeUrl))
			{
				Asset.BadgeTexture = ImageMap[Asset.BadgeUrl];
			}
		}
		ImageMap.Empty();
		(void)OnIconsDownloaded.ExecuteIfBound(true);
		OnIconsDownloaded.Unbind();
	}
}
