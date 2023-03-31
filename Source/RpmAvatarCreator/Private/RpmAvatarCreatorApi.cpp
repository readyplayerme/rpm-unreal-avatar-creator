// Copyright © 2023++ Ready Player Me


#include "RpmAvatarCreatorApi.h"

#include "RpmPartnerAssetDownloader.h"
#include "RpmColorDownloader.h"
#include "RpmAuthManager.h"
#include "RpmAvatarRequestHandler.h"
#include "Requests/RequestFactory.h"
#include "ImageUtils.h"
#include "RpmDefaultAvatarDownloader.h"
#include "Serialization/BufferArchive.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/SkeletalMesh.h"
#include "Misc/Base64.h"

URpmAvatarCreatorApi::URpmAvatarCreatorApi()
	: FullBodySkeleton(nullptr)
	, HalfBodySkeleton(nullptr)
{
	RequestFactory = MakeShared<FRequestFactory>();
	AuthManager = MakeShared<FRpmAuthManager>(RequestFactory);
	AuthManager->BindTokenRefreshDelegate();
	ColorDownloader = MakeShared<FRpmColorDownloader>(RequestFactory);
	AssetDownloader = NewObject<URpmPartnerAssetDownloader>();
	AssetDownloader->SetRequestFactory(RequestFactory);
	DefaultAvatarDownloader = NewObject<URpmDefaultAvatarDownloader>();
	DefaultAvatarDownloader->SetRequestFactory(RequestFactory);
	AvatarRequestHandler = NewObject<URpmAvatarRequestHandler>();
	AvatarRequestHandler->SetRequestFactory(RequestFactory);
}

void URpmAvatarCreatorApi::SetPartnerDomain(const FString& PartnerDomain)
{
	AvatarProperties.Partner = PartnerDomain;
	RequestFactory->SetPartnerDomain(PartnerDomain);
	AuthManager->LoadUserData();
}

void URpmAvatarCreatorApi::SetPreviewDownloadedDelegate(const FPreviewDownloadCompleted& PreviewDownloaded)
{
	AvatarRequestHandler->OnPreviewDownloaded = PreviewDownloaded;
}

void URpmAvatarCreatorApi::SetProfilePhoto(UTextureRenderTarget2D* TextureRenderTarget)
{
	FBufferArchive Buffer;
	if (FImageUtils::ExportRenderTarget2DAsPNG(TextureRenderTarget, Buffer))
	{
		AvatarProperties.Base64Image = FBase64::Encode(const_cast<uint8*>(Buffer.GetData()), Buffer.Num());
		AvatarProperties.Assets.Empty();
		AvatarProperties.Colors.Empty();
	}
}

FRpmUserData URpmAvatarCreatorApi::GetUserData() const
{
	return AuthManager->GetUserData();
}

void URpmAvatarCreatorApi::AuthAnonymous(const FAuthenticationCompleted& Completed, const FAvatarCreatorFailed& Failed)
{
	AuthManager->AuthAnonymous(Completed, Failed);
}

void URpmAvatarCreatorApi::SendActivationCode(const FString& Email, const FAuthenticationCompleted& Completed, const FAvatarCreatorFailed& Failed)
{
	AuthManager->SendActivationCode(Email, Completed, Failed);
}

void URpmAvatarCreatorApi::ConfirmActivationCode(const FString& Code, const FAuthenticationCompleted& Completed, const FAvatarCreatorFailed& Failed)
{
	AuthManager->ConfirmActivationCode(Code, Completed, Failed);
}

void URpmAvatarCreatorApi::LogOut()
{
	AuthManager->Logout();
}

void URpmAvatarCreatorApi::PropertiesDownloaded(bool bSuccess, ERpmAvatarCreatorError Error)
{
	if (!bSuccess)
	{
		ExecuteEditorReadyCallback(false, Error);
		return;
	}

	AvatarProperties = AvatarRequestHandler->GetAvatarProperties();
	AvatarRequestHandler->Mesh = nullptr;

	if (AssetDownloader->Assets.Num() == 0)
	{
		AssetDownloader->GetPartnerAssetsDownloadCallback().BindUObject(this, &URpmAvatarCreatorApi::AssetsDownloaded);
		AssetDownloader->DownloadAssets();
	}
	else
	{
		AssetsDownloaded(true);
	}
}

