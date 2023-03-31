// Copyright © 2023++ Ready Player Me


#include "RpmAvatarRequestHandler.h"

#include "glTFRuntimeFunctionLibrary.h"
#include "Requests/RequestFactory.h"
#include "Requests/Endpoints.h"
#include "Extractors/PayloadExtractor.h"
#include "Utils/GlTFConfigCreator.h"

static const FString FULLBODY_BONE_NODE = "Armature";
static const FString HALFBODY_BONE_NODE = "AvatarRoot";

URpmAvatarRequestHandler::URpmAvatarRequestHandler()
	: Mesh(nullptr)
	, bAvatarExists(false)
	, bIsExistingAvatarUnchanged(false)
{
}

void URpmAvatarRequestHandler::SetRequestFactory(TSharedPtr<class FRequestFactory> Factory)
{
	RequestFactory = Factory;
}

FBaseRequestCompleted& URpmAvatarRequestHandler::GetAvatarPropertiesDownloadedCallback()
{
	return OnAvatarPropertiesDownloaded;
}

FBaseRequestCompleted& URpmAvatarRequestHandler::GetAvatarPreviewDownloadedCallback()
{
	return OnAvatarPreviewDownloaded;
}

void URpmAvatarRequestHandler::DownloadAvatarProperties(const FString& InAvatarId)
{
	bAvatarExists = true;
	//TODO: Fix the case when the item is selected in the UI but the Update request fails.
	bIsExistingAvatarUnchanged = bAvatarExists;
	AvatarProperties.Id = InAvatarId;
	AvatarMetadataRequest = RequestFactory->CreateAvatarMetadataRequest(AvatarProperties.Id);
	AvatarMetadataRequest->GetCompleteCallback().BindUObject(this, &URpmAvatarRequestHandler::OnPropertiesRequestCompleted);
	AvatarMetadataRequest->Download();
}

void URpmAvatarRequestHandler::CreateAvatar(const FRpmAvatarProperties& Properties)
{
	bAvatarExists = false;
	Mesh = nullptr;
	AvatarProperties = Properties;
	CreateAvatarRequest = RequestFactory->CreateAvatarCreateRequest(FPayloadExtractor::MakeCreatePayload(AvatarProperties));
	CreateAvatarRequest->GetCompleteCallback().BindUObject(this, &URpmAvatarRequestHandler::OnAvatarCreateCompleted);
	CreateAvatarRequest->Download();
}

FRpmAvatarProperties URpmAvatarRequestHandler::GetAvatarProperties() const
{
	return AvatarProperties;
}

void URpmAvatarRequestHandler::UpdateAvatar(ERpmPartnerAssetType AssetType, int64 AssetId)
{
	AvatarProperties.Assets.FindOrAdd(AssetType) = AssetId;
	UpdateAvatar(FPayloadExtractor::MakeUpdatePayload(AssetType, AssetId));
}

void URpmAvatarRequestHandler::UpdateAvatar(ERpmPartnerAssetColor AssetColor, int32 ColorIndex)
{
	AvatarProperties.Colors.FindOrAdd(AssetColor) = ColorIndex;
	UpdateAvatar(FPayloadExtractor::MakeUpdatePayload(AssetColor, ColorIndex));
}

void URpmAvatarRequestHandler::UpdateAvatar(const FString& Payload)
{
	bIsExistingAvatarUnchanged = false;
	if (UpdateAvatarRequest)
	{
		UpdateAvatarRequest->GetCompleteCallback().Unbind();
		UpdateAvatarRequest->CancelRequest();
	}
	UpdateAvatarRequest = RequestFactory->CreateUpdateAvatarRequest(AvatarProperties.Id, Payload);
	UpdateAvatarRequest->GetCompleteCallback().BindUObject(this, &URpmAvatarRequestHandler::OnUpdateAvatarCompleted);
	UpdateAvatarRequest->Download();
}

