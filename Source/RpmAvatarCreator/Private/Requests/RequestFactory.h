// Copyright © 2023++ Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "IBaseRequest.h"
#include "AuthorizedRequest.h"
#include "RpmAvatarCreatorTypes.h"

class FRequestFactory
{
public:
	FRequestFactory();

	void SetPartnerDomain(const FString& Domain);

	void SetAppId(const FString& Id);

	void SetUserData(const FRpmUserData& UserData);

	void SetTokenRefreshedDelegate(const FTokenRefreshed& TokenRefreshed);

	void SetSessionExpiredDelegate(const FSessionExpired& SessionExpired);

	void CancelRequests() const;

	TSharedPtr<IBaseRequest> CreateAuthStartRequest(const FString& PayloadJson) const;
	TSharedPtr<IBaseRequest> CreateConfirmCodeRequest(const FString& PayloadJson) const;
	TSharedPtr<IBaseRequest> CreateTokenRefreshRequest() const;
	TSharedPtr<IBaseRequest> CreateAuthAnonymousRequest() const;
	TSharedPtr<IBaseRequest> CreateAssetRequest(int32 Limit, int32 Page) const;
	TSharedPtr<IBaseRequest> CreateColorRequest(const FString& AvatarId) const;
	TSharedPtr<IBaseRequest> CreateAvatarModelRequest(const FString& AvatarId, bool bIsPreview) const;
	TSharedPtr<IBaseRequest> CreateAvatarMetadataRequest(const FString& AvatarId) const;
	TSharedPtr<IBaseRequest> CreateAvatarTemplatesRequest() const;
	TSharedPtr<IBaseRequest> CreateAvatarCreateRequest(const FString& PayloadJson, const FString& TemplateId) const;
	TSharedPtr<IBaseRequest> CreateImageRequest(const FString& IconUrl) const;
	TSharedPtr<IBaseRequest> CreateRenderRequest(const FString& AvatarId) const;
	TSharedPtr<IBaseRequest> CreateUserAvatarsRequest() const;
	TSharedPtr<IBaseRequest> CreateUpdateAvatarRequest(const FString& AvatarId, const FString& PayloadJson) const;
	TSharedPtr<IBaseRequest> CreateSaveAvatarRequest(const FString& AvatarId) const;
	TSharedPtr<IBaseRequest> CreateDeleteAvatarRequest(const FString& AvatarId, bool bIsDraft) const;

protected:
	TSharedPtr<IBaseRequest> CreateBaseRequest(const FString& Url, ERequestVerb RequestVerb = ERequestVerb::Get, const FString& Payload = "", float Timeout = -1.f) const;
	TSharedPtr<IBaseRequest> CreateAuthorizedRequest(const FString& Url, ERequestVerb RequestVerb = ERequestVerb::Get, const FString& Payload = "", float Timeout = -1.f) const;

private:
	FString PartnerDomain;
	FString AppId;
	FRpmUserData UserData;
	FTokenRefreshed TokenRefreshedDelegate;
	FSessionExpired SessionExpiredDelegate;
	TSharedRef<FCancellationDelegate> CancellationDelegate;
};
