// Copyright © 2023++ Ready Player Me


#include "RpmAuthManager.h"
#include "Extractors/UserSessionExtractor.h"
#include "Requests/RequestFactory.h"

void FRpmAuthManager::AuthAnonymous(TSharedPtr<FRequestFactory> Factory)
{
	RequestFactory = Factory;
	AuthRequest = RequestFactory->CreateAuthRequest();
	AuthRequest->GetCompleteCallback().BindSP(AsShared(), &FRpmAuthManager::DownloadCompleted);
	AuthRequest->Download();
}

FAuthCompleted& FRpmAuthManager::GetCompleteCallback()
{
	return OnAuthCompleted;
}

void FRpmAuthManager::DownloadCompleted(bool bSuccess)
{
	if (bSuccess)
	{
		UserSession = FUserSessionExtractor::ExtractUserSession(AuthRequest->GetContentAsString());
		if (UserSession)
		{
			(void)OnAuthCompleted.ExecuteIfBound(true);
			OnAuthCompleted.Unbind();
			return;
		}
	}
	(void)OnAuthCompleted.ExecuteIfBound(false);
	OnAuthCompleted.Unbind();
}

FRpmUserSession FRpmAuthManager::GetUserSession() const
{
	return *UserSession;
}
