// Copyright © 2023++ Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"

DECLARE_DELEGATE_OneParam(FFileDownloadCompleted, bool /*bSuccess*/);

class FBaseRequest : public TSharedFromThis<FBaseRequest>
{
public:
	virtual ~FBaseRequest() = default;
	FBaseRequest() = default;
	FBaseRequest(const FString& Url, const FString& AuthToken = "", const FString& RequestVerb = "GET", const FString& Payload = "", float Timeout = -1.f);

	virtual void Download();
	virtual void CancelRequest();

	virtual FFileDownloadCompleted& GetCompleteCallback();

	virtual FString GetContentAsString() const;
	virtual const TArray<uint8>& GetContent() const;
	virtual int32 GetResponseCode() const;
	virtual void SetAuthToken(const FString& Token);

protected:
	virtual void OnReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);

	FFileDownloadCompleted OnDownloadCompleted;

	FString Url;
	FString AuthToken;
	FString RequestVerb = "GET";
	FString Payload = "";
	float Timeout = -1.f;

	TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> DownloadRequest;
};
