// Copyright © 2023++ Ready Player Me


#include "RpmAvatarCreatorApi.h"

#include "RpmPartnerAssetLoader.h"
#include "RpmAuthManager.h"
#include "RpmAvatarRequestHandler.h"
#include "Requests/RequestFactory.h"
#include "ImageUtils.h"
#include "Serialization/BufferArchive.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Utils/PayloadUpdater.h"

URpmAvatarCreatorApi::URpmAvatarCreatorApi()
	: TargetSkeleton(nullptr)
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
	AvatarProperties = FPayloadUpdater::MakeAvatarProperties();
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

void URpmAvatarCreatorApi::PrepareEditor(const FAvatarEditorReady& EditorReady, const FAvatarCreatorFailed& Failed)
{
	AvatarRequestHandler->GetAvatarCreatedCallback().BindUObject(this, &URpmAvatarCreatorApi::AvatarCreated, EditorReady, Failed);
	AvatarRequestHandler->CreateAvatar(AvatarProperties, TargetSkeleton);

	AssetLoader->GetPartnerAssetsDownloadCallback().BindUObject(this, &URpmAvatarCreatorApi::AssetsDownloaded, EditorReady, Failed);
	AssetLoader->DownloadAssets(RequestFactory, AvatarProperties.BodyType, AvatarProperties.Gender);
}

void URpmAvatarCreatorApi::AssetsDownloaded(bool bSuccess, FAvatarEditorReady EditorReady, FAvatarCreatorFailed Failed)
{
	if (!bSuccess)
	{
		(void)Failed.ExecuteIfBound(ERpmAvatarCreatorError::AssetDownloadFailure);
		return;
	}
	if (AvatarRequestHandler->Mesh)
	{
		(void)EditorReady.ExecuteIfBound();
		(void)AvatarRequestHandler->OnPreviewDownloaded.ExecuteIfBound(AvatarRequestHandler->Mesh);
	}
}

void URpmAvatarCreatorApi::AvatarCreated(bool bSuccess, FAvatarEditorReady EditorReady, FAvatarCreatorFailed Failed)
{
	if (!bSuccess)
	{
		(void)Failed.ExecuteIfBound(ERpmAvatarCreatorError::AvatarPreviewFailure);
		return;
	}
	if (AssetLoader->AreAssetsReady())
	{
		(void)EditorReady.ExecuteIfBound();
		(void)AvatarRequestHandler->OnPreviewDownloaded.ExecuteIfBound(AvatarRequestHandler->Mesh);
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
