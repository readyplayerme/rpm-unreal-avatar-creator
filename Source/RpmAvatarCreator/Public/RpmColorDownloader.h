// Copyright © 2023++ Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "RpmAvatarCreatorTypes.h"

DECLARE_DELEGATE_OneParam(FColorDownloadCompleted, bool /*bSuccess*/);

class RPMAVATARCREATOR_API FRpmColorDownloader : public TSharedFromThis<FRpmColorDownloader>
{
public:
	void DownloadColors(TSharedPtr<class FRequestFactory> RequestFactory, const FString& AvatarId);
	TArray<FRpmColorPalette> GetColors() const;

	FColorDownloadCompleted& GetCompleteCallback();

private:
	void DownloadCompleted(bool bSuccess);

	FColorDownloadCompleted OnColorDownloadCompleted;

private:
	TSharedPtr<class FRequestFactory> RequestFactory;
	TArray<FRpmColorPalette> Colors;
	TSharedPtr<class FBaseRequest> ColorRequest;
};
