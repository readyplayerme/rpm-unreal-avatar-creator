// Copyright © 2023++ Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "RpmAvatarCreatorTypes.h"
#include "UObject/NoExportTypes.h"
#include "RpmAvatarCreatorApi.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class RPMAVATARCREATOR_API URpmAvatarCreatorApi : public UObject
{
	GENERATED_BODY()

public:
	URpmAvatarCreatorApi();

	UFUNCTION(BlueprintCallable, Category = "Ready Player Me", meta = (DisplayName = "Authenticate"))
	void Authenticate(const FAuthenticationCompleted& Completed, const FAvatarCreatorFailed& Failed);

	UFUNCTION(BlueprintCallable, Category = "Ready Player Me", meta = (DisplayName = "Prepare Editor"))
	void PrepareEditor(const FAvatarEditorReady& EditorReady, const FAvatarCreatorFailed& Failed);

	UFUNCTION(BlueprintCallable, Category = "Ready Player Me", meta = (DisplayName = "Update Avatar"))
	void UpdateAvatar(ERpmPartnerAssetType AssetType, int64 AssetId);

	UFUNCTION(BlueprintCallable, Category = "Ready Player Me", meta = (DisplayName = "Save Avatar"))
	void SaveAvatar(const FAvatarSaveCompleted& AvatarSaveCompleted, const FAvatarCreatorFailed& Failed);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ready Player Me")
	USkeleton* TargetSkeleton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ready Player Me")
	FString PartnerDomain;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ready Player Me")
	FRpmAvatarProperties AvatarProperties;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ready Player Me")
	FPreviewDownloadCompleted OnPreviewDownloaded;

	UFUNCTION(BlueprintCallable, Category = "Ready Player Me", meta = (DisplayName = "Get Avatar Properties"))
	FRpmAvatarProperties GetAvatarProperties() const;

	UFUNCTION(BlueprintCallable, Category = "Ready Player Me", meta = (DisplayName = "Get Partner Assets"))
	const TArray<FRpmPartnerAsset>& GetPartnerAssets() const;

	virtual void BeginDestroy() override;

private:
	UFUNCTION()
	void OnAuthComplete(bool bSuccess);

	UFUNCTION()
	void AssetsDownloaded(bool bSuccess, FAvatarEditorReady EditorReady, FAvatarCreatorFailed Failed);

	UFUNCTION()
	void AvatarCreated(bool bSuccess, FAvatarEditorReady EditorReady, FAvatarCreatorFailed Failed);

	TSharedPtr<class FRequestFactory> RequestFactory;

	TSharedPtr<class FRpmAuthManager> AuthManager;

	UPROPERTY()
	class URpmPartnerAssetLoader* AssetLoader;

	UPROPERTY()
	class URpmAvatarRequestHandler* AvatarRequestHandler;

	FAuthenticationCompleted OnAuthenticationCompleted;
	FAvatarCreatorFailed OnAvatarCreatorFailed;

	FPartnerAssetsDownloadCompleted OnAssetsDownloadCompleted;
};
