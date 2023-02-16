// Copyright © 2023++ Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RpmAvatarCreatorTypes.h"
#include "RpmPartnerAssetLoader.generated.h"

UCLASS(BlueprintType)
class RPMAVATARCREATOR_API URpmPartnerAssetLoader : public UObject
{
	GENERATED_BODY()

public:
	// UFUNCTION(BlueprintCallable, Category = "Ready Player Me", meta = (DisplayName = "Download Assets"))
	void DownloadAssets(TSharedPtr<class FRequestFactory> Factory, EAvatarBodyType BodyType, EAvatarGender Gender);

	UPROPERTY(BlueprintReadOnly, Category="Ready Player Me")
	TArray<FRpmPartnerAsset> Assets;

	FPartnerAssetsDownloadCompleted& GetPartnerAssetsDownloadCallback();

	bool AreAssetsReady() const;

private:
	UFUNCTION()
	void OnAssetsDownloadCompleted(bool bSuccess, EAvatarBodyType BodyType, EAvatarGender Gender);

	UFUNCTION()
	void OnIconDownloadCompleted(bool bSuccess, FString Icon);

	UPROPERTY()
	TMap<FString, UTexture2D*> ImageMap;

	void DownloadIcons(EAvatarBodyType BodyType, EAvatarGender Gender);

	FPartnerAssetsDownloadCompleted OnPartnerAssetsDownloaded;

	TSharedPtr<class FRequestFactory> RequestFactory;
	TSharedPtr<class FBaseRequest> AssetRequest;
	TMap<FString, TSharedPtr<class FBaseRequest>> IconRequests;
};
