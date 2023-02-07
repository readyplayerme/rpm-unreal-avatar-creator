// Copyright © 2023++ Ready Player Me


#include "RpmPartnerAssetLoader.h"

#include "Kismet/KismetRenderingLibrary.h"
#include "Requests/RequestFactory.h"
#include "Extractors/PartnerAssetExtractor.h"

void URpmPartnerAssetLoader::DownloadAssets(TSharedPtr<FRequestFactory> Factory)
{
	RequestFactory = Factory;
	AssetRequest = RequestFactory->CreateAssetRequest();
	AssetRequest->GetCompleteCallback().BindUObject(this, &URpmPartnerAssetLoader::OnAssetsDownloadCompleted);
	AssetRequest->Download();
}

bool URpmPartnerAssetLoader::AreAssetsReady() const
{
	return bAssetsReady;
}

FPartnerAssetsDownloadCompleted& URpmPartnerAssetLoader::GetPartnerAssetsDownloadCallback()
{
	return OnPartnerAssetsDownloaded;
}

void URpmPartnerAssetLoader::OnAssetsDownloadCompleted(bool bSuccess)
{
	if (!bSuccess)
	{
		(void)OnPartnerAssetsDownloaded.ExecuteIfBound(false);
		OnPartnerAssetsDownloaded.Unbind();
		return;
	}
	Assets = FPartnerAssetExtractor::ExtractAssets(AssetRequest->GetContentAsString());
	if (Assets.Num() == 0)
	{
		(void)OnPartnerAssetsDownloaded.ExecuteIfBound(false);
		OnPartnerAssetsDownloaded.Unbind();
		return;
	}
	DownloadIcons();
}

void URpmPartnerAssetLoader::DownloadIcons()
{
	int32 Index = 0;
	for (const auto& Asset : Assets)
	{
		auto IconRequest = RequestFactory->CreateImageRequest(Asset.Icon);
		IconRequests.Add(Index, IconRequest);
		IconRequest->GetCompleteCallback().BindUObject(this, &URpmPartnerAssetLoader::OnIconDownloadCompleted, Index);
		IconRequest->Download();
		if (!Asset.Badge.IsEmpty())
		{
			auto BadgeRequest = RequestFactory->CreateImageRequest(Asset.Badge);
			if (!BadgeRequests.Contains(Asset.Badge))
			{
				BadgeRequests.Add(Asset.Badge, BadgeRequest);
			}
			BadgeRequest->GetCompleteCallback().BindUObject(this, &URpmPartnerAssetLoader::OnBadgeDownloadCompleted, Asset.Badge);
			BadgeRequest->Download();
		}
		++Index;
	}
}

void URpmPartnerAssetLoader::OnIconDownloadCompleted(bool bSuccess, int32 Index)
{
	if (!bSuccess)
	{
		(void)OnPartnerAssetsDownloaded.ExecuteIfBound(false);
		OnPartnerAssetsDownloaded.Unbind();
		// Check if we need to stop downloading other assets
		return;
	}
	Assets[Index].IconTexture = UKismetRenderingLibrary::ImportBufferAsTexture2D(this, IconRequests[Index]->GetContent());
	IconRequests.Remove(Index);
	if (IconRequests.Num() == 0 && BadgeRequests.Num() == 0)
	{
		bAssetsReady = true;
		(void)OnPartnerAssetsDownloaded.ExecuteIfBound(true);
		OnPartnerAssetsDownloaded.Unbind();
	}
}

void URpmPartnerAssetLoader::OnBadgeDownloadCompleted(bool bSuccess, FString Badge)
{
	if (!bSuccess)
	{
		(void)OnPartnerAssetsDownloaded.ExecuteIfBound(false);
		OnPartnerAssetsDownloaded.Unbind();
		// Check if we need to stop downloading other assets
		return;
	}
	UTexture2D* Texture = UKismetRenderingLibrary::ImportBufferAsTexture2D(this, BadgeRequests[Badge]->GetContent());
	for (auto& Asset : Assets)
	{
		if (Asset.Badge == Badge)
		{
			Asset.BadgeTexture = Texture;
			BadgeRequests.Remove(Badge);
		}
	}
	if (IconRequests.Num() == 0 && BadgeRequests.Num() == 0)
	{
		bAssetsReady = true;
		(void)OnPartnerAssetsDownloaded.ExecuteIfBound(true);
		OnPartnerAssetsDownloaded.Unbind();
	}
}
