// Copyright © 2023++ Ready Player Me


#include "RpmAuthManager.h"
#include "Extractors/UserSessionExtractor.h"
#include "Requests/RequestFactory.h"
#include "RpmUserSessionSaveGame.h"
#include "Kismet/GameplayStatics.h"

static const FString USER_SESSION_SLOT = "RpmUserSessionSlot";

FRpmAuthManager::FRpmAuthManager(TSharedPtr<FRequestFactory> RequestFactory)
	: RequestFactory(RequestFactory)
{
	LoadUserSession();
}

void FRpmAuthManager::BindTokenRefreshDelegate()
{
	OnTokenRefreshed.BindSP(AsShared(), &FRpmAuthManager::TokenRefreshed);
	RequestFactory->SetTokenRefreshedDelegate(OnTokenRefreshed);
}

void FRpmAuthManager::Logout()
{
	UGameplayStatics::DeleteGameInSlot(USER_SESSION_SLOT, 0);
	UserSession.Reset();
}

void FRpmAuthManager::SendActivationCode(const FString& Email, const FAuthenticationCompleted& Completed, const FAvatarCreatorFailed& Failed)
{
	OnAuthenticationCompleted = Completed;
	OnAvatarCreatorFailed = Failed;

	AuthRequest = RequestFactory->CreateSendCodeRequest(FUserSessionExtractor::MakeSendCodePayload(Email));
	AuthRequest->GetCompleteCallback().BindSP(AsShared(), &FRpmAuthManager::SendActivationCodeCompleted);
	AuthRequest->Download();
}

void FRpmAuthManager::ConfirmActivationCode(const FString& Code, const FAuthenticationCompleted& Completed, const FAvatarCreatorFailed& Failed)
{
	OnAuthenticationCompleted = Completed;
	OnAvatarCreatorFailed = Failed;

	AuthRequest = RequestFactory->CreateConfirmCodeRequest(FUserSessionExtractor::MakeConfirmCodePayload(Code));
	AuthRequest->GetCompleteCallback().BindSP(AsShared(), &FRpmAuthManager::ConfirmActivationCodeCompleted);
	AuthRequest->Download();
}

void FRpmAuthManager::AuthAnonymous(const FAuthenticationCompleted& Completed, const FAvatarCreatorFailed& Failed)
{
	OnAuthenticationCompleted = Completed;
	OnAvatarCreatorFailed = Failed;

	AuthRequest = RequestFactory->CreateAuthAnonymousRequest();
	AuthRequest->GetCompleteCallback().BindSP(AsShared(), &FRpmAuthManager::AuthAnonymousCompleted);
	AuthRequest->Download();
}

void FRpmAuthManager::ConfirmActivationCodeCompleted(bool bSuccess)
{
	if (!bSuccess)
	{
		(void)OnAvatarCreatorFailed.ExecuteIfBound(ERpmAvatarCreatorError::AuthenticationFailure);
		return;
	}
	UserData = FUserSessionExtractor::ExtractUserData(AuthRequest->GetContentAsString());
	UserSession = FRpmUserSession{false, UserData->Id, UserData->Token, UserData->RefreshToken, UserData->Partner};
	SaveUserSession();
	(void)OnAuthenticationCompleted.ExecuteIfBound();
}

void FRpmAuthManager::SendActivationCodeCompleted(bool bSuccess)
{
	if (!bSuccess)
	{
		(void)OnAvatarCreatorFailed.ExecuteIfBound(ERpmAvatarCreatorError::AuthenticationFailure);
		return;
	}
	(void)OnAuthenticationCompleted.ExecuteIfBound();
}

void FRpmAuthManager::AuthAnonymousCompleted(bool bSuccess)
{
	if (bSuccess)
	{
		UserSession = FUserSessionExtractor::ExtractUserSession(AuthRequest->GetContentAsString());
	}
	bSuccess &= UserSession.IsSet();

	if (!bSuccess)
	{
		(void)OnAvatarCreatorFailed.ExecuteIfBound(ERpmAvatarCreatorError::AuthenticationFailure);
		return;
	}
	RequestFactory->SetUserSession(*UserSession);
	SaveUserSession();
	(void)OnAuthenticationCompleted.ExecuteIfBound();
}

void FRpmAuthManager::SaveUserSession() const
{
	URpmUserSessionSaveGame* SaveGame = Cast<URpmUserSessionSaveGame>(UGameplayStatics::CreateSaveGameObject(URpmUserSessionSaveGame::StaticClass()));
	if (SaveGame)
	{
		SaveGame->bIsAnonymous = UserSession->bIsAnonymous;
		SaveGame->Id = UserSession->Id;
		SaveGame->Token = UserSession->Token;
		SaveGame->RefreshToken = UserSession->RefreshToken;
		SaveGame->Partner = UserSession->Partner;
		if (!UGameplayStatics::SaveGameToSlot(SaveGame, USER_SESSION_SLOT, 0))
		{
			UE_LOG(LogRpmAvatarCreator, Warning, TEXT("Failed to save user session"));
		}
	}
}

void FRpmAuthManager::LoadUserSession()
{
	const URpmUserSessionSaveGame* SaveGame = Cast<URpmUserSessionSaveGame>(UGameplayStatics::LoadGameFromSlot(USER_SESSION_SLOT, 0));
	if (SaveGame)
	{
		UserSession = FRpmUserSession{SaveGame->bIsAnonymous, SaveGame->Id, SaveGame->Token, SaveGame->RefreshToken, SaveGame->Partner};
		RequestFactory->SetUserSession(*UserSession);
	}
}

void FRpmAuthManager::TokenRefreshed(const FString& Token, const FString& RefreshToken)
{
	UserSession->Token = Token;
	UserSession->RefreshToken = RefreshToken;
	RequestFactory->SetUserSession(*UserSession);
	SaveUserSession();
}

TOptional<FRpmUserSession> FRpmAuthManager::GetUserSession() const
{
	return UserSession;
}
