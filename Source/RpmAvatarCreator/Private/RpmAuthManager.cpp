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

FBaseRequestCompleted& FRpmAuthManager::GetCompleteCallback()
{
	return OnAuthCompleted;
}

void FRpmAuthManager::DownloadCompleted(bool bSuccess)
{
	if (bSuccess)
	{
		UserSession = FUserSessionExtractor::ExtractUserSession(AuthRequest->GetContentAsString());
	}
	(void)OnAuthCompleted.ExecuteIfBound(bSuccess && UserSession.IsSet());
	OnAuthCompleted.Unbind();
}

TOptional<FRpmUserSession> FRpmAuthManager::GetUserSession() const
{
	return UserSession;
}
