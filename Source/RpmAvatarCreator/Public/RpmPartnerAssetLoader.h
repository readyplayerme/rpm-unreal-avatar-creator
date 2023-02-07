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
	void DownloadAssets(TSharedPtr<class FRequestFactory> Factory);

	UPROPERTY(BlueprintReadOnly, Category="Ready Player Me")
	TArray<FRpmPartnerAsset> Assets;

	FPartnerAssetsDownloadCompleted& GetPartnerAssetsDownloadCallback();

	bool AreAssetsReady() const;

private:
	UFUNCTION()
	void OnAssetsDownloadCompleted(bool bSuccess);

	UFUNCTION()
	void OnIconDownloadCompleted(bool bSuccess, int32 Index);

	UFUNCTION()
	void OnBadgeDownloadCompleted(bool bSuccess, FString Badge);

	void DownloadIcons();

	FPartnerAssetsDownloadCompleted OnPartnerAssetsDownloaded;

	TSharedPtr<class FRequestFactory> RequestFactory;
	TSharedPtr<class FBaseRequest> AssetRequest;
	TMap<int32, TSharedPtr<class FBaseRequest>> IconRequests;
	TMap<FString, TSharedPtr<class FBaseRequest>> BadgeRequests;

	bool bAssetsReady = false;
};
