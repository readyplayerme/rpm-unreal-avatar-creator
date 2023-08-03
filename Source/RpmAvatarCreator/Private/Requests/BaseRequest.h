// Copyright © 2023++ Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"
#include "IBaseRequest.h"

enum class ERequestVerb
{
	Get,
	Post,
	Put,
	Patch,
	Delete
};

class FBaseRequest : public IBaseRequest, public TSharedFromThis<FBaseRequest>
{
public:
	FBaseRequest() = default;
	FBaseRequest(const TSharedRef<FCancellationDelegate>& CancellationDelegate, const FString& Url, const FString& AuthToken = "", ERequestVerb RequestVerb = ERequestVerb::Get, const FString& Payload = "", float Timeout = -1.f);

	virtual void Download() override;
	virtual void CancelRequest() override;

	virtual FFileDownloadCompleted& GetCompleteCallback() override;

	virtual FString GetContentAsString() const override;
	virtual const TArray<uint8>& GetContent() const override;
	virtual int32 GetResponseCode() const override;
	virtual void SetAuthToken(const FString& Token) override;
	virtual void SetAppId(const FString& Id) override;

protected:
	virtual void OnReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);

	FFileDownloadCompleted OnDownloadCompleted;

	TSharedRef<FCancellationDelegate> CancellationDelegate;
	FDelegateHandle CancellationHandle;

	FString Url;
	FString AuthToken;
	FString AppId;
	ERequestVerb RequestVerb = ERequestVerb::Get;
	FString Payload = "";
	float Timeout = -1.f;

	TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> DownloadRequest;
};
