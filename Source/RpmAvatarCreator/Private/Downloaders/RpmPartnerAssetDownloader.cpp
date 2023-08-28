// Copyright © 2023++ Ready Player Me


#include "RpmPartnerAssetDownloader.h"

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

FRpmPartnerAssetDownloader::FRpmPartnerAssetDownloader(TSharedPtr<FRequestFactory> RequestFactory)
	: RequestFactory(RequestFactory)
{
}

void FRpmPartnerAssetDownloader::DownloadAssets()
{
	if (AreAssetsReady())
	{
		(void)OnPartnerAssetsDownloaded.ExecuteIfBound(true);
		OnPartnerAssetsDownloaded.Unbind();
		return;
	}
	if (Assets.Num() == 0)
	{
		CurrentPageIndex = 0;
	}
	AssetRequest = RequestFactory->CreateAssetRequest(100, CurrentPageIndex + 1);
	AssetRequest->GetCompleteCallback().BindSP(AsShared(), &FRpmPartnerAssetDownloader::OnAssetsDownloadCompleted);
	AssetRequest->Download();
}

void FRpmPartnerAssetDownloader::ClearAssets()
{
	Assets.Empty();
}

TArray<FRpmPartnerAsset> FRpmPartnerAssetDownloader::GetFilteredAssets(EAvatarBodyType BodyType, EAvatarGender Gender) const
{
	return Assets.FilterByPredicate([BodyType, Gender](const auto& Asset){ return IsAssetFiltered(Asset, BodyType, Gender); });
}

bool FRpmPartnerAssetDownloader::AreAssetsReady() const
{
	return !AssetRequest.IsValid() && Assets.Num() > 0;
}

FBaseRequestCompleted& FRpmPartnerAssetDownloader::GetPartnerAssetsDownloadCallback()
{
	return OnPartnerAssetsDownloaded;
}

void FRpmPartnerAssetDownloader::OnAssetsDownloadCompleted(bool bSuccess)
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
