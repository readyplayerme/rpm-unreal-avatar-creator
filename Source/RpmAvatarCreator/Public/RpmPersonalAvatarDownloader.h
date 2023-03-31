// Copyright © 2023++ Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RpmAvatarCreatorTypes.h"
#include "RpmPersonalAvatarDownloader.generated.h"

UCLASS(BlueprintType)
class RPMAVATARCREATOR_API URpmPersonalAvatarDownloader : public UObject
{
	GENERATED_BODY()

public:
	void DownloadPersonalAvatars(const FPersonalAvatarsDownloadCompleted& DownloadCompleted, const FAvatarCreatorFailed& Failed);

	void SetImageDownloadDelegate(const FPersonalAvatarImageDownloadCompleted& ImageDownloaded);

	void DownloadImages(const FString& Partner);

	void SetRequestFactory(TSharedPtr<class FRequestFactory> Factory);

	TArray<FRpmPersonalAvatar> GetFilteredAvatars();

private:
	void OnAvatarsDownloadCompleted(bool bSuccess);

	void OnImageDownloadCompleted(bool bSuccess, FString AvatarId);

	UPROPERTY()
	TMap<FString, UTexture2D*> ImageMap;

	FPersonalAvatarsDownloadCompleted OnAvatarsDownloaded;
	FAvatarCreatorFailed OnFailed;

	FPersonalAvatarImageDownloadCompleted OnImageDownloaded;

	TSharedPtr<class FRequestFactory> RequestFactory;
	TArray<FRpmPersonalAvatar> PersonalAvatars;
	TSharedPtr<class IBaseRequest> AvatarsRequest;
	TMap<FString, TSharedPtr<class IBaseRequest>> ImageRequests;
};
