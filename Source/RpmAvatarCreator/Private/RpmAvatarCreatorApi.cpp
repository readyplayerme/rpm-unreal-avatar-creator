// Copyright © 2023++ Ready Player Me


#include "RpmAvatarCreatorApi.h"

#include "RpmPartnerAssetLoader.h"
#include "RpmAuthManager.h"
#include "RpmAvatarRequestHandler.h"
#include "Requests/RequestFactory.h"
#include "ImageUtils.h"
#include "Serialization/BufferArchive.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Misc/Base64.h"

URpmAvatarCreatorApi::URpmAvatarCreatorApi()
	: FullBodySkeleton(nullptr)
	, HalfBodySkeleton(nullptr)
{
	AuthManager = MakeShared<FRpmAuthManager>();
	AssetLoader = NewObject<URpmPartnerAssetLoader>();
	AvatarRequestHandler = NewObject<URpmAvatarRequestHandler>();
}

void URpmAvatarCreatorApi::SetProfilePhoto(UTextureRenderTarget2D* TextureRenderTarget)
{
	FBufferArchive Buffer;
	bool bSuccess = FImageUtils::ExportRenderTarget2DAsPNG(TextureRenderTarget, Buffer);
	if (bSuccess)
	{
		AvatarProperties.Base64Image = FBase64::Encode(const_cast<uint8*>(Buffer.GetData()), Buffer.Num());
	}
}

void URpmAvatarCreatorApi::Authenticate(const FAuthenticationCompleted& Completed, const FAvatarCreatorFailed& Failed)
{
	AvatarProperties.Partner = PartnerDomain;
	OnAuthenticationCompleted = Completed;
	OnAvatarCreatorFailed = Failed;
	RequestFactory = MakeShared<FRequestFactory>(PartnerDomain);
	AuthManager->GetCompleteCallback().BindUObject(this, &URpmAvatarCreatorApi::OnAuthComplete);
	AuthManager->AuthAnonymous(RequestFactory);

	AvatarRequestHandler->Initialize(RequestFactory, OnPreviewDownloaded);
}

void URpmAvatarCreatorApi::OnAuthComplete(bool bSuccess)
{
	if (!bSuccess)
	{
		(void)OnAvatarCreatorFailed.ExecuteIfBound(ERpmAvatarCreatorError::AuthenticationFailure);
		return;
	}
	const FRpmUserSession Session = AuthManager->GetUserSession();
	RequestFactory->SetAuthToken(Session.Token);
	(void)OnAuthenticationCompleted.ExecuteIfBound();
}

void URpmAvatarCreatorApi::PropertiesDownloaded(bool bSuccess)
{
	if (!bSuccess)
	{
		const ERpmAvatarCreatorError Error = AvatarProperties.Id.IsEmpty() ?
			ERpmAvatarCreatorError::MetadataDownloadFailure : ERpmAvatarCreatorError::AvatarCreateFailure;
		(void)OnEditorFailed.ExecuteIfBound(Error);
		OnEditorFailed.Clear();
		OnEditorReady.Clear();
		return;
	}

	AvatarProperties = AvatarRequestHandler->GetAvatarProperties();
	AvatarRequestHandler->GetAvatarPreviewDownloadedCallback().BindUObject(this, &URpmAvatarCreatorApi::PreviewDownloaded);
	USkeleton* TargetSkeleton = AvatarProperties.BodyType == EAvatarBodyType::HalfBody ? HalfBodySkeleton : FullBodySkeleton;
	AvatarRequestHandler->DownloadPreview(TargetSkeleton);

	AssetLoader->GetPartnerAssetsDownloadCallback().BindUObject(this, &URpmAvatarCreatorApi::AssetsDownloaded);
	AssetLoader->DownloadAssets(RequestFactory, AvatarProperties.BodyType, AvatarProperties.Gender);
}

void URpmAvatarCreatorApi::PrepareEditor(const FAvatarEditorReady& EditorReady, const FAvatarCreatorFailed& Failed)
{
	OnEditorReady = EditorReady;
	OnEditorFailed = Failed;

	if (AvatarProperties.Id.IsEmpty())
	{
		AvatarRequestHandler->GetAvatarPropertiesDownloadedCallback().BindUObject(this, &URpmAvatarCreatorApi::PropertiesDownloaded);
		AvatarRequestHandler->CreateAvatar(AvatarProperties);
	}
	else
	{
		AvatarRequestHandler->GetAvatarPropertiesDownloadedCallback().BindUObject(this, &URpmAvatarCreatorApi::PropertiesDownloaded);
		AvatarRequestHandler->DownloadAvatarProperties(AvatarProperties.Id);
	}
}

void URpmAvatarCreatorApi::AssetsDownloaded(bool bSuccess)
{
	ExecuteEditorReadyCallback(bSuccess, ERpmAvatarCreatorError::AssetDownloadFailure);
}

void URpmAvatarCreatorApi::PreviewDownloaded(bool bSuccess)
{
	ExecuteEditorReadyCallback(bSuccess, ERpmAvatarCreatorError::AvatarPreviewFailure);
}

void URpmAvatarCreatorApi::ExecuteEditorReadyCallback(bool bSuccess, ERpmAvatarCreatorError Error)
{
	if (!bSuccess)
	{
		(void)OnEditorFailed.ExecuteIfBound(Error);
		OnEditorFailed.Clear();
		OnEditorReady.Clear();
		return;
	}
	if (AssetLoader->AreAssetsReady() && IsValid(AvatarRequestHandler->Mesh))
	{
		(void)OnEditorReady.ExecuteIfBound();
		(void)AvatarRequestHandler->OnPreviewDownloaded.ExecuteIfBound(AvatarRequestHandler->Mesh);
		OnEditorFailed.Clear();
		OnEditorReady.Clear();
	}
}

void URpmAvatarCreatorApi::UpdateAvatar(ERpmPartnerAssetType AssetType, int64 AssetId)
{
	AvatarRequestHandler->UpdateAvatar(AssetType, AssetId);
}

void URpmAvatarCreatorApi::SaveAvatar(const FAvatarSaveCompleted& AvatarSaveCompleted, const FAvatarCreatorFailed& Failed)
{
	AvatarRequestHandler->SaveAvatar(AvatarSaveCompleted, Failed);
}

FRpmAvatarProperties URpmAvatarCreatorApi::GetAvatarProperties() const
{
	return AvatarRequestHandler->GetAvatarProperties();
}

TArray<FRpmPartnerAsset> URpmAvatarCreatorApi::GetFilteredPartnerAssets() const
{
	return AssetLoader->GetFilteredAssets(AvatarProperties.BodyType, AvatarProperties.Gender);
}

void URpmAvatarCreatorApi::BeginDestroy()
{
	//TODO: Cancel Load
	Super::BeginDestroy();
}
