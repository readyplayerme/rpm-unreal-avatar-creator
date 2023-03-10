// Copyright © 2023++ Ready Player Me

#pragma once

#include "CoreMinimal.h"

class FEndpoints
{
public:
	static FString GetAuthEndpoint(const FString& Subdomain);

	static FString GetAssetEndpoint(const FString& Subdomain);
	
	static FString GetColorEndpoint(const FString& AvatarId);

	static FString GetAvatarPublicUrl(const FString& AvatarId);

	static FString GetAvatarMetadataEndpoint(const FString& AvatarId);

	static FString GetCreateEndpoint();

	static FString GetAvatarPreviewEndpoint(const FString& AvatarId);

	static FString GetUpdateAvatarEndpoint(const FString& AvatarId);

	static FString GetSaveAvatarEndpoint(const FString& AvatarId);

	static FString GetDeleteAvatarEndpoint(const FString& AvatarId);
};