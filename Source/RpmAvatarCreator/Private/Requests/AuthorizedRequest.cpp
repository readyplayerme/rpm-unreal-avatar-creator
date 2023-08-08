// Copyright © 2023++ Ready Player Me


#include "AuthorizedRequest.h"

#include "Interfaces/IHttpResponse.h"
#include "Extractors/UserDataExtractor.h"

FAuthorizedRequest::FAuthorizedRequest(TSharedPtr<IBaseRequest> MainRequest, const TSharedPtr<IBaseRequest> RefreshRequest, const FTokenRefreshed& TokenRefreshedDelegate, const FSessionExpired& SessionExpired)
			: MainRequest(MainRequest)
			, TokenRefreshRequest(RefreshRequest)
			, TokenRefreshedDelegate(TokenRefreshedDelegate)
			, SessionExpiredDelegate(SessionExpired)
{
}

void FAuthorizedRequest::MainRequestCompleted(bool bSuccess)
{
	if (!bSuccess && MainRequest->GetResponseCode() == EHttpResponseCodes::Denied && TokenRefreshedDelegate.IsBound())
	{
		if (TokenRefreshRequest.IsValid())
		{
			TokenRefreshRequest->GetCompleteCallback().BindSP(AsShared(), &FAuthorizedRequest::RefreshRequestCompleted);
			TokenRefreshRequest->Download();
		}
		else
		{
			(void)SessionExpiredDelegate.ExecuteIfBound();
			MainRequest.Reset();
			OnDownloadCompleted.Unbind();
		}
	}
	else
	{
		(void)OnDownloadCompleted.ExecuteIfBound(bSuccess);
		OnDownloadCompleted.Unbind();
		TokenRefreshRequest.Reset();
	}
}

void FAuthorizedRequest::RefreshRequestCompleted(bool bSuccess)
{
	if (bSuccess)
	{
		const auto UserData = FUserDataExtractor::ExtractRefreshedUserSession(TokenRefreshRequest->GetContentAsString());
		if (UserData.bIsAuthenticated)
		{
			(void)TokenRefreshedDelegate.ExecuteIfBound(UserData.Token, UserData.RefreshToken);
			TokenRefreshRequest.Reset();
			MainRequest->SetAuthToken(UserData.Token);
			Download();
			return;
		}
	}
	else if (TokenRefreshRequest->GetResponseCode() == EHttpResponseCodes::Denied && SessionExpiredDelegate.IsBound())
	{
		(void)SessionExpiredDelegate.ExecuteIfBound();
		MainRequest.Reset();
		TokenRefreshRequest.Reset();
		OnDownloadCompleted.Unbind();
		return;
	}
	(void)OnDownloadCompleted.ExecuteIfBound(false);
	OnDownloadCompleted.Unbind();
	TokenRefreshRequest.Reset();
}

FFileDownloadCompleted& FAuthorizedRequest::GetCompleteCallback()
{
	return OnDownloadCompleted;
}

void FAuthorizedRequest::Download()
{
	MainRequest->GetCompleteCallback().BindSP(AsShared(), &FAuthorizedRequest::MainRequestCompleted);
	MainRequest->Download();
}

void FAuthorizedRequest::CancelRequest()
{
	MainRequest->CancelRequest();
	TokenRefreshRequest->CancelRequest();
}

FString FAuthorizedRequest::GetContentAsString() const
{
	return MainRequest->GetContentAsString();
}

const TArray<uint8>& FAuthorizedRequest::GetContent() const
{
	return MainRequest->GetContent();
}

int32 FAuthorizedRequest::GetResponseCode() const
{
	return MainRequest->GetResponseCode();
}