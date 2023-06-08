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

	void SetRequestFactory(TSharedPtr<class FRequestFactory> Factory);

	void CreateAvatar(const FRpmAvatarProperties& Properties, const FString& TemplateId);

	void UpdateAvatar(ERpmPartnerAssetType AssetType, int64 AssetId);

	void UpdateAvatar(ERpmPartnerAssetColor AssetColor, int32 ColorIndex);

	void DownloadAvatarProperties(const FString& InAvatarId);

	void DownloadModel(class USkeleton* Skeleton);

	void SaveAvatar(const FAvatarSaveCompleted& AvatarSaveCompleted, const FAvatarCreatorFailed& Failed);

	void DeleteAvatar(const FString& AvatarId, bool bIsDraft, const FAvatarDeleteCompleted& AvatarDeleteCompleted, const FAvatarCreatorFailed& Failed);

	FRpmAvatarProperties GetAvatarProperties() const;

	UPROPERTY()
	USkeletalMesh* Mesh;
	
	UPROPERTY()
	class URpmUserAvatarDownloader* UserAvatarDownloader;
	
	UPROPERTY()
	FPreviewDownloadCompleted OnPreviewDownloaded;

	FBaseRequestCompleted& GetAvatarPropertiesDownloadedCallback();
	
	FBaseRequestCompleted& GetAvatarPreviewDownloadedCallback();

private:
	UFUNCTION()
	void OnUpdateAvatarCompleted(bool bSuccess);

	UFUNCTION()
	void OnSaveAvatarCompleted(bool bSuccess, FAvatarSaveCompleted AvatarSaveCompleted, FAvatarCreatorFailed Failed);

	UFUNCTION()
	void OnDeleteAvatarCompleted(bool bSuccess, FAvatarDeleteCompleted AvatarDeleteCompleted, FAvatarCreatorFailed Failed, FString AvatarId, bool bIsDraft);

	UFUNCTION()
	void OnModelDownloadCompleted(bool bSuccess);

	UFUNCTION()
	void OnPropertiesRequestCompleted(bool bSuccess);

	void UpdateAvatar(const FString& Payload);

	void LoadGlb(const TArray<uint8>& Data);

	UPROPERTY()
	USkeleton* TargetSkeleton;

	bool bAvatarExists;
	bool bIsExistingAvatarUnchanged;

	FRpmAvatarProperties AvatarProperties;

	FBaseRequestCompleted OnAvatarPropertiesDownloaded;
	FBaseRequestCompleted OnAvatarPreviewDownloaded;

	TSharedPtr<class FRequestFactory> RequestFactory;
	TSharedPtr<class IBaseRequest> AvatarMetadataRequest;
	TSharedPtr<class IBaseRequest> UpdateAvatarRequest;
	TSharedPtr<class IBaseRequest> SaveAvatarRequest;
	TSharedPtr<class IBaseRequest> DeleteAvatarRequest;
	TSharedPtr<class IBaseRequest> AvatarModelRequest;
};
