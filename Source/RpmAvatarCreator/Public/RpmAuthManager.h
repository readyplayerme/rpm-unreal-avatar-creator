// Copyright © 2023++ Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "RpmAvatarCreatorTypes.h"
#include "Requests/AuthorizedRequest.h"

class RPMAVATARCREATOR_API FRpmAuthManager : public TSharedFromThis<FRpmAuthManager>
{
public:
	FRpmAuthManager(TSharedPtr<class FRequestFactory> RequestFactory);
	void BindTokenRefreshDelegate();
	void SendActivationCode(const FString& Email, const FAuthenticationCompleted& Completed, const FAvatarCreatorFailed& Failed);
	void ConfirmActivationCode(const FString& Code, const FAuthenticationCompleted& Completed, const FAvatarCreatorFailed& Failed);
	void AuthAnonymous(const FAuthenticationCompleted& Completed, const FAvatarCreatorFailed& Failed);
	void Logout();
	void LoadUserData();
	TOptional<FRpmUserData> GetUserData() const;

private:
	void AuthAnonymousCompleted(bool bSuccess);
	void SendActivationCodeCompleted(bool bSuccess);
	void ConfirmActivationCodeCompleted(bool bSuccess);
	void TokenRefreshed(const FString& Token, const FString& RefreshToken);

	void SaveUserData() const;
	
	TSharedPtr<class FRequestFactory> RequestFactory;
	TOptional<FRpmUserData> UserData;
	TSharedPtr<class FBaseRequest> AuthRequest;

	FAuthenticationCompleted OnAuthenticationCompleted;
	FAvatarCreatorFailed OnAvatarCreatorFailed;

	FTokenRefreshed OnTokenRefreshed;
};
