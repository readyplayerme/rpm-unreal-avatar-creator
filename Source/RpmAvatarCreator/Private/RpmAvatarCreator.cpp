// Copyright © 2021++ Ready Player Me

#include "RpmAvatarCreator.h"
// #include "ReadyPlayerMeTypes.h"

// DEFINE_LOG_CATEGORY(LogRPMAvatarCreator);

#define LOCTEXT_NAMESPACE "FRpmAvatarCreatorModule"

void FRpmAvatarCreatorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FRpmAvatarCreatorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FRpmAvatarCreatorModule, RpmAvatarCreator)
