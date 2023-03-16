// Copyright © 2023++ Ready Player Me


#include "BaseRequest.h"

#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"

FBaseRequest::FBaseRequest(const FString& Url, const FString& AuthToken, const FString& RequestVerb, const FString& Payload, float Timeout)
	: Url(Url)
	, AuthToken(AuthToken)
	, RequestVerb(RequestVerb)
	, Payload(Payload)
	, Timeout(Timeout)
{
}

void FBaseRequest::Download()
{
	DownloadRequest = FHttpModule::Get().CreateRequest();
	if (Timeout > 0.f)
	{
		DownloadRequest->SetTimeout(Timeout);
	}
	DownloadRequest->SetURL(Url);
	if (!AuthToken.IsEmpty())
	{
		const FString Authorization = FString::Printf(TEXT("Bearer %s"), *AuthToken);
		DownloadRequest->SetHeader(TEXT("Authorization"), Authorization);
	}
	if (!Payload.IsEmpty())
	{
		DownloadRequest->SetHeader("Content-Type", "application/json");
		DownloadRequest->SetContentAsString(Payload);
	}
	DownloadRequest->OnProcessRequestComplete().BindSP(AsShared(), &FBaseRequest::OnReceived);
	DownloadRequest->SetVerb(RequestVerb);
	DownloadRequest->ProcessRequest();
}

FFileDownloadCompleted& FBaseRequest::GetCompleteCallback()
{
	return OnDownloadCompleted;
}

void FBaseRequest::CancelRequest()
{
	if (DownloadRequest.IsValid() && (DownloadRequest->GetStatus() == EHttpRequestStatus::Type::Processing || DownloadRequest->GetStatus() == EHttpRequestStatus::Type::NotStarted))
	{
		DownloadRequest->CancelRequest();
	}
	OnDownloadCompleted.Unbind();
}

void FBaseRequest::OnReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
{
	const bool Success = bSuccess && Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode());
	(void)OnDownloadCompleted.ExecuteIfBound(Success);
	OnDownloadCompleted.Unbind();
}

FString FBaseRequest::GetContentAsString() const
{
	return DownloadRequest->GetResponse()->GetContentAsString();
}

const TArray<uint8>& FBaseRequest::GetContent() const
{
	return DownloadRequest->GetResponse()->GetContent();
}

int32 FBaseRequest::GetResponseCode() const
{
	return DownloadRequest->GetResponse()->GetResponseCode();
}

void FBaseRequest::SetAuthToken(const FString& Token)
{
	AuthToken = Token;
}
