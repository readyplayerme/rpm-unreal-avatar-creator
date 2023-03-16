// Copyright © 2023++ Ready Player Me


#include "RequestFactory.h"

#include "CoreMinimal.h"
#include "Endpoints.h"
#include "Extractors/UserSessionExtractor.h"

void FRequestFactory::SetPartnerDomain(const FString& Domain)
{
	PartnerDomain = Domain;
}

void FRequestFactory::SetUserSession(const FRpmUserSession& Session)
{
	UserSession = Session;
}

void FRequestFactory::SetTokenRefreshedDelegate(const FTokenRefreshed& TokenRefreshed)
{
	TokenRefreshedDelegate = TokenRefreshed;
}

TSharedPtr<FBaseRequest> FRequestFactory::CreateSendCodeRequest(const FString& PayloadJson) const
{
	return MakeShared<FBaseRequest>(FEndpoints::GetSendCodeEndpoint(PartnerDomain), "", "POST", PayloadJson);
}

TSharedPtr<FBaseRequest> FRequestFactory::CreateConfirmCodeRequest(const FString& PayloadJson) const
{
	return MakeShared<FBaseRequest>(FEndpoints::GetConfirmCodeEndpoint(PartnerDomain), "", "POST", PayloadJson);
}

TSharedPtr<FBaseRequest> FRequestFactory::CreateTokenRefreshRequest() const
{
	return MakeShared<FBaseRequest>(FEndpoints::GetTokenRefreshEndpoint(PartnerDomain), "", "POST", FUserSessionExtractor::MakeTokenRefreshPayload(UserSession));
}

TSharedPtr<FBaseRequest> FRequestFactory::CreateAuthAnonymousRequest() const
{
	return MakeShared<FBaseRequest>(FEndpoints::GetAuthAnonymousEndpoint(PartnerDomain), "", "POST");
}

TSharedPtr<FBaseRequest> FRequestFactory::CreateAssetRequest() const
{
	return CreateAuthorizedRequest(MakeShared<FBaseRequest>(FEndpoints::GetAssetEndpoint(PartnerDomain), UserSession.Token));
}

TSharedPtr<FBaseRequest> FRequestFactory::CreateColorRequest(const FString& AvatarId) const
{
	return CreateAuthorizedRequest(MakeShared<FBaseRequest>(FEndpoints::GetColorEndpoint(AvatarId), UserSession.Token));
}

TSharedPtr<FBaseRequest> FRequestFactory::CreateImageRequest(const FString& IconUrl) const
{
	return MakeShared<FBaseRequest>(IconUrl);
}

TSharedPtr<FBaseRequest> FRequestFactory::CreateAvatarModelRequest(const FString& AvatarId, bool bIsPreview) const
{
	return MakeShared<FBaseRequest>(FEndpoints::GetAvatarModelEndpoint(AvatarId, bIsPreview));
}

TSharedPtr<FBaseRequest> FRequestFactory::CreateAvatarMetadataRequest(const FString& AvatarId) const
{
	return MakeShared<FBaseRequest>(FEndpoints::GetAvatarMetadataEndpoint(AvatarId));
}

TSharedPtr<FBaseRequest> FRequestFactory::CreateAvatarCreateRequest(const FString& PayloadJson) const
{
	return CreateAuthorizedRequest(MakeShared<FBaseRequest>(FEndpoints::GetCreateEndpoint(), UserSession.Token, "POST", PayloadJson));
}

TSharedPtr<FBaseRequest> FRequestFactory::CreateUpdateAvatarRequest(const FString& AvatarId, const FString& PayloadJson) const
{
	return CreateAuthorizedRequest(MakeShared<FBaseRequest>(FEndpoints::GetUpdateAvatarEndpoint(AvatarId), UserSession.Token, "PATCH", PayloadJson));
}

TSharedPtr<FBaseRequest> FRequestFactory::CreateSaveAvatarRequest(const FString& AvatarId) const
{
	return CreateAuthorizedRequest(MakeShared<FBaseRequest>(FEndpoints::GetSaveAvatarEndpoint(AvatarId), UserSession.Token, "PUT"));
}

TSharedPtr<FBaseRequest> FRequestFactory::CreateDeleteAvatarRequest(const FString& AvatarId) const
{
	return CreateAuthorizedRequest(MakeShared<FBaseRequest>(FEndpoints::GetDeleteAvatarEndpoint(AvatarId), UserSession.Token, "DELETE"));
}

TSharedPtr<FBaseRequest> FRequestFactory::CreateAuthorizedRequest(TSharedPtr<FBaseRequest> MainRequest) const
{
	return MakeShared<FAuthorizedRequest>(MainRequest, CreateTokenRefreshRequest(), TokenRefreshedDelegate);
}
