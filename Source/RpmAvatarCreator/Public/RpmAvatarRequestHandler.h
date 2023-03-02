// Copyright © 2023++ Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "RpmAvatarCreatorTypes.h"
#include "UObject/NoExportTypes.h"
#include "RpmAvatarRequestHandler.generated.h"

UCLASS(BlueprintType)
class RPMAVATARCREATOR_API URpmAvatarRequestHandler : public UObject
{
	GENERATED_BODY()

public:
	URpmAvatarRequestHandler();

	void Initialize(TSharedPtr<class FRequestFactory> RequestFactory, const FPreviewDownloadCompleted& PreviewDownloaded);

	void CreateAvatar(const FRpmAvatarProperties& Properties);

	void UpdateAvatar(ERpmPartnerAssetType AssetType, int64 AssetId);

	void DownloadAvatarProperties(const FString& InAvatarId);

	void DownloadPreview(USkeleton* Skeleton);

	void SaveAvatar(const FAvatarSaveCompleted& AvatarSaveCompleted, const FAvatarCreatorFailed& Failed);
	
	FRpmAvatarProperties GetAvatarProperties() const;

	UPROPERTY()
	USkeletalMesh* Mesh;
	
	UPROPERTY()
	FPreviewDownloadCompleted OnPreviewDownloaded;

	FAvatarCreateCompleted& GetAvatarPropertiesDownloadedCallback();
	
	FAvatarPreviewDownloadCompleted& GetAvatarPreviewDownloadedCallback();

private:
	UFUNCTION()
	void OnAvatarCreateCompleted(bool bSuccess);

	UFUNCTION()
	void OnUpdateAvatarCompleted(bool bSuccess);

	UFUNCTION()
	void OnSaveAvatarCompleted(bool bSuccess, FAvatarSaveCompleted AvatarSaveCompleted, FAvatarCreatorFailed Failed);

	UFUNCTION()
	void OnPreviewDownloadCompleted(bool bSuccess);

	UFUNCTION()
	void OnPropertiesRequestCompleted(bool bSuccess);

	void LoadGlb(const TArray<uint8>& Data);

	UPROPERTY()
	USkeleton* TargetSkeleton;

	FRpmAvatarProperties AvatarProperties;

	FAvatarCreateCompleted OnAvatarPropertiesDownloaded;
	FAvatarPreviewDownloadCompleted OnAvatarPreviewDownloaded;

	TSharedPtr<class FRequestFactory> RequestFactory;
	TSharedPtr<class FBaseRequest> AvatarMetadataRequest;
	TSharedPtr<class FBaseRequest> CreateAvatarRequest;
	TSharedPtr<class FBaseRequest> UpdateAvatarRequest;
	TSharedPtr<class FBaseRequest> SaveAvatarRequest;
	TSharedPtr<class FBaseRequest> PreviewAvatarRequest;
};
