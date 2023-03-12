// Copyright © 2023++ Ready Player Me


#include "RpmColorDownloader.h"
#include "Extractors/PartnerAssetExtractor.h"
#include "Requests/RequestFactory.h"

void FRpmColorDownloader::DownloadColors(TSharedPtr<FRequestFactory> Factory, const FString& AvatarId)
{
	Colors.Empty();
	RequestFactory = Factory;
	ColorRequest = RequestFactory->CreateColorRequest(AvatarId);
	ColorRequest->GetCompleteCallback().BindSP(AsShared(), &FRpmColorDownloader::DownloadCompleted);
	ColorRequest->Download();
}

FBaseRequestCompleted& FRpmColorDownloader::GetCompleteCallback()
{
	return OnColorDownloadCompleted;
}

void FRpmColorDownloader::DownloadCompleted(bool bSuccess)
{
	if (bSuccess)
	{
		Colors = FPartnerAssetExtractor::ExtractColors(ColorRequest->GetContentAsString());
		ColorRequest.Reset();
	}
	(void)OnColorDownloadCompleted.ExecuteIfBound(bSuccess);
	OnColorDownloadCompleted.Unbind();
}

TArray<FRpmColorPalette> FRpmColorDownloader::GetColors() const
{
	return Colors;
}
