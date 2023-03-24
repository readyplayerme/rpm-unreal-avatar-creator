// Copyright © 2023++ Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "RpmAvatarCreatorTypes.h"
#include "UObject/NoExportTypes.h"
#include "RpmAvatarCreatorApi.generated.h"

UCLASS(BlueprintType)
class RPMAVATARCREATOR_API URpmAvatarCreatorApi : public UObject
{
	GENERATED_BODY()

public:
	URpmAvatarCreatorApi();

	UFUNCTION(BlueprintCallable, Category = "Ready Player Me", meta = (DisplayName = "Set Partner Domain"))
	void SetPartnerDomain(const FString& PartnerDomain);

	UFUNCTION(BlueprintCallable, Category = "Ready Player Me", meta = (DisplayName = "Set Preview Downloaded Delegate"))
	void SetPreviewDownloadedDelegate(const FPreviewDownloadCompleted& PreviewDownloaded);

	UFUNCTION(BlueprintCallable, Category = "Ready Player Me", meta = (DisplayName = "Get User Data"))
	FRpmUserData GetUserData() const;

	UFUNCTION(BlueprintCallable, Category = "Ready Player Me", meta = (DisplayName = "Authenticate Anonymous"))
	void AuthAnonymous(const FAuthenticationCompleted& Completed, const FAvatarCreatorFailed& Failed);

	UFUNCTION(BlueprintCallable, Category = "Ready Player Me", meta = (DisplayName = "Send Activation Code"))
	void SendActivationCode(const FString& Email, const FAuthenticationCompleted& Completed, const FAvatarCreatorFailed& Failed);

	UFUNCTION(BlueprintCallable, Category = "Ready Player Me", meta = (DisplayName = "Confirm Activation Code"))
	void ConfirmActivationCode(const FString& Code, const FAuthenticationCompleted& Completed, const FAvatarCreatorFailed& Failed);

	UFUNCTION(BlueprintCallable, Category = "Ready Player Me", meta = (DisplayName = "LogOut"))
	void LogOut();

	UFUNCTION(BlueprintCallable, Category = "Ready Player Me", meta = (DisplayName = "Prepare Editor"))
	void PrepareEditor(const FAvatarEditorReady& EditorReady, const FAvatarCreatorFailed& Failed);

	UFUNCTION(BlueprintCallable, Category = "Ready Player Me", meta = (DisplayName = "Update Avatar Asset"))
	void UpdateAvatarAsset(ERpmPartnerAssetType AssetType, int64 AssetId);

	UFUNCTION(BlueprintCallable, Category = "Ready Player Me", meta = (DisplayName = "Update Avatar Color"))
	void UpdateAvatarColor(ERpmPartnerAssetColor AssetColor, int32 ColorIndex);

	UFUNCTION(BlueprintCallable, Category = "Ready Player Me", meta = (DisplayName = "Save Avatar"))
	void SaveAvatar(const FAvatarSaveCompleted& AvatarSaveCompleted, const FAvatarCreatorFailed& Failed);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ready Player Me", Meta = (ExposeOnSpawn="true"))
	class USkeleton* FullBodySkeleton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ready Player Me", Meta = (ExposeOnSpawn="true"))
	class USkeleton* HalfBodySkeleton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ready Player Me")
	FRpmAvatarProperties AvatarProperties;

	UFUNCTION(BlueprintCallable, Category = "Ready Player Me", meta = (DisplayName = "Get Avatar Properties"))
	FRpmAvatarProperties GetAvatarProperties() const;

	UFUNCTION(BlueprintCallable, Category = "Ready Player Me", meta = (DisplayName = "Get Filtered Partner Assets"))
	TArray<FRpmPartnerAsset> GetFilteredPartnerAssets() const;

	UFUNCTION(BlueprintCallable, Category = "Ready Player Me", meta = (DisplayName = "Get Color Palettes"))
	TArray<FRpmColorPalette> GetColorPalettes() const;

	UFUNCTION(BlueprintCallable, Category = "Ready Player Me", meta = (DisplayName = "Set Photo"))
	void SetProfilePhoto(UTextureRenderTarget2D* TextureRenderTarget);

	virtual void BeginDestroy() override;

private:
	UFUNCTION()
	void ColorsDownloaded(bool bSuccess);

	UFUNCTION()
	void AssetsDownloaded(bool bSuccess);

	UFUNCTION()
	void IconsDownloaded(bool bSuccess);

	UFUNCTION()
	void ModelDownloaded(bool bSuccess);

	UFUNCTION()
	void PropertiesDownloaded(bool bSuccess, ERpmAvatarCreatorError Error);

	void ExecuteEditorReadyCallback(bool bSuccess, ERpmAvatarCreatorError Error);

	TSharedPtr<class FRequestFactory> RequestFactory;

	TSharedPtr<class FRpmAuthManager> AuthManager;

	TSharedPtr<class FRpmColorDownloader> ColorDownloader;

	UPROPERTY()
	class URpmPartnerAssetDownloader* AssetDownloader;

	UPROPERTY()
	class URpmAvatarRequestHandler* AvatarRequestHandler;

	FAvatarEditorReady OnEditorReady;
	FAvatarCreatorFailed OnEditorFailed;
};
