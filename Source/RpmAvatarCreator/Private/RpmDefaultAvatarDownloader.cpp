// Copyright © 2023++ Ready Player Me


#include "RpmDefaultAvatarDownloader.h"

#include "Kismet/KismetRenderingLibrary.h"
#include "Requests/RequestFactory.h"
#include "Extractors/PayloadExtractor.h"

static const FString RENDER_URL_PREFIX = "https://models.readyplayer.me/";

namespace
{
	bool IsAvatarFiltered(const FRpmAvatarProperties& Properties, EAvatarBodyType BodyType, EAvatarGender Gender)
	{
		const bool BodyTypeFiltered = Properties.BodyType == BodyType;
		const bool GenderFiltered = Properties.Gender == Gender || Gender == EAvatarGender::Undefined;
		return BodyTypeFiltered && GenderFiltered;
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

void URpmDefaultAvatarDownloader::DownloadDefaultAvatars(EAvatarBodyType BodyType, EAvatarGender Gender, const FDefaultAvatarsDownloadCompleted& DownloadCompleted, const FAvatarCreatorFailed& Failed)
{
	OnDownloadCompleted = DownloadCompleted;
	OnFailed = Failed;
	SelectedBodyType = BodyType;
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

TArray<FRpmDefaultAvatarData> URpmDefaultAvatarDownloader::GetFilteredAvatars() const
{
	TArray<FRpmDefaultAvatarData> DefaultAvatars;
	for (const auto& Id : TemplateAvatarIds)
	{
		if (IsAvatarFiltered(TemplateProperties[Id], SelectedBodyType, SelectedGender))
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
		if (ImageMap.Contains(Properties.Key) || !IsAvatarFiltered(Properties.Value, SelectedBodyType, SelectedGender))
		{
			continue;
		}

		// "{0}/{1}.png?scene=halfbody-portrait-v1-transparent{2}{3}"
		const FString ImageUrl = RENDER_URL_PREFIX + Properties.Key + ".png";
		auto IconRequest = RequestFactory->CreateImageRequest(ImageUrl);
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
		// ImageMap.Empty();
		(void)OnDownloadCompleted.ExecuteIfBound(GetFilteredAvatars());
		OnFailed.Unbind();
		OnDownloadCompleted.Unbind();
	}
}
