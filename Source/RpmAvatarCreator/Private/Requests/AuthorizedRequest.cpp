// Copyright © 2023++ Ready Player Me


#include "AuthorizedRequest.h"

#include "Interfaces/IHttpResponse.h"
#include "Extractors/UserDataExtractor.h"

FAuthorizedRequest::FAuthorizedRequest(TSharedPtr<IBaseRequest> MainRequest, const TSharedPtr<IBaseRequest> RefreshRequest, const FTokenRefreshed& TokenRefreshedDelegate)
			: MainRequest(MainRequest)
			, TokenRefreshRequest(RefreshRequest)
			, TokenRefreshedDelegate(TokenRefreshedDelegate)
{
}

void FAuthorizedRequest::MainRequestCompleted(bool bSuccess)
{
	if (!bSuccess && MainRequest->GetResponseCode() == EHttpResponseCodes::Denied && TokenRefreshedDelegate.IsBound())
	{
		TokenRefreshRequest->GetCompleteCallback().BindSP(AsShared(), &FAuthorizedRequest::RefreshRequestCompleted);
		TokenRefreshRequest->Download();
		return;
	}
	(void)OnDownloadCompleted.ExecuteIfBound(bSuccess);
	OnDownloadCompleted.Unbind();
	TokenRefreshRequest.Reset();
	TokenRefreshedDelegate.Unbind();
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
			TokenRefreshedDelegate.Unbind();
			MainRequest->SetAuthToken(UserData.Token);
			Download();
			return;
		}
	}
	(void)OnDownloadCompleted.ExecuteIfBound(false);
	OnDownloadCompleted.Unbind();
	TokenRefreshRequest.Reset();
	TokenRefreshedDelegate.Unbind();
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