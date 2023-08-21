// Copyright © 2023++ Ready Player Me


#include "RpmAvatarCreatorApi.h"

#include "ReadyPlayerMeSettings.h"
#include "RpmPartnerAssetDownloader.h"
#include "RpmColorDownloader.h"
#include "RpmAuthManager.h"
#include "RpmAvatarRequestHandler.h"
#include "Requests/RequestFactory.h"
#include "RpmUserAvatarDownloader.h"
#include "RpmAvatarTemplateDownloader.h"
#include "RpmImageDownloader.h"
#include "Engine/SkeletalMesh.h"

URpmAvatarCreatorApi::URpmAvatarCreatorApi()
	: FullBodySkeleton(nullptr)
	, HalfBodySkeleton(nullptr)
{
	RequestFactory = MakeShared<FRequestFactory>();
	AuthManager = MakeShared<FRpmAuthManager>(RequestFactory);
	AuthManager->BindTokenRefreshDelegate();
	ImageDownloader = NewObject<URpmImageDownloader>();
	ImageDownloader->SetRequestFactory(RequestFactory);
	ColorDownloader = MakeShared<FRpmColorDownloader>(RequestFactory);
	AssetDownloader = MakeShared<FRpmPartnerAssetDownloader>(RequestFactory);
	AvatarTemplateDownloader = NewObject<URpmAvatarTemplateDownloader>();
	AvatarTemplateDownloader->SetRequestFactory(RequestFactory);
	UserAvatarDownloader = MakeShared<FRpmUserAvatarDownloader>(RequestFactory);
	AvatarRequestHandler = NewObject<URpmAvatarRequestHandler>();
	AvatarRequestHandler->SetRequestFactory(RequestFactory);
	AvatarRequestHandler->SetUserAvatarDownloader(UserAvatarDownloader);
	AvatarRequestHandler->ImageDownloader = ImageDownloader;
}

void URpmAvatarCreatorApi::SetPartnerDomain(const FString& PartnerDomain)
{
	AvatarProperties.Partner = PartnerDomain;
	RequestFactory->SetPartnerDomain(PartnerDomain);
	AuthManager->LoadUserData();
	const UReadyPlayerMeSettings* Settings = GetDefault<UReadyPlayerMeSettings>();
	const bool bIsAppIdSet = IsValid(Settings) && !Settings->AppId.IsEmpty();
	checkf(bIsAppIdSet, TEXT("AppId is required for the avatar creator. Set your ReadyPlayerMe AppId in the project settings"));
}

void URpmAvatarCreatorApi::SetPreviewDownloadedDelegate(const FPreviewDownloadCompleted& PreviewDownloaded)
{
	AvatarRequestHandler->OnPreviewDownloaded = PreviewDownloaded;
}

void URpmAvatarCreatorApi::SetSessionExpiredDelegate(const FSessionExpired& SessionExpired)
{
	RequestFactory->SetSessionExpiredDelegate(SessionExpired);
}

FRpmUserData URpmAvatarCreatorApi::GetUserData() const
{
	return AuthManager->GetUserData();
}

void URpmAvatarCreatorApi::AuthAnonymous(const FAuthenticationCompleted& Completed, const FAvatarCreatorFailed& Failed)
{
	AuthManager->AuthAnonymous(Completed, Failed);
}

void URpmAvatarCreatorApi::AuthStart(const FString& Email, bool bIsTypeCode, const FAuthenticationCompleted& Completed, const FAvatarCreatorFailed& Failed)
{
	AuthManager->AuthStart(Email, bIsTypeCode, Completed, Failed);
}

void URpmAvatarCreatorApi::ConfirmActivationCode(const FString& Code, const FAuthenticationCompleted& Completed, const FAvatarCreatorFailed& Failed)
{
	AuthManager->ConfirmActivationCode(Code, Completed, Failed);
}

void URpmAvatarCreatorApi::LogOut()
{
	AuthManager->Logout();
	const FString Partner = AvatarProperties.Partner;
	AvatarProperties = {};
	AvatarProperties.Partner = Partner;
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
		AvatarRequestHandler->CreateAvatar(AvatarProperties, SelectedAvatarTemplateId);
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

void URpmAvatarCreatorApi::AssetsDownloaded(bool bSuccess)
{
	if (!bSuccess)
	{
		ExecuteEditorReadyCallback(false, ERpmAvatarCreatorError::AssetDownloadFailure);
		return;
	}

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

void URpmAvatarCreatorApi::DownloadAvatarTemplates(const FAvatarTemplatesDownloadCompleted& DownloadCompleted, const FAvatarCreatorFailed& Failed)
{
	AvatarTemplateDownloader->DownloadTemplates(AvatarProperties.Gender, DownloadCompleted, Failed);
}

void URpmAvatarCreatorApi::DownloadUserAvatars(const FUserAvatarsDownloadCompleted& DownloadCompleted, const FAvatarCreatorFailed& Failed)
{
	UserAvatarDownloader->DownloadUserAvatars(DownloadCompleted, Failed);
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

void URpmAvatarCreatorApi::DeleteAvatar(const FString& AvatarId, bool bIsDraft, const FAvatarDeleteCompleted& AvatarDeleteCompleted, const FAvatarCreatorFailed& Failed)
{
	AvatarRequestHandler->DeleteAvatar(AvatarId, bIsDraft, AvatarDeleteCompleted, Failed);
}

void URpmAvatarCreatorApi::UpdateLockedAssets(const FUpdateLockedAssetsCompleted& UpdateLockedAssetsCompleted, const FAvatarCreatorFailed& Failed)
{
	
}

void URpmAvatarCreatorApi::DownloadImage(const FString& Url, int32 Size, const FImageDownloadCompleted& DownloadCompleted)
{
	return ImageDownloader->DownloadImage(Url, Size, DownloadCompleted);
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
	RequestFactory->CancelRequests();
	Super::BeginDestroy();
}
