#include <iostream>
#include <vector>
#include <cmath>
#include <windows.h>
#include <memory> // Para std::unique_ptr
#include <mutex>  // Para proteção de threads
#

bool useIstrigFilter = false;
class PixelSearcher {
private:
    static constexpr int size = 60;
    int monitor = 0;

public:

    void RGBtoHSV(int r, int g, int b, float& h, float& s, float& v) {
        double red = r / 255.0;
        double green = g / 255.0;
        double blue = b / 255.0;

        double max_value = max(max(red, green), blue);
        double min_value = min(min(red, green), blue);
        double delta = max_value - min_value;

        if (max_value == 0) {
            s = 0;
        }
        else {
            s = delta / max_value;
        }

        if (max_value == min_value) {
            h = 0;
        }
        else {
            if (max_value == red) {
                h = (green - blue) / delta + (green < blue ? 6 : 0);
            }
            else if (max_value == green) {
                h = (blue - red) / delta + 2;
            }
            else {
                h = (red - green) / delta + 4;
            }
            h /= 6;
        }

        v = max_value;
    }
    bool IsPurpleColor(int red, int green, int blue) {
        float h, s, v;
        RGBtoHSV(red, green, blue, h, s, v);

        int hue_int = static_cast<int>(h * 360);
        int saturation_int = static_cast<int>(s * 100);
        int value_int = static_cast<int>(v * 100);
        if (useIstrigFilter) {
            //roxo
            switch (cfg::color_mode) {
            case 0:
                //roxo
                if (green >= 170) {
                    return false;
                }

                if (green >= 120) {
                    return abs(red - blue) <= 8 &&
                        red - green >= 50 &&
                        blue - green >= 50 &&
                        red >= 105 &&
                        blue >= 105;
                }

                return abs(red - blue) <= 13 &&
                    red - green >= 60 &&
                    blue - green >= 60 &&
                    red >= 110 &&
                    blue >= 100;
                break;
            case 1:
                //roxo
                if (green >= 170) {
                    return false;
                }

                if (green >= 120) {
                    return abs(red - blue) <= 8 &&
                        red - green >= 50 &&
                        blue - green >= 50 &&
                        red >= 105 &&
                        blue >= 105;
                }

                return abs(red - blue) <= 13 &&
                    red - green >= 60 &&
                    blue - green >= 60 &&
                    red >= 110 &&
                    blue >= 100;
                break;
            case 2:
                if (red >= 168 && red <= 255 &&
                    green >= 168 && green <= 255 &&
                    blue >= 0 && blue <= 110 &&
                    hue_int >= 55 && hue_int <= 65 &&
                    saturation_int >= 5 && saturation_int <= 100 &&
                    value_int >= 70 && value_int <= 100) {
                    return true;
                }
                break;
            case 3:
                if (red >= 225 && red <= 255 &&
                    green >= 45 && green <= 136 &&
                    blue >= 45 && blue <= 136 &&
                    hue_int >= 0 && hue_int <= 1 &&
                    saturation_int >= 37 && saturation_int <= 80 &&
                    value_int >= 88 && value_int <= 100) {
                    return true;
                }
                break;
            }
        }
        else {
            if (red >= cfg::menorRGB[0] && red <= cfg::maiorRGB[0] &&
                green >= cfg::menorRGB[1] && green <= cfg::maiorRGB[1] &&
                blue >= cfg::menorRGB[2] && blue <= cfg::maiorRGB[2] &&
                hue_int >= cfg::menorHSV[0] && hue_int <= cfg::maiorHSV[0] &&
                saturation_int >= cfg::menorHSV[1] && saturation_int <= cfg::maiorHSV[1] &&
                value_int >= cfg::menorHSV[2] && value_int <= cfg::maiorHSV[2]) {
                return true;
            }
        }


        return false;
    }

};
void ProcessImage(BYTE* screenData, int w, int h) {
    PixelSearcher PS;
    Vector2 middle_screen(Width / 2, Height / 2);

    struct PurplePixel {
        int x;
        int y;
    };

    std::vector<PurplePixel> purplePixels; // Lista de pixels roxos
    int closestX = 0;
    int closestY = 0;
    const int MAX_INT_VALUE = 2147483647; // Valor máximo para um int de 32 bits
    int closestDistanceSquared = MAX_INT_VALUE; // Inicializa com um valor grande

    for (int j = 0; j < h; ++j) {
        for (int i = 0; i < w * 4; i += 4) {
            int red = screenData[i + (j * w * 4) + 2];
            int green = screenData[i + (j * w * 4) + 1];
            int blue = screenData[i + (j * w * 4) + 0];

            if (PS.IsPurpleColor(red, green, blue)) {
                oX = i / 4 + (middle_screen.x - (w / 2));
                oY = j + (middle_screen.y - (h / 2));
                silent_x = (i / 4) - (w / 2);
                silent_y = j - (h / 2);
                PurplePixel pixel;
                int dx = (i / 4) - (w / 2) + cfg::head_offset_x;
                int dy = j - (h / 2) + cfg::head_offset_y;
                int distanceSquared = dx * dx + dy * dy;
                if (distanceSquared < closestDistanceSquared) {
                    closestDistanceSquared = distanceSquared;
                    pixel.x = dx;
                    pixel.y = dy;
                }
                purplePixels.push_back(pixel);
            }
        }
    }

    if (!purplePixels.empty()) {
        std::sort(purplePixels.begin(), purplePixels.end(), [](const PurplePixel& a, const PurplePixel& b) {
            return a.x < b.x;
            });

        // Encontre o pixel mais próximo em termos de coordenada X
        // Encontre o pixel roxo mais alto (comece do topo e vá até o final horizontalmente)
        int highestY = h; // Inicialize com o valor máximo (altura da tela)
        for (const auto& pixel : purplePixels) {
            if (pixel.y < highestY) {
                highestY = pixel.y;
                closestX = pixel.x;
                closestY = pixel.y;
            }
        }
    }

    // Defina aim_x e aim_y com as coordenadas do pixel selecionado
    aim_x = closestX;
    aim_y = closestY;
}