void URpmAvatarRequestHandler::OnUpdateAvatarCompleted(bool bSuccess)
{
	if (!bSuccess)
	{
		(void)OnPreviewDownloaded.ExecuteIfBound(nullptr);
		return;
	}
	LoadGlb(UpdateAvatarRequest->GetContent());
	UpdateAvatarRequest.Reset();
	(void)OnPreviewDownloaded.ExecuteIfBound(Mesh);
}

void URpmAvatarRequestHandler::SaveAvatar(const FAvatarSaveCompleted& AvatarSaveCompleted, const FAvatarCreatorFailed& Failed)
{
	if (bIsExistingAvatarUnchanged)
	{
		OnSaveAvatarCompleted(true, AvatarSaveCompleted, Failed);
	}
	SaveAvatarRequest = RequestFactory->CreateSaveAvatarRequest(AvatarProperties.Id);
	SaveAvatarRequest->GetCompleteCallback().BindUObject(this, &URpmAvatarRequestHandler::OnSaveAvatarCompleted, AvatarSaveCompleted, Failed);
	SaveAvatarRequest->Download();
}

void URpmAvatarRequestHandler::OnSaveAvatarCompleted(bool bSuccess, FAvatarSaveCompleted AvatarSaveCompleted, FAvatarCreatorFailed Failed)
{
	if (!bSuccess)
	{
		(void)Failed.ExecuteIfBound(ERpmAvatarCreatorError::AvatarSaveFailure);
		return;
	}
	(void)AvatarSaveCompleted.ExecuteIfBound(FEndpoints::GetAvatarPublicUrl(AvatarProperties.Id));
}

void URpmAvatarRequestHandler::DownloadModel(USkeleton* Skeleton)
{
	TargetSkeleton = Skeleton;
	AvatarModelRequest = RequestFactory->CreateAvatarModelRequest(AvatarProperties.Id, !bAvatarExists);
	AvatarModelRequest->GetCompleteCallback().BindUObject(this, &URpmAvatarRequestHandler::OnModelDownloadCompleted);
	AvatarModelRequest->Download();
}

void URpmAvatarRequestHandler::OnAvatarCreateCompleted(bool bSuccess)
{
	if (bSuccess)
	{
		AvatarProperties = FPayloadExtractor::ExtractPayload(CreateAvatarRequest->GetContentAsString());
	}

	(void)OnAvatarPropertiesDownloaded.ExecuteIfBound(bSuccess);
	OnAvatarPropertiesDownloaded.Unbind();
}

void URpmAvatarRequestHandler::OnPropertiesRequestCompleted(bool bSuccess)
{
	if (bSuccess)
	{
		AvatarProperties = FPayloadExtractor::ExtractPayload(AvatarMetadataRequest->GetContentAsString());
	}
	(void)OnAvatarPropertiesDownloaded.ExecuteIfBound(bSuccess);
	OnAvatarPropertiesDownloaded.Unbind();
}

void URpmAvatarRequestHandler::OnModelDownloadCompleted(bool bSuccess)
{
	if (bSuccess)
	{
		LoadGlb(AvatarModelRequest->GetContent());
		bSuccess = Mesh != nullptr;
	}
	(void)OnAvatarPreviewDownloaded.ExecuteIfBound(bSuccess);
	OnAvatarPreviewDownloaded.Unbind();
}

void URpmAvatarRequestHandler::LoadGlb(const TArray<uint8>& Data)
{
	UglTFRuntimeAsset* Asset = UglTFRuntimeFunctionLibrary::glTFLoadAssetFromData(Data, FGlTFConfigCreator::GetGlTFRuntimeConfig());
	
	if (Asset == nullptr)
	{
		return;
	}
	const FString RootBoneName = AvatarProperties.BodyType == EAvatarBodyType::FullBody ? FULLBODY_BONE_NODE : HALFBODY_BONE_NODE;
	Mesh = Asset->LoadSkeletalMeshRecursive(RootBoneName, {}, FGlTFConfigCreator::GetSkeletalMeshConfig(RootBoneName, TargetSkeleton));
}
