// Copyright © 2023++ Ready Player Me


#include "RequestFactory.h"

#include "CoreMinimal.h"
#include "Endpoints.h"

FRequestFactory::FRequestFactory(const FString& Domain)
	: PartnerDomain(Domain)
{
}

void FRequestFactory::SetAuthToken(const FString& Token)
{
	AuthToken = Token;
}

TSharedPtr<FBaseRequest> FRequestFactory::CreateAuthRequest() const
{
	return MakeShared<FBaseRequest>(FEndpoints::GetAuthEndpoint(PartnerDomain), "", "POST");
}

TSharedPtr<FBaseRequest> FRequestFactory::CreateAssetRequest() const
{
	return MakeShared<FBaseRequest>(FEndpoints::GetAssetEndpoint(PartnerDomain), AuthToken);
}

TSharedPtr<FBaseRequest> FRequestFactory::CreateImageRequest(const FString& IconUrl) const
{
	return MakeShared<FBaseRequest>(IconUrl);
}

TSharedPtr<FBaseRequest> FRequestFactory::CreateAvatarPreviewRequest(const FString& AvatarId) const
{
	return MakeShared<FBaseRequest>(FEndpoints::GetAvatarPreviewEndpoint(AvatarId));
}

TSharedPtr<FBaseRequest> FRequestFactory::CreateAvatarMetadataRequest(const FString& AvatarId) const
{
	return MakeShared<FBaseRequest>(FEndpoints::GetAvatarMetadataEndpoint(AvatarId));
}

TSharedPtr<FBaseRequest> FRequestFactory::CreateAvatarCreateRequest(const FString& PayloadJson) const
{
	return MakeShared<FBaseRequest>(FEndpoints::GetCreateEndpoint(), AuthToken, "POST", PayloadJson);
}

TSharedPtr<FBaseRequest> FRequestFactory::CreateUpdateAvatarRequest(const FString& AvatarId, const FString& PayloadJson) const
{
	return MakeShared<FBaseRequest>(FEndpoints::GetUpdateAvatarEndpoint(AvatarId), AuthToken, "PATCH", PayloadJson);
}

TSharedPtr<FBaseRequest> FRequestFactory::CreateSaveAvatarRequest(const FString& AvatarId) const
{
	return MakeShared<FBaseRequest>(FEndpoints::GetSaveAvatarEndpoint(AvatarId), AuthToken, "PUT");
}

TSharedPtr<FBaseRequest> FRequestFactory::CreateDeleteAvatarRequest(const FString& AvatarId) const
{
	return MakeShared<FBaseRequest>(FEndpoints::GetDeleteAvatarEndpoint(AvatarId), AuthToken, "DELETE");
}

