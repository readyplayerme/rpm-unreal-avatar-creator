// Copyright © 2023++ Ready Player Me


#include "RequestFactory.h"

#include "CoreMinimal.h"
#include "Endpoints.h"
#include "Extractors/UserDataExtractor.h"

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
	return MakeShared<FBaseRequest>(FEndpoints::GetTokenRefreshEndpoint(PartnerDomain), "", "POST", FUserDataExtractor::MakeTokenRefreshPayload(UserData));
}

TSharedPtr<FBaseRequest> FRequestFactory::CreateAuthAnonymousRequest() const
{
	return MakeShared<FBaseRequest>(FEndpoints::GetAuthAnonymousEndpoint(PartnerDomain), "", "POST");
}

TSharedPtr<FBaseRequest> FRequestFactory::CreateAssetRequest() const
{
	return CreateAuthorizedRequest(MakeShared<FBaseRequest>(FEndpoints::GetAssetEndpoint(PartnerDomain), UserData.Token));
}

TSharedPtr<FBaseRequest> FRequestFactory::CreateColorRequest(const FString& AvatarId) const
{
	return CreateAuthorizedRequest(MakeShared<FBaseRequest>(FEndpoints::GetColorEndpoint(AvatarId), UserData.Token));
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
	return CreateAuthorizedRequest(MakeShared<FBaseRequest>(FEndpoints::GetCreateEndpoint(), UserData.Token, "POST", PayloadJson));
}

TSharedPtr<FBaseRequest> FRequestFactory::CreateUpdateAvatarRequest(const FString& AvatarId, const FString& PayloadJson) const
{
	return CreateAuthorizedRequest(MakeShared<FBaseRequest>(FEndpoints::GetUpdateAvatarEndpoint(AvatarId), UserData.Token, "PATCH", PayloadJson));
}

TSharedPtr<FBaseRequest> FRequestFactory::CreateSaveAvatarRequest(const FString& AvatarId) const
{
	return CreateAuthorizedRequest(MakeShared<FBaseRequest>(FEndpoints::GetSaveAvatarEndpoint(AvatarId), UserData.Token, "PUT"));
}

TSharedPtr<FBaseRequest> FRequestFactory::CreateDeleteAvatarRequest(const FString& AvatarId) const
{
	return CreateAuthorizedRequest(MakeShared<FBaseRequest>(FEndpoints::GetDeleteAvatarEndpoint(AvatarId), UserData.Token, "DELETE"));
}

TSharedPtr<FBaseRequest> FRequestFactory::CreateAuthorizedRequest(TSharedPtr<FBaseRequest> MainRequest) const
{
	return MakeShared<FAuthorizedRequest>(MainRequest, CreateTokenRefreshRequest(), TokenRefreshedDelegate);
}