void Triggerbot() {
    int pixel_sens = 90.9419;
    int pixelcolorcustom;
    if (cfg::color_mode == 0 || cfg::color_mode == 1) {
        pixelcolorcustom = RGB(235, 105, 254);
    }
    else if (cfg::color_mode == 2) {
        pixelcolorcustom = RGB(255, 255, 85);
    }
    else if (cfg::color_mode == 3) {
        pixelcolorcustom = RGB(254, 99, 106);
    }

    COLORREF pixel_color = pixelcolorcustom;
    int leftbound = Width / 2 - cfg::triggerbot_fovX;
    int rightbound = Width / 2 + cfg::triggerbot_fovX;
    int topbound = Height / 2 - cfg::triggerbot_fovY;
    int bottombound = Height / 2 + cfg::triggerbot_fovY;

        if ((GetAsyncKeyState)(cfg::triggerbot_key) && cfg::triggerbot_ativo) {
            HDC hdcScreen = GetDC(NULL);
            HDC hdcMem = CreateCompatibleDC(hdcScreen);

            int width = rightbound - leftbound;
            int height = bottombound - topbound;

            BITMAPINFOHEADER bmpInfo = { 0 };
            bmpInfo.biSize = sizeof(BITMAPINFOHEADER);
            bmpInfo.biPlanes = 1;
            bmpInfo.biBitCount = 32;
            bmpInfo.biWidth = width;
            bmpInfo.biHeight = -height;
            bmpInfo.biCompression = BI_RGB;
            LPBYTE lpBits = NULL;
            HBITMAP hBitmap = CreateDIBSection(hdcScreen, (BITMAPINFO*)&bmpInfo, DIB_RGB_COLORS, (LPVOID*)&lpBits, NULL, 0);
            if (hBitmap != NULL) {
                SelectObject(hdcMem, hBitmap);
                BitBlt(hdcMem, 0, 0, width, height, hdcScreen, leftbound, topbound, SRCCOPY);
                for (int i = 0; i < width * height; i++) {
                    int r = lpBits[i * 4 + 2];
                    int g = lpBits[i * 4 + 1];
                    int b = lpBits[i * 4];
                    if (abs(r - GetRValue(pixel_color)) < pixel_sens &&
                        abs(g - GetGValue(pixel_color)) < pixel_sens &&
                        abs(b - GetBValue(pixel_color)) < pixel_sens)
                    {
                        if (!((GetAsyncKeyState)(VK_LBUTTON))) {
                            XD.MouseEvent(0, 0, Driver::LeftButtonDown);
                            std::this_thread::sleep_for(std::chrono::milliseconds(1));
                            XD.MouseEvent(0, 0, Driver::LeftButtonUp);
                            std::this_thread::sleep_for(std::chrono::milliseconds(cfg::triggerbot_delay));
                        }
                        break;
                    }
                }
                DeleteObject(hBitmap);
            }
            DeleteDC(hdcMem);
            ReleaseDC(NULL, hdcScreen);
        }
}


void CaptureScreen() {

    bool moved_mouse = false;
    bool skipNextFrames = false;
    Vector2 middle_screen(Width / 2, Height / 2);

    while (true) {
        int w, h;
        {
            std::lock_guard<std::mutex> lock(fovMutex);
            w = currentFOV;
            h = currentFOV;
        }
        HDC hScreen = GetDC(NULL);
        HDC hDC = CreateCompatibleDC(hScreen);
        HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, w, h);
        BYTE* screenData = (BYTE*)malloc(Width * Height);

        if (!skipNextFrames) {
            if (moved_mouse) {
                Aimbot(aim_x, aim_y, cfg::aimbot_smooth);
                Magnet(aim_x, aim_y, cfg::aimassist_smooth);
                moved_mouse = false;
            }
            SelectObject(hDC, hBitmap);
            BitBlt(hDC, 0, 0, w, h, hScreen, middle_screen.x - (w / 2), middle_screen.y - (h / 2), SRCCOPY);

            BITMAPINFO  bmi = { 0 };
            bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
            bmi.bmiHeader.biPlanes = 1;
            bmi.bmiHeader.biBitCount = 32;
            bmi.bmiHeader.biWidth = w;
            bmi.bmiHeader.biHeight = -h;
            bmi.bmiHeader.biCompression = BI_RGB;


            GetDIBits(hDC, hBitmap, 0, h, screenData, &bmi, DIB_RGB_COLORS);

            ProcessImage(screenData, w, h);
            Triggerbot();
            DeleteObject(hBitmap);
            DeleteDC(hDC);
            ReleaseDC(NULL, hScreen);
            free(screenData);

        }

        std::this_thread::sleep_for(std::chrono::milliseconds(cfg::sleep));
        skipNextFrames = false;
        moved_mouse = true;
    }
}
