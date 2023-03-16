// Copyright © 2023++ Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "BaseRequest.h"
#include "AuthorizedRequest.h"
#include "RpmAvatarCreatorTypes.h"

class FRequestFactory : public FBaseRequest
{
public:
	void SetPartnerDomain(const FString& Domain);

	void SetUserSession(const FRpmUserSession& Session);

	void SetTokenRefreshedDelegate(const FTokenRefreshed& TokenRefreshed);

	TSharedPtr<FBaseRequest> CreateSendCodeRequest(const FString& PayloadJson) const;
	TSharedPtr<FBaseRequest> CreateConfirmCodeRequest(const FString& PayloadJson) const;
	TSharedPtr<FBaseRequest> CreateTokenRefreshRequest() const;
	TSharedPtr<FBaseRequest> CreateAuthAnonymousRequest() const;
	TSharedPtr<FBaseRequest> CreateAssetRequest() const;
	TSharedPtr<FBaseRequest> CreateColorRequest(const FString& AvatarId) const;
	TSharedPtr<FBaseRequest> CreateAvatarModelRequest(const FString& AvatarId, bool bIsPreview) const;
	TSharedPtr<FBaseRequest> CreateAvatarMetadataRequest(const FString& AvatarId) const;
	TSharedPtr<FBaseRequest> CreateAvatarCreateRequest(const FString& PayloadJson) const;
	TSharedPtr<FBaseRequest> CreateImageRequest(const FString& IconUrl) const;
	TSharedPtr<FBaseRequest> CreateUpdateAvatarRequest(const FString& AvatarId, const FString& PayloadJson) const;
	TSharedPtr<FBaseRequest> CreateSaveAvatarRequest(const FString& AvatarId) const;
	TSharedPtr<FBaseRequest> CreateDeleteAvatarRequest(const FString& AvatarId) const;

	TSharedPtr<FBaseRequest> CreateAuthorizedRequest(TSharedPtr<FBaseRequest> MainRequest) const;

private:
	FString PartnerDomain;
	FRpmUserSession UserSession;
	FTokenRefreshed TokenRefreshedDelegate;
};
