// Copyright © 2023++ Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RpmAvatarCreatorTypes.h"
#include "RpmDefaultAvatarDownloader.generated.h"

UCLASS(BlueprintType)
class RPMAVATARCREATOR_API URpmDefaultAvatarDownloader : public UObject
{
	GENERATED_BODY()

public:
	void SetTemplateAvatarIds(const TArray<FString>& AvatarIds);

	void DownloadDefaultAvatars(EAvatarBodyType BodyType, EAvatarGender Gender, const FDefaultAvatarsDownloadCompleted& DownloadCompleted, const FAvatarCreatorFailed& Failed);

	void SetRequestFactory(TSharedPtr<class FRequestFactory> Factory);

	TArray<FRpmDefaultAvatarData> GetFilteredAvatars() const;

	bool AreAvatarsReady() const;

private:
	void OnPropertiesDownloadCompleted(bool bSuccess, FString AvatarId);

	void OnImageDownloadCompleted(bool bSuccess, FString AvatarId);

	void DownloadImages();

	UPROPERTY()
	TMap<FString, UTexture2D*> ImageMap;

	FDefaultAvatarsDownloadCompleted OnDownloadCompleted;
	FAvatarCreatorFailed OnFailed;

	EAvatarBodyType SelectedBodyType;
	EAvatarGender SelectedGender;

	TArray<FString> TemplateAvatarIds;

	TSharedPtr<class FRequestFactory> RequestFactory;
	TMap<FString, FRpmAvatarProperties> TemplateProperties;
	TMap<FString, TSharedPtr<class IBaseRequest>> PropertiesRequests;
	TMap<FString, TSharedPtr<class IBaseRequest>> ImageRequests;
};