void URpmAvatarCreatorApi::PrepareEditor(const FAvatarEditorReady& EditorReady, const FAvatarCreatorFailed& Failed)
{
	OnEditorReady = EditorReady;
	OnEditorFailed = Failed;

	const ERpmAvatarCreatorError Error = !AvatarProperties.Id.IsEmpty() ?
		ERpmAvatarCreatorError::MetadataDownloadFailure : ERpmAvatarCreatorError::AvatarCreateFailure;
	AvatarRequestHandler->GetAvatarPropertiesDownloadedCallback().BindUObject(this, &URpmAvatarCreatorApi::PropertiesDownloaded, Error);
	if (AvatarProperties.Id.IsEmpty())
	{
		AvatarRequestHandler->CreateAvatar(AvatarProperties);
	}
	else
	{
		AvatarRequestHandler->DownloadAvatarProperties(AvatarProperties.Id);
	}
}

void URpmAvatarCreatorApi::ColorsDownloaded(bool bSuccess)
{
	if (!bSuccess)
	{
		ExecuteEditorReadyCallback(false, ERpmAvatarCreatorError::ColorDownloadFailure);
		return;
	}
	AvatarRequestHandler->GetAvatarPreviewDownloadedCallback().BindUObject(this, &URpmAvatarCreatorApi::ModelDownloaded);
	USkeleton* TargetSkeleton = AvatarProperties.BodyType == EAvatarBodyType::HalfBody ? HalfBodySkeleton : FullBodySkeleton;
	AvatarRequestHandler->DownloadModel(TargetSkeleton);
}

void URpmAvatarCreatorApi::IconsDownloaded(bool bSuccess)
{
	ExecuteEditorReadyCallback(bSuccess, ERpmAvatarCreatorError::AssetDownloadFailure);
}

void URpmAvatarCreatorApi::AssetsDownloaded(bool bSuccess)
{
	if (!bSuccess)
	{
		ExecuteEditorReadyCallback(false, ERpmAvatarCreatorError::AssetDownloadFailure);
		return;
	}
	AssetDownloader->GetIconsDownloadCallback().BindUObject(this, &URpmAvatarCreatorApi::IconsDownloaded);
	AssetDownloader->DownloadIcons(AvatarProperties.BodyType, AvatarProperties.Gender);

	ColorDownloader->GetCompleteCallback().BindUObject(this, &URpmAvatarCreatorApi::ColorsDownloaded);
	ColorDownloader->DownloadColors(AvatarProperties.Id);
}

void URpmAvatarCreatorApi::ModelDownloaded(bool bSuccess)
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
	if (AssetDownloader->AreAssetsReady() && IsValid(AvatarRequestHandler->Mesh))
	{
		(void)OnEditorReady.ExecuteIfBound();
		(void)AvatarRequestHandler->OnPreviewDownloaded.ExecuteIfBound(AvatarRequestHandler->Mesh);
		OnEditorFailed.Clear();
		OnEditorReady.Clear();
	}
}

void URpmAvatarCreatorApi::SetDefaultAvatarIds(const TArray<FString>& AvatarIds)
{
	DefaultAvatarDownloader->SetTemplateAvatarIds(AvatarIds);
}

void URpmAvatarCreatorApi::DownloadDefaultAvatars(const FDefaultAvatarsDownloadCompleted& DownloadCompleted, const FAvatarCreatorFailed& Failed)
{
	DefaultAvatarDownloader->DownloadDefaultAvatars(AvatarProperties.BodyType, AvatarProperties.Gender, DownloadCompleted, Failed);
}

void URpmAvatarCreatorApi::UpdateAvatarAsset(ERpmPartnerAssetType AssetType, int64 AssetId)
{
	AvatarRequestHandler->UpdateAvatar(AssetType, AssetId);
}

void URpmAvatarCreatorApi::UpdateAvatarColor(ERpmPartnerAssetColor AssetColor, int32 ColorIndex)
{
	AvatarRequestHandler->UpdateAvatar(AssetColor, ColorIndex);
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
	return AssetDownloader->GetFilteredAssets(AvatarProperties.BodyType, AvatarProperties.Gender);
}

TArray<FRpmColorPalette> URpmAvatarCreatorApi::GetColorPalettes() const
{
	return ColorDownloader->GetColors();
}

void URpmAvatarCreatorApi::BeginDestroy()
{
	//TODO: Cancel Load
	Super::BeginDestroy();
}
