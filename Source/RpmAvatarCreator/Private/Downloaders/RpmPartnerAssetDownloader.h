// Copyright © 2023++ Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "RpmAvatarCreatorTypes.h"

class FRpmPartnerAssetDownloader : public TSharedFromThis<FRpmPartnerAssetDownloader>
{
public:
	FRpmPartnerAssetDownloader(TSharedPtr<class FRequestFactory> RequestFactory);

	void DownloadAssets();

	void ClearAssets();

	TArray<FRpmPartnerAsset> GetFilteredAssets(EAvatarBodyType BodyType, EAvatarGender Gender) const;

	FBaseRequestCompleted& GetPartnerAssetsDownloadCallback();

	bool AreAssetsReady() const;

private:
	void OnAssetsDownloadCompleted(bool bSuccess);

	TArray<FRpmPartnerAsset> Assets;

	FBaseRequestCompleted OnPartnerAssetsDownloaded;

	TSharedPtr<class FRequestFactory> RequestFactory;
	TSharedPtr<class IBaseRequest> AssetRequest;
	int32 CurrentPageIndex = 0;
};
