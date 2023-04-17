// Copyright © 2023++ Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RpmAvatarCreatorTypes.h"
#include "RpmUserAvatarDownloader.generated.h"

UCLASS(BlueprintType)
class RPMAVATARCREATOR_API URpmUserAvatarDownloader : public UObject
{
	GENERATED_BODY()

public:
	void DownloadUserAvatars(const FUserAvatarsDownloadCompleted& DownloadCompleted, const FAvatarCreatorFailed& Failed);

	void SetImageDownloadDelegate(const FUserAvatarImageDownloadCompleted& ImageDownloaded);

	void DownloadImages(const FString& Partner);

	void AddAvatar(const FString& AvatarId, const FString& Partner);

	void DeleteAvatar(const FString& AvatarId);

	void SetRequestFactory(TSharedPtr<class FRequestFactory> Factory);

	TArray<FRpmUserAvatar> GetFilteredAvatars();

private:
	void OnAvatarsDownloadCompleted(bool bSuccess);

	void OnImageDownloadCompleted(bool bSuccess, FString AvatarId);

	UPROPERTY()
	TMap<FString, UTexture2D*> ImageMap;

	FUserAvatarsDownloadCompleted OnAvatarsDownloaded;
	FAvatarCreatorFailed OnFailed;

	FUserAvatarImageDownloadCompleted OnImageDownloaded;

	TSharedPtr<class FRequestFactory> RequestFactory;
	TArray<FRpmUserAvatar> UserAvatars;
	TSharedPtr<class IBaseRequest> AvatarsRequest;
	TMap<FString, TSharedPtr<class IBaseRequest>> ImageRequests;
};
