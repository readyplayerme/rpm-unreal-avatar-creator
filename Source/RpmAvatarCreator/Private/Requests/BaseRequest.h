// Copyright © 2021++ Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Launch/Resources/Version.h"
#include "Interfaces/IHttpRequest.h"

DECLARE_DELEGATE_OneParam(FFileDownloadCompleted, bool /*bSuccess*/);

class FBaseRequest : public TSharedFromThis<FBaseRequest>
{
public:
	FBaseRequest() = default;
	FBaseRequest(const FString& Url, const FString& AuthToken = "", const FString& RequestVerb = "GET", const FString& Payload = "", float Timeout = -1.f)
		: Url(Url)
		, AuthToken(AuthToken)
		, RequestVerb(RequestVerb)
		, Payload(Payload)
		, Timeout(Timeout)
	{
	}

	void Download();
	void CancelRequest();

	FFileDownloadCompleted& GetCompleteCallback();

	FString GetContentAsString() const;
	const TArray<uint8>& GetContent() const;

private:
	void OnReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);

	FFileDownloadCompleted OnDownloadCompleted;

protected:
	FString Url;
	FString AuthToken;
	FString RequestVerb = "GET";
	FString Payload = "";
	float Timeout = -1.f;

	TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> DownloadRequest;
};
