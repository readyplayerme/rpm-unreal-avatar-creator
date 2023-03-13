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
	void DownloadAssets(TSharedPtr<class FRequestFactory> Factory, EAvatarBodyType BodyType, EAvatarGender Gender);

	UPROPERTY()
	TArray<FRpmPartnerAsset> Assets;

	TArray<FRpmPartnerAsset> GetFilteredAssets(EAvatarBodyType BodyType, EAvatarGender Gender) const;

	FBaseRequestCompleted& GetPartnerAssetsDownloadCallback();

	bool AreAssetsReady() const;

private:
	UFUNCTION()
	void OnAssetsDownloadCompleted(bool bSuccess, EAvatarBodyType BodyType, EAvatarGender Gender);

	UFUNCTION()
	void OnIconDownloadCompleted(bool bSuccess, FString Icon);

	UPROPERTY()
	TMap<FString, UTexture2D*> ImageMap;

	void DownloadIcons(EAvatarBodyType BodyType, EAvatarGender Gender);

	FBaseRequestCompleted OnPartnerAssetsDownloaded;

	TSharedPtr<class FRequestFactory> RequestFactory;
	TSharedPtr<class FBaseRequest> AssetRequest;
	TMap<FString, TSharedPtr<class FBaseRequest>> IconRequests;
};
