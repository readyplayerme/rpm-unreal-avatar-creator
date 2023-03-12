// Copyright © 2023++ Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "RpmAvatarCreatorTypes.h"

class RPMAVATARCREATOR_API FRpmColorDownloader : public TSharedFromThis<FRpmColorDownloader>
{
public:
	void DownloadColors(TSharedPtr<class FRequestFactory> RequestFactory, const FString& AvatarId);
	TArray<FRpmColorPalette> GetColors() const;

	FBaseRequestCompleted& GetCompleteCallback();

private:
	void DownloadCompleted(bool bSuccess);

	FBaseRequestCompleted OnColorDownloadCompleted;

private:
	TSharedPtr<class FRequestFactory> RequestFactory;
	TArray<FRpmColorPalette> Colors;
	TSharedPtr<class FBaseRequest> ColorRequest;
};
