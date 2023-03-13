// Copyright © 2023++ Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "RpmAvatarCreatorTypes.h"

class RPMAVATARCREATOR_API FRpmAuthManager : public TSharedFromThis<FRpmAuthManager>
{
public:
	void AuthAnonymous(TSharedPtr<class FRequestFactory> RequestFactory);
	TOptional<FRpmUserSession> GetUserSession() const;

	FBaseRequestCompleted& GetCompleteCallback();

private:
	void DownloadCompleted(bool bSuccess);

	FBaseRequestCompleted OnAuthCompleted;

private:
	TSharedPtr<class FRequestFactory> RequestFactory;
	TOptional<FRpmUserSession> UserSession;
	TSharedPtr<class FBaseRequest> AuthRequest;
};
