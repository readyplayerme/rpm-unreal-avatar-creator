// Copyright © 2023++ Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RpmAvatarCreatorTypes.h"
#include "RpmPartnerAssetDownloader.generated.h"

UCLASS(BlueprintType)
class RPMAVATARCREATOR_API URpmPartnerAssetDownloader : public UObject
{
	GENERATED_BODY()

public:
	void SetRequestFactory(TSharedPtr<class FRequestFactory> Factory);

	void DownloadAssets();

	void DownloadIcons(EAvatarBodyType BodyType, EAvatarGender Gender);

	UPROPERTY()
	TArray<FRpmPartnerAsset> Assets;

	TArray<FRpmPartnerAsset> GetFilteredAssets(EAvatarBodyType BodyType, EAvatarGender Gender) const;

	FBaseRequestCompleted& GetPartnerAssetsDownloadCallback();

	FBaseRequestCompleted& GetIconsDownloadCallback();

	bool AreAssetsReady() const;

private:
	UFUNCTION()
	void OnAssetsDownloadCompleted(bool bSuccess);

	UFUNCTION()
	void OnIconDownloadCompleted(bool bSuccess, FString Icon);

	UPROPERTY()
	TMap<FString, UTexture2D*> ImageMap;

	FBaseRequestCompleted OnPartnerAssetsDownloaded;
	FBaseRequestCompleted OnIconsDownloaded;

	TSharedPtr<class FRequestFactory> RequestFactory;
	TSharedPtr<class IBaseRequest> AssetRequest;
	TMap<FString, TSharedPtr<class IBaseRequest>> IconRequests;
};
