#include "compressed/blurLogo.h"
#include "compressed/browser.h"
#include "compressed/close.h"
#include "compressed/minimize.h"
#include "compressed/mainLogo.h"
#include "compressed/whiteEffect.h"
#include "compressed/telegram.h"
#include "compressed/logo.h"

namespace img {

    bool Load();

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


