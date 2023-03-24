// Copyright © 2023++ Ready Player Me


#include "RequestFactory.h"

#include "CoreMinimal.h"
#include "BaseRequest.h"
#include "Endpoints.h"
#include "Extractors/UserDataExtractor.h"

constexpr float IMAGE_REQUEST_TIMEOUT = 60.f;

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

TSharedPtr<IBaseRequest> FRequestFactory::CreateSendCodeRequest(const FString& PayloadJson) const
{
	return MakeShared<FBaseRequest>(FEndpoints::GetSendCodeEndpoint(PartnerDomain), "", "POST", PayloadJson);
}

TSharedPtr<IBaseRequest> FRequestFactory::CreateConfirmCodeRequest(const FString& PayloadJson) const
{
	return MakeShared<FBaseRequest>(FEndpoints::GetConfirmCodeEndpoint(PartnerDomain), "", "POST", PayloadJson);
}

TSharedPtr<IBaseRequest> FRequestFactory::CreateTokenRefreshRequest() const
{
	return MakeShared<FBaseRequest>(FEndpoints::GetTokenRefreshEndpoint(PartnerDomain), "", "POST", FUserDataExtractor::MakeTokenRefreshPayload(UserData));
}

TSharedPtr<IBaseRequest> FRequestFactory::CreateAuthAnonymousRequest() const
{
	return MakeShared<FBaseRequest>(FEndpoints::GetAuthAnonymousEndpoint(PartnerDomain), "", "POST");
}

TSharedPtr<IBaseRequest> FRequestFactory::CreateAssetRequest() const
{
	return CreateAuthorizedRequest(MakeShared<FBaseRequest>(FEndpoints::GetAssetEndpoint(PartnerDomain), UserData.Token));
}

TSharedPtr<IBaseRequest> FRequestFactory::CreateColorRequest(const FString& AvatarId) const
{
	return CreateAuthorizedRequest(MakeShared<FBaseRequest>(FEndpoints::GetColorEndpoint(AvatarId), UserData.Token));
}

TSharedPtr<IBaseRequest> FRequestFactory::CreateImageRequest(const FString& IconUrl) const
{
	return MakeShared<FBaseRequest>(IconUrl, "", "GET", "", IMAGE_REQUEST_TIMEOUT);
}

TSharedPtr<IBaseRequest> FRequestFactory::CreateAvatarModelRequest(const FString& AvatarId, bool bIsPreview) const
{
	return MakeShared<FBaseRequest>(FEndpoints::GetAvatarModelEndpoint(AvatarId, bIsPreview));
}

TSharedPtr<IBaseRequest> FRequestFactory::CreateAvatarMetadataRequest(const FString& AvatarId) const
{
	return MakeShared<FBaseRequest>(FEndpoints::GetAvatarMetadataEndpoint(AvatarId));
}

TSharedPtr<IBaseRequest> FRequestFactory::CreateAvatarCreateRequest(const FString& PayloadJson) const
{
	return CreateAuthorizedRequest(MakeShared<FBaseRequest>(FEndpoints::GetCreateEndpoint(), UserData.Token, "POST", PayloadJson));
}

TSharedPtr<IBaseRequest> FRequestFactory::CreateUpdateAvatarRequest(const FString& AvatarId, const FString& PayloadJson) const
{
	return CreateAuthorizedRequest(MakeShared<FBaseRequest>(FEndpoints::GetUpdateAvatarEndpoint(AvatarId), UserData.Token, "PATCH", PayloadJson));
}

TSharedPtr<IBaseRequest> FRequestFactory::CreateSaveAvatarRequest(const FString& AvatarId) const
{
	return CreateAuthorizedRequest(MakeShared<FBaseRequest>(FEndpoints::GetSaveAvatarEndpoint(AvatarId), UserData.Token, "PUT"));
}

TSharedPtr<IBaseRequest> FRequestFactory::CreateDeleteAvatarRequest(const FString& AvatarId) const
{
	return CreateAuthorizedRequest(MakeShared<FBaseRequest>(FEndpoints::GetDeleteAvatarEndpoint(AvatarId), UserData.Token, "DELETE"));
}

TSharedPtr<IBaseRequest> FRequestFactory::CreateAuthorizedRequest(TSharedPtr<IBaseRequest> MainRequest) const
{
	if (!UserData.bIsExistingUser)
	{
		return MainRequest;
	}
	return MakeShared<FAuthorizedRequest>(MainRequest, CreateTokenRefreshRequest(), TokenRefreshedDelegate);
}
