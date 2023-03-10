// Copyright © 2023++ Ready Player Me


#include "RpmAvatarRequestHandler.h"

#include "glTFRuntimeFunctionLibrary.h"
#include "Requests/RequestFactory.h"
#include "Requests/Endpoints.h"
#include "Extractors/PayloadExtractor.h"
#include "Utils/GlTFConfigCreator.h"
#include "Utils/PayloadUpdater.h"

static const FString FULLBODY_BONE_NODE = "Armature";
static const FString HALFBODY_BONE_NODE = "AvatarRoot";

namespace
{
	TMap<ERpmPartnerAssetColor, int32> MakeDefaultColors()
	{
		return
		{
			{ERpmPartnerAssetColor::SkinColor, 5},
			{ERpmPartnerAssetColor::BeardColor, 0},
			{ERpmPartnerAssetColor::HairColor, 0},
			{ERpmPartnerAssetColor::EyebrowColor, 0}
		};
	}

	TMap<ERpmPartnerAssetType, int64> MakeDefaultPartnerAssets()
	{
		return
		{
			{ERpmPartnerAssetType::EyeColor, 9781796},
			{ERpmPartnerAssetType::HairStyle, 23368535},
			{ERpmPartnerAssetType::EyebrowStyle, 41308196},
			{ERpmPartnerAssetType::Shirt, 9247449},
			{ERpmPartnerAssetType::Outfit, 109373713}
		};
	}
}

URpmAvatarRequestHandler::URpmAvatarRequestHandler()
	: Mesh(nullptr)
{
}

void URpmAvatarRequestHandler::Initialize(TSharedPtr<FRequestFactory> Factory, const FPreviewDownloadCompleted& PreviewDownloaded)
{
	RequestFactory = Factory;
	OnPreviewDownloaded = PreviewDownloaded;
}

FAvatarCreateCompleted& URpmAvatarRequestHandler::GetAvatarCreatedCallback()
{
	return OnAvatarCreated;
}

void URpmAvatarRequestHandler::CreateAvatar(const FRpmAvatarProperties& Properties, USkeleton* Skeleton)
{
	Mesh = nullptr;
	AvatarProperties = Properties;
	// TODO: Fix this when the default avatar api is available
	AvatarProperties.Colors = MakeDefaultColors();
	AvatarProperties.Assets = MakeDefaultPartnerAssets();
	// TODO: Fix this when the default avatar api is available
	AvatarProperties.Assets[ERpmPartnerAssetType::Outfit] = AvatarProperties.Gender == EAvatarGender::Feminine ? 109376347 : 109373713;
	TargetSkeleton = Skeleton;
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
	FPayloadUpdater::UpdatePayload(AvatarProperties, AssetType, AssetId);
	if (UpdateAvatarRequest)
	{
		UpdateAvatarRequest->GetCompleteCallback().Unbind();
		UpdateAvatarRequest->CancelRequest();
	}
	UpdateAvatarRequest = RequestFactory->CreateUpdateAvatarRequest(AvatarId, FPayloadExtractor::MakeUpdatePayload(AssetType, AssetId));
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
	SaveAvatarRequest = RequestFactory->CreateSaveAvatarRequest(AvatarId);
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
	(void)AvatarSaveCompleted.ExecuteIfBound(FEndpoints::GetAvatarPublicUrl(AvatarId));
}

void URpmAvatarRequestHandler::OnAvatarCreateCompleted(bool bSuccess)
{
	if (!bSuccess)
	{
		(void)OnAvatarCreated.ExecuteIfBound(false);
		OnAvatarCreated.Unbind();
		return;
	}

	AvatarId = FPayloadExtractor::ExtractAvatarId(CreateAvatarRequest->GetContentAsString());
	AvatarProperties = FPayloadExtractor::ExtractPayload(CreateAvatarRequest->GetContentAsString());
	PreviewAvatarRequest = RequestFactory->CreateAvatarPreviewRequest(AvatarId);
	PreviewAvatarRequest->GetCompleteCallback().BindUObject(this, &URpmAvatarRequestHandler::OnPreviewDownloadCompleted);
	PreviewAvatarRequest->Download();
}

void URpmAvatarRequestHandler::OnPreviewDownloadCompleted(bool bSuccess)
{
	if (bSuccess)
	{
		LoadGlb(PreviewAvatarRequest->GetContent());
		bSuccess = Mesh != nullptr;
	}
	(void)OnAvatarCreated.ExecuteIfBound(bSuccess);
	OnAvatarCreated.Unbind();
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
