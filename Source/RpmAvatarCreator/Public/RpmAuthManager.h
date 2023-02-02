// Copyright © 2023++ Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "RpmAvatarCreatorTypes.h"

DECLARE_DELEGATE_OneParam(FAuthCompleted, bool /*bSuccess*/);

class RPMAVATARCREATOR_API FRpmAuthManager : public TSharedFromThis<FRpmAuthManager>
{
public:
	void AuthAnonymous(TSharedPtr<class FRequestFactory> RequestFactory);
	FRpmUserSession GetUserSession() const;

	FAuthCompleted& GetCompleteCallback();

private:
	void DownloadCompleted(bool bSuccess);

	FAuthCompleted OnAuthCompleted;

private:
	TSharedPtr<class FRequestFactory> RequestFactory;
	TOptional<FRpmUserSession> UserSession;
	TSharedPtr<class FBaseRequest> AuthRequest;
};
