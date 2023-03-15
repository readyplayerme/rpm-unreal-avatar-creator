// Copyright © 2023++ Ready Player Me

#pragma once

#include "CoreMinimal.h"
#include "RpmAvatarCreatorTypes.h"

class RPMAVATARCREATOR_API FRpmAuthManager : public TSharedFromThis<FRpmAuthManager>
{
public:
	FRpmAuthManager(TSharedPtr<class FRequestFactory> RequestFactory);
	void SendActivationCode(const FString& Email, const FAuthenticationCompleted& Completed, const FAvatarCreatorFailed& Failed);
	void ConfirmActivationCode(const FString& Code, const FAuthenticationCompleted& Completed, const FAvatarCreatorFailed& Failed);
	void AuthAnonymous(const FAuthenticationCompleted& Completed, const FAvatarCreatorFailed& Failed);
	void Logout();
	TOptional<FRpmUserSession> GetUserSession() const;

private:
	void AuthAnonymousCompleted(bool bSuccess);
	void SendActivationCodeCompleted(bool bSuccess);
	void ConfirmActivationCodeCompleted(bool bSuccess);

	void SaveUserSession() const;
	void LoadUserSession();
	
	TSharedPtr<class FRequestFactory> RequestFactory;
	TOptional<FRpmUserData> UserData;
	TOptional<FRpmUserSession> UserSession;
	TSharedPtr<class FBaseRequest> AuthRequest;

	FAuthenticationCompleted OnAuthenticationCompleted;
	FAvatarCreatorFailed OnAvatarCreatorFailed;
};
