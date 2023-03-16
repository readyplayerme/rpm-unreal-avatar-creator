// Copyright © 2023++ Ready Player Me

#pragma once

#include "BaseRequest.h"

DECLARE_DELEGATE_TwoParams(FTokenRefreshed, const FString&, const FString&);

//TODO: Make an IBaseRequest interface
class FAuthorizedRequest : public FBaseRequest
{
public:
	FAuthorizedRequest() = default;
	FAuthorizedRequest(TSharedPtr<FBaseRequest> MainRequest, const TSharedPtr<FBaseRequest> RefreshRequest, const FTokenRefreshed& TokenRefreshedDelegate);

	virtual void Download() override;
	virtual void CancelRequest() override;
	virtual FString GetContentAsString() const override;
	virtual const TArray<uint8>& GetContent() const override;
	virtual int32 GetResponseCode() const override;

protected:
	void MainRequestCompleted(bool bSuccess);
	void RefreshRequestCompleted(bool bSuccess);

	TSharedPtr<FBaseRequest> MainRequest;
	TSharedPtr<FBaseRequest> TokenRefreshRequest;
	FTokenRefreshed TokenRefreshedDelegate;
};
