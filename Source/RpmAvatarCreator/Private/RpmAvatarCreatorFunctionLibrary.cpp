// Copyright © 2023++ Ready Player Me

#include "RpmAvatarCreatorFunctionLibrary.h"
#include "Serialization/BufferArchive.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Misc/Base64.h"
#include "ImageUtils.h"

FString URpmAvatarCreatorFunctionLibrary::GetBase64String(UTextureRenderTarget2D* TextureRenderTarget)
{
    FBufferArchive Buffer;
    if (FImageUtils::ExportRenderTarget2DAsPNG(TextureRenderTarget, Buffer))
    {
        return FBase64::Encode(const_cast<uint8*>(Buffer.GetData()), Buffer.Num());
    }
    return {};
}

