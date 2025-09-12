#include "compressed/blurLogo.h"
#include "compressed/browser.h"
#include "compressed/close.h"
#include "compressed/minimize.h"
#include "compressed/mainLogo.h"
#include "compressed/whiteEffect.h"
#include "compressed/telegram.h"
#include "compressed/logo.h"
#include <d3d11.h>
#include "../include/other/stb_image.h"

namespace img {

    bool LoadImageFromCompressed(ID3D11ShaderResourceView** outTexture, const unsigned char* ImageData, static const unsigned int ImageSize, ID3D11Device* device) {
        int ImageWidth = 0;
        int ImageHeight = 0;
        unsigned char* Image = stbi_load_from_memory(ImageData, (int)ImageSize, &ImageWidth, &ImageHeight, nullptr, 4);
        if (!Image) {
            return false;
            MessageBoxA(nullptr, "!Image", "pustote", MB_OK);
        }
        D3D11_TEXTURE2D_DESC TextureDesc = {};
        TextureDesc.Width = ImageWidth;
        TextureDesc.Height = ImageHeight;
        TextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        TextureDesc.MipLevels = 1;
        TextureDesc.ArraySize = 1;
        TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        TextureDesc.Usage = D3D11_USAGE_DEFAULT;
        TextureDesc.SampleDesc.Count = 1;

        D3D11_SUBRESOURCE_DATA TextureInitData = {};
        TextureInitData.pSysMem = Image;
        TextureInitData.SysMemPitch = ImageWidth * 4;

        ID3D11Texture2D* Texture = nullptr;
        HRESULT hr;
        hr = device->CreateTexture2D(&TextureDesc, &TextureInitData, &Texture);
        if (FAILED(hr)) {
            return false;
            MessageBoxA(nullptr, "CreateTexture2D failed!", "pustote", MB_OK);
        }
        ID3D11ShaderResourceView* ShaderResourceView = nullptr;
        hr = device->CreateShaderResourceView(Texture, nullptr, &ShaderResourceView);
        Texture->Release();
        if (FAILED(hr)) {
            return false;
            MessageBoxA(nullptr, "CreateShaderResourceView failed!", "pustote", MB_OK);
        }
        *outTexture = ShaderResourceView;
    }

    const unsigned int mainLogo_size = logo_png_len;
    const unsigned char* mainLogo_data = logo_png;

    const unsigned int logo_size = logoW_png_len;
    const unsigned char* logo_data = logoW_png;

    const unsigned int logoBlur_size = logoBG_png_len;
    const unsigned char* logoBlur_data = logoBG_png;

    const unsigned int whiteEffect_size = background_png_len;
    const unsigned char* whiteEffect_data = background_png;

    const unsigned int telegram_size = telega_png_len;
    const unsigned char* telegram_data = telega_png;

    const unsigned int minimize_size = minim_png_len;
    const unsigned char* minimize_data = minim_png;

    const unsigned int close_size = close_png_len;
    const unsigned char* close_data = close_png;

    const unsigned int browser_size = browser2_png_len;
    const unsigned char* browser_data = browser2_png;
}


