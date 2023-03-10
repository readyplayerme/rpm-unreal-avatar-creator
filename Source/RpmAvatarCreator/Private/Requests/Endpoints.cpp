// Copyright © 2023++ Ready Player Me


#include "Endpoints.h"

static const TCHAR* API_ENDPOINT = TEXT("https://{0}.readyplayer.me/api{1}");
static const TCHAR* AVATAR_API_V2_ENDPOINT = TEXT("https://api.readyplayer.me/v2/avatars");
static const TCHAR* REQUEST_OPTIMIZATION_PARAMS = TEXT("morphTargets=none&textureAtlas=none&textureSizeLimit=512&useHands=false");

FString FEndpoints::GetAuthEndpoint(const FString& Subdomain)
{
	return FString::Format(API_ENDPOINT, {Subdomain, TEXT("/users")});
}

FString FEndpoints::GetAssetEndpoint(const FString& Subdomain)
{
	return FString::Format(API_ENDPOINT, {Subdomain, TEXT("/assets")});
}

FString FEndpoints::GetAvatarPublicUrl(const FString& AvatarId)
{
	return FString::Format(TEXT("{0}/{1}.glb"), {AVATAR_API_V2_ENDPOINT, AvatarId});
}

FString FEndpoints::GetCreateEndpoint()
{
	return AVATAR_API_V2_ENDPOINT;
}

FString FEndpoints::GetAvatarPreviewEndpoint(const FString& AvatarId)
{
	return FString::Format(TEXT("{0}/{1}.glb?preview=true&{2}"), {AVATAR_API_V2_ENDPOINT, AvatarId, REQUEST_OPTIMIZATION_PARAMS});
}

FString FEndpoints::GetUpdateAvatarEndpoint(const FString& AvatarId)
{
	return FString::Format(TEXT("{0}/{1}?responseType=glb&{2}"), {AVATAR_API_V2_ENDPOINT, AvatarId, REQUEST_OPTIMIZATION_PARAMS});
}

FString FEndpoints::GetSaveAvatarEndpoint(const FString& AvatarId)
{
	return FString::Format(TEXT("{0}/{1}"), {AVATAR_API_V2_ENDPOINT, AvatarId});
}

FString FEndpoints::GetDeleteAvatarEndpoint(const FString& AvatarId)
{
	return FString::Format(TEXT("{0}/{1}"), {AVATAR_API_V2_ENDPOINT, AvatarId});
}
