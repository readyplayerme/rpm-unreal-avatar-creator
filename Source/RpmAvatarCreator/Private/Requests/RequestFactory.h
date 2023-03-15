// Copyright © 2023++ Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "BaseRequest.h"

class FRequestFactory : public FBaseRequest
{
public:
	FRequestFactory(const FString& Domain);

	void SetAuthToken(const FString& Token);

	TSharedPtr<FBaseRequest> CreateAuthRequest() const;
	TSharedPtr<FBaseRequest> CreateAssetRequest() const;
	TSharedPtr<FBaseRequest> CreateColorRequest(const FString& AvatarId) const;
	TSharedPtr<FBaseRequest> CreateAvatarModelRequest(const FString& AvatarId, bool bIsPreview) const;
	TSharedPtr<FBaseRequest> CreateAvatarMetadataRequest(const FString& AvatarId) const;
	TSharedPtr<FBaseRequest> CreateAvatarCreateRequest(const FString& PayloadJson) const;
	TSharedPtr<FBaseRequest> CreateImageRequest(const FString& IconUrl) const;
	TSharedPtr<FBaseRequest> CreateUpdateAvatarRequest(const FString& AvatarId, const FString& PayloadJson) const;
	TSharedPtr<FBaseRequest> CreateSaveAvatarRequest(const FString& AvatarId) const;
	TSharedPtr<FBaseRequest> CreateDeleteAvatarRequest(const FString& AvatarId) const;

private:
	const FString PartnerDomain;
	FString AuthToken;
};
