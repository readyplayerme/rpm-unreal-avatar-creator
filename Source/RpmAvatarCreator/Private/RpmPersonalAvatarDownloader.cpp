// Copyright © 2023++ Ready Player Me


#include "RpmPersonalAvatarDownloader.h"

#include "Kismet/KismetRenderingLibrary.h"
#include "Requests/RequestFactory.h"
#include "Extractors/PersonalAvatarExtractor.h"
#include "Requests/Endpoints.h"

void URpmPersonalAvatarDownloader::SetRequestFactory(TSharedPtr<FRequestFactory> Factory)
{
	RequestFactory = Factory;
}

void URpmPersonalAvatarDownloader::DownloadPersonalAvatars(const FPersonalAvatarsDownloadCompleted& DownloadCompleted, const FAvatarCreatorFailed& Failed)
{
	OnAvatarsDownloaded = DownloadCompleted;
	OnFailed = Failed;
	if (PersonalAvatars.Num() != 0)
	{
		(void)OnAvatarsDownloaded.ExecuteIfBound(GetFilteredAvatars());
		OnFailed.Unbind();
		OnAvatarsDownloaded.Unbind();
	}
	else
	{
		AvatarsRequest = RequestFactory->CreatePersonalAvatarsRequest();
		AvatarsRequest->GetCompleteCallback().BindUObject(this, &URpmPersonalAvatarDownloader::OnAvatarsDownloadCompleted);
		AvatarsRequest->Download();
	}
}

TArray<FRpmPersonalAvatar> URpmPersonalAvatarDownloader::GetFilteredAvatars()
{
	for (auto& Avatar : PersonalAvatars)
	{
		if (ImageMap.Contains(Avatar.Id))
		{
			Avatar.Image = ImageMap[Avatar.Id];
		}
	}
	return PersonalAvatars;
}

void URpmPersonalAvatarDownloader::OnAvatarsDownloadCompleted(bool bSuccess)
{
	if (!bSuccess)
	{
		(void)OnFailed.ExecuteIfBound(ERpmAvatarCreatorError::PersonalAvatarDownloadFailure);
		OnFailed.Unbind();
		OnAvatarsDownloaded.Unbind();
		return;
	}
	PersonalAvatars = FPersonalAvatarExtractor::ExtractPersonalAvatars(AvatarsRequest->GetContentAsString());
	AvatarsRequest.Reset();
	(void)OnAvatarsDownloaded.ExecuteIfBound(GetFilteredAvatars());
	OnFailed.Unbind();
	OnAvatarsDownloaded.Unbind();
}

void URpmPersonalAvatarDownloader::SetImageDownloadDelegate(const FPersonalAvatarImageDownloadCompleted& ImageDownloaded)
{
	OnImageDownloaded = ImageDownloaded;
}

void URpmPersonalAvatarDownloader::DownloadImages(const FString& Partner)
{
	for (const auto& Avatar : PersonalAvatars)
	{
		if (!Partner.IsEmpty() && Partner != Avatar.Partner)
		{
			continue;
		}
		if (ImageMap.Contains(Avatar.Id))
		{
			(void)OnImageDownloaded.ExecuteIfBound(Avatar.Id, ImageMap[Avatar.Id]);
			continue;
		}
		if (ImageRequests.Contains(Avatar.Id))
		{
			continue;
		}

		auto IconRequest = RequestFactory->CreateImageRequest(FEndpoints::GetRenderEndpoint(Avatar.Id));
		ImageRequests.Add(Avatar.Id, IconRequest);
		IconRequest->GetCompleteCallback().BindUObject(this, &URpmPersonalAvatarDownloader::OnImageDownloadCompleted, Avatar.Id);
		IconRequest->Download();
	}
}

void URpmPersonalAvatarDownloader::OnImageDownloadCompleted(bool bSuccess, FString AvatarId)
{
	UTexture2D* Texture = nullptr;
	if (bSuccess)
	{
		Texture = UKismetRenderingLibrary::ImportBufferAsTexture2D(this, ImageRequests[AvatarId]->GetContent());
		ImageRequests.Remove(AvatarId);
		ImageMap.Add(AvatarId, Texture);
		// Check if we need to stop downloading other assets
	}

	(void)OnImageDownloaded.ExecuteIfBound(AvatarId, Texture);
}
