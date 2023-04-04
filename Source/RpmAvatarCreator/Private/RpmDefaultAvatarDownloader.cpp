// Copyright © 2023++ Ready Player Me


#include "RpmDefaultAvatarDownloader.h"

#include "Kismet/KismetRenderingLibrary.h"
#include "Requests/RequestFactory.h"
#include "Extractors/PayloadExtractor.h"
#include "Requests/Endpoints.h"

namespace
{
	bool IsAvatarFiltered(const FRpmAvatarProperties& Properties, EAvatarGender Gender)
	{
		return Properties.Gender == Gender || Gender == EAvatarGender::Undefined;
	}
}

void URpmDefaultAvatarDownloader::SetRequestFactory(TSharedPtr<FRequestFactory> Factory)
{
	RequestFactory = Factory;
}

void URpmDefaultAvatarDownloader::SetTemplateAvatarIds(const TArray<FString>& AvatarIds)
{
	TemplateAvatarIds = AvatarIds;
}

void URpmDefaultAvatarDownloader::DownloadDefaultAvatars(EAvatarGender Gender, const FDefaultAvatarsDownloadCompleted& DownloadCompleted, const FAvatarCreatorFailed& Failed)
{
	OnDownloadCompleted = DownloadCompleted;
	OnFailed = Failed;
	SelectedGender = Gender;
	if (TemplateProperties.Num() != 0)
	{
		DownloadImages();
	}
	else
	{
		for (const auto& AvatarId : TemplateAvatarIds)
		{
			auto PropertiesRequest = RequestFactory->CreateAvatarMetadataRequest(AvatarId);
			PropertiesRequests.Add(AvatarId, PropertiesRequest);
			PropertiesRequest->GetCompleteCallback().BindUObject(this, &URpmDefaultAvatarDownloader::OnPropertiesDownloadCompleted, AvatarId);
			PropertiesRequest->Download();
		}
	}
}

bool URpmDefaultAvatarDownloader::IsValidDefaultAvatar(const FString& Id) const
{
	return TemplateProperties.Contains(Id) && ImageMap.Contains(Id) && IsAvatarFiltered(TemplateProperties[Id], SelectedGender);
}

TArray<FRpmDefaultAvatarData> URpmDefaultAvatarDownloader::GetFilteredAvatars() const
{
	TArray<FRpmDefaultAvatarData> DefaultAvatars;
	for (const auto& Id : TemplateAvatarIds)
	{
		if (IsValidDefaultAvatar(Id))
		{
			FRpmDefaultAvatarData Data;
			Data.Image = ImageMap[Id];
			Data.Properties = TemplateProperties[Id];
			DefaultAvatars.Add(Data);
		}
	}
	return DefaultAvatars;
}

bool URpmDefaultAvatarDownloader::AreAvatarsReady() const
{
	return ImageRequests.Num() == 0 && PropertiesRequests.Num() == 0;
}

void URpmDefaultAvatarDownloader::OnPropertiesDownloadCompleted(bool bSuccess, FString AvatarId)
{
	if (!bSuccess)
	{
		(void)OnFailed.ExecuteIfBound(ERpmAvatarCreatorError::DefaultAvatarFailure);
		OnFailed.Unbind();
		OnDownloadCompleted.Unbind();
		return;
	}
	const auto Request = PropertiesRequests[AvatarId];
	const FRpmAvatarProperties Properties = FPayloadExtractor::ExtractPayload(Request->GetContentAsString());
	TemplateProperties.Add(AvatarId, Properties);
	PropertiesRequests.Remove(AvatarId);
	if (PropertiesRequests.Num() == 0)
	{
		DownloadImages();
	}
}

void URpmDefaultAvatarDownloader::DownloadImages()
{
	for (const auto& Properties : TemplateProperties)
	{
		if (ImageMap.Contains(Properties.Key) || !IsAvatarFiltered(Properties.Value, SelectedGender))
		{
			continue;
		}

		auto IconRequest = RequestFactory->CreateImageRequest(FEndpoints::GetRenderEndpoint(Properties.Key));
		ImageRequests.Add(Properties.Key, IconRequest);
		IconRequest->GetCompleteCallback().BindUObject(this, &URpmDefaultAvatarDownloader::OnImageDownloadCompleted, Properties.Key);
		IconRequest->Download();
	}
	if (ImageRequests.Num() == 0)
	{
		(void)OnDownloadCompleted.ExecuteIfBound(GetFilteredAvatars());
		OnFailed.Unbind();
		OnDownloadCompleted.Unbind();
	}
}

void URpmDefaultAvatarDownloader::OnImageDownloadCompleted(bool bSuccess, FString AvatarId)
{
	if (!bSuccess)
	{
		(void)OnFailed.ExecuteIfBound(ERpmAvatarCreatorError::DefaultAvatarFailure);
		OnFailed.Unbind();
		OnDownloadCompleted.Unbind();
		// Check if we need to stop downloading other assets
		return;
	}

	UTexture2D* Texture = UKismetRenderingLibrary::ImportBufferAsTexture2D(this, ImageRequests[AvatarId]->GetContent());
	ImageRequests.Remove(AvatarId);
	ImageMap.Add(AvatarId, Texture);

	if (ImageRequests.Num() == 0)
	{
		(void)OnDownloadCompleted.ExecuteIfBound(GetFilteredAvatars());
		OnFailed.Unbind();
		OnDownloadCompleted.Unbind();
	}
}
