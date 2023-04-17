// Copyright © 2023++ Ready Player Me


#include "RpmUserAvatarDownloader.h"

#include "Kismet/KismetRenderingLibrary.h"
#include "Requests/RequestFactory.h"
#include "Extractors/UserAvatarExtractor.h"
#include "Requests/Endpoints.h"

void URpmUserAvatarDownloader::SetRequestFactory(TSharedPtr<FRequestFactory> Factory)
{
	RequestFactory = Factory;
}

void URpmUserAvatarDownloader::DownloadUserAvatars(const FUserAvatarsDownloadCompleted& DownloadCompleted, const FAvatarCreatorFailed& Failed)
{
	OnAvatarsDownloaded = DownloadCompleted;
	OnFailed = Failed;
	if (UserAvatars.Num() != 0)
	{
		(void)OnAvatarsDownloaded.ExecuteIfBound(GetFilteredAvatars());
		OnFailed.Unbind();
		OnAvatarsDownloaded.Unbind();
	}
	else
	{
		AvatarsRequest = RequestFactory->CreateUserAvatarsRequest();
		AvatarsRequest->GetCompleteCallback().BindUObject(this, &URpmUserAvatarDownloader::OnAvatarsDownloadCompleted);
		AvatarsRequest->Download();
	}
}

TArray<FRpmUserAvatar> URpmUserAvatarDownloader::GetFilteredAvatars()
{
	for (auto& Avatar : UserAvatars)
	{
		if (ImageMap.Contains(Avatar.Id))
		{
			Avatar.Image = ImageMap[Avatar.Id];
		}
	}
	return UserAvatars;
}

void URpmUserAvatarDownloader::OnAvatarsDownloadCompleted(bool bSuccess)
{
	if (!bSuccess)
	{
		(void)OnFailed.ExecuteIfBound(ERpmAvatarCreatorError::UserAvatarDownloadFailure);
		OnFailed.Unbind();
		OnAvatarsDownloaded.Unbind();
		return;
	}
	UserAvatars = FUserAvatarExtractor::ExtractUserAvatars(AvatarsRequest->GetContentAsString());
	AvatarsRequest.Reset();
	(void)OnAvatarsDownloaded.ExecuteIfBound(GetFilteredAvatars());
	OnFailed.Unbind();
	OnAvatarsDownloaded.Unbind();
}

void URpmUserAvatarDownloader::SetImageDownloadDelegate(const FUserAvatarImageDownloadCompleted& ImageDownloaded)
{
	OnImageDownloaded = ImageDownloaded;
}

void URpmUserAvatarDownloader::AddAvatar(const FString& AvatarId, const FString& Partner)
{
	FRpmUserAvatar UserAvatar;
	UserAvatar.Id = AvatarId;
	UserAvatar.Partner = Partner;
	UserAvatars.Add(UserAvatar);
}

void URpmUserAvatarDownloader::DeleteAvatar(const FString& AvatarId)
{
	UserAvatars.RemoveAll([AvatarId](const auto& UserAvatar)
	{
		return UserAvatar.Id == AvatarId;
	});
}

void URpmUserAvatarDownloader::DownloadImages(const FString& Partner)
{
	for (const auto& Avatar : UserAvatars)
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
		IconRequest->GetCompleteCallback().BindUObject(this, &URpmUserAvatarDownloader::OnImageDownloadCompleted, Avatar.Id);
		IconRequest->Download();
	}
}

void URpmUserAvatarDownloader::OnImageDownloadCompleted(bool bSuccess, FString AvatarId)
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
