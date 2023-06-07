// Copyright © 2023++ Ready Player Me


#include "RequestFactory.h"

#include "CoreMinimal.h"
#include "BaseRequest.h"
#include "Endpoints.h"
#include "Extractors/UserDataExtractor.h"

constexpr float IMAGE_REQUEST_TIMEOUT = 60.f;

FRequestFactory::FRequestFactory()
	: CancellationDelegate(MakeShared<FCancellationDelegate>())
{
}

void FRequestFactory::SetPartnerDomain(const FString& Domain)
{
	PartnerDomain = Domain;
}

void FRequestFactory::SetUserData(const FRpmUserData& Data)
{
	UserData = Data;
}

void FRequestFactory::SetTokenRefreshedDelegate(const FTokenRefreshed& TokenRefreshed)
{
	TokenRefreshedDelegate = TokenRefreshed;
}

void FRequestFactory::CancelRequests()
{
	CancellationDelegate->Broadcast();
}

TSharedPtr<IBaseRequest> FRequestFactory::CreateAuthStartRequest(const FString& PayloadJson) const
{
	return MakeShared<FBaseRequest>(CancellationDelegate, FEndpoints::GetAuthStartEndpoint(PartnerDomain), "", ERequestVerb::Post, PayloadJson);
}

TSharedPtr<IBaseRequest> FRequestFactory::CreateConfirmCodeRequest(const FString& PayloadJson) const
{
	return MakeShared<FBaseRequest>(CancellationDelegate, FEndpoints::GetConfirmCodeEndpoint(PartnerDomain), "", ERequestVerb::Post, PayloadJson);
}

TSharedPtr<IBaseRequest> FRequestFactory::CreateTokenRefreshRequest() const
{
	return MakeShared<FBaseRequest>(CancellationDelegate, FEndpoints::GetTokenRefreshEndpoint(PartnerDomain), "", ERequestVerb::Post, FUserDataExtractor::MakeTokenRefreshPayload(UserData));
}

TSharedPtr<IBaseRequest> FRequestFactory::CreateAuthAnonymousRequest() const
{
	return MakeShared<FBaseRequest>(CancellationDelegate, FEndpoints::GetAuthAnonymousEndpoint(PartnerDomain), "", ERequestVerb::Post);
}

TSharedPtr<IBaseRequest> FRequestFactory::CreateAvatarTemplatesRequest() const
{
	return CreateAuthorizedRequest(MakeShared<FBaseRequest>(CancellationDelegate, FEndpoints::GetAvatarTemplatesEndpoint(), UserData.Token));
}

TSharedPtr<IBaseRequest> FRequestFactory::CreateAssetRequest() const
{
	return CreateAuthorizedRequest(MakeShared<FBaseRequest>(CancellationDelegate, FEndpoints::GetAssetEndpoint(PartnerDomain), UserData.Token));
}

TSharedPtr<IBaseRequest> FRequestFactory::CreateColorRequest(const FString& AvatarId) const
{
	return CreateAuthorizedRequest(MakeShared<FBaseRequest>(CancellationDelegate, FEndpoints::GetColorEndpoint(AvatarId), UserData.Token));
}

TSharedPtr<IBaseRequest> FRequestFactory::CreateImageRequest(const FString& IconUrl) const
{
	return MakeShared<FBaseRequest>(CancellationDelegate, IconUrl);
}

TSharedPtr<IBaseRequest> FRequestFactory::CreateRenderRequest(const FString& AvatarId) const
{
	return MakeShared<FBaseRequest>(CancellationDelegate, FEndpoints::GetRenderEndpoint(AvatarId), "", ERequestVerb::Get, "", IMAGE_REQUEST_TIMEOUT);
}

TSharedPtr<IBaseRequest> FRequestFactory::CreateUserAvatarsRequest() const
{
	return CreateAuthorizedRequest(MakeShared<FBaseRequest>(CancellationDelegate, FEndpoints::GetUserAvatarsEndpoint(UserData.Id), UserData.Token));
}

TSharedPtr<IBaseRequest> FRequestFactory::CreateAvatarModelRequest(const FString& AvatarId, bool bIsPreview) const
{
	return MakeShared<FBaseRequest>(CancellationDelegate, FEndpoints::GetAvatarModelEndpoint(AvatarId, bIsPreview));
}

TSharedPtr<IBaseRequest> FRequestFactory::CreateAvatarMetadataRequest(const FString& AvatarId) const
{
	return MakeShared<FBaseRequest>(CancellationDelegate, FEndpoints::GetAvatarMetadataEndpoint(AvatarId));
}

TSharedPtr<IBaseRequest> FRequestFactory::CreateAvatarCreateRequest(const FString& PayloadJson, const FString& TemplateId) const
{
	if (TemplateId.IsEmpty())
	{
		return CreateAuthorizedRequest(MakeShared<FBaseRequest>(CancellationDelegate, FEndpoints::GetCreateEndpoint(), UserData.Token, ERequestVerb::Post, PayloadJson));
	}
	return CreateAuthorizedRequest(MakeShared<FBaseRequest>(CancellationDelegate, FEndpoints::GetAvatarTemplatesEndpoint(TemplateId), UserData.Token, ERequestVerb::Post, PayloadJson));
}

TSharedPtr<IBaseRequest> FRequestFactory::CreateUpdateAvatarRequest(const FString& AvatarId, const FString& PayloadJson) const
{
	return CreateAuthorizedRequest(MakeShared<FBaseRequest>(CancellationDelegate, FEndpoints::GetUpdateAvatarEndpoint(AvatarId), UserData.Token, ERequestVerb::Patch, PayloadJson));
}

TSharedPtr<IBaseRequest> FRequestFactory::CreateSaveAvatarRequest(const FString& AvatarId) const
{
	return CreateAuthorizedRequest(MakeShared<FBaseRequest>(CancellationDelegate, FEndpoints::GetSaveAvatarEndpoint(AvatarId), UserData.Token, ERequestVerb::Put));
}

TSharedPtr<IBaseRequest> FRequestFactory::CreateDeleteAvatarRequest(const FString& AvatarId, bool bIsDraft) const
{
	return CreateAuthorizedRequest(MakeShared<FBaseRequest>(CancellationDelegate, FEndpoints::GetDeleteAvatarEndpoint(AvatarId, bIsDraft), UserData.Token, ERequestVerb::Delete));
}

TSharedPtr<IBaseRequest> FRequestFactory::CreateAuthorizedRequest(TSharedPtr<IBaseRequest> MainRequest) const
{
	if (!UserData.bIsExistingUser)
	{
		return MainRequest;
	}
	return MakeShared<FAuthorizedRequest>(MainRequest, CreateTokenRefreshRequest(), TokenRefreshedDelegate);
}
