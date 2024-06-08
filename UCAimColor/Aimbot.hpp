#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <random>
#include <windows.h>
#include <chrono>
#include "Stopwatch.hpp"
#include <memory>
#include <mutex> 
#include "Driver.hpp"
Driver::Comms XD;
std::mutex fovMutex; 
int currentFOV = 0; 
bool key_ativa = false;
#define M_PI 3.141592653589793238462643383279502884197169
int aim_x = 0;
int aim_y = 0;
int Width;
int Height;
int oX, oY;
int silent_x, silent_y;
int fov = 0;
void add_overflow(double Input, double& Overflow)
{
    Overflow = std::modf(Input, &Input) + Overflow;

    if (Overflow > 1.0)
    {
        double Integral{ 0.0 };
        Overflow = std::modf(Overflow, &Integral);
        Input += Integral;
    }
}
static bool InsideCircleTrigger(float centerX, float centerY, float fovX, float fovY, float x, float y)
{
    float minX = centerX - fovX / 2;
    float maxX = centerX + fovX / 2;
    float minY = centerY - fovY / 2;
    float maxY = centerY + fovY / 2;
    return x >= minX && x <= maxX && y >= minY && y <= maxY;
}
float DistanceBetweenCross(float X, float Y)
{
    float ydist = (Y - (Height / 2));
    float xdist = (X - (Width / 2));
    float Hypotenuse = sqrt(pow(ydist, 2) + pow(xdist, 2));
    return Hypotenuse;
}



void Aimbot(int aimX, int aimY, double smooth) {
    if(cfg::aimbot_ativo){
            if ((GetAsyncKeyState)(cfg::aimkey)) {
                std::lock_guard<std::mutex> lock(fovMutex);
                currentFOV = cfg::aimbot_fov;
                    double x_{ 0.0 }, y_{ 0.0 }, overflow_x{ 0.0 }, overflow_y{ 0.0 };

                    double stepX = static_cast<double>(aimX) / smooth * cfg::speed;
                    double stepY = static_cast<double>(aimY) / smooth * cfg::speed;

                    for (int i = 1; i <= smooth; i++) {

                        int newX = static_cast<int>(stepX / i);
                        int newY = static_cast<int>(stepY / i);

                        add_overflow(newX, overflow_x);                        
                        add_overflow(newY, overflow_y);

                        int final_x{ static_cast<int>(newX - x_) };
                        int final_y{ static_cast<int>(newY - y_) };
                            if (cfg::recoil_ativo) {
                                XD.MouseEvent(final_x, final_y + cfg::recoil_offset, Driver::None);
                            }
                            else {
                                XD.MouseEvent(final_x, final_y, Driver::None);
                            }
                        x_ = newX;
                        y_ = newY;
                }
            }
    }
}

void recoil_control()
{
    stopwatch timer;

    while (true)
    {
        if ((GetAsyncKeyState)(cfg::recoil_key) && cfg::recoil_ativo)
        {
            if (timer.get_elapsed() > cfg::time_to_start)
            {
                if (cfg::recoil_offset < cfg::recoil_length)
                {
                    cfg::recoil_offset += cfg::recoil_speed;
                }
                else
                {
                    cfg::recoil_offset = cfg::recoil_length;
                }
            }
        }
        else
        {
            if (cfg::recoil_offset > 0)
            {
                cfg::recoil_offset -= cfg::recoil_speed;
            }
            else
            {
                cfg::recoil_offset = 0;
                timer.update();
            }
        }

        if (!(GetAsyncKeyState)(cfg::aimkey))
        {
            cfg::recoil_offset = 0;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
void Magnet(int aimX, int aimY, double smooth) {
    static bool keyPressProcessed = false; 

    if ((GetAsyncKeyState)(cfg::assist_aimkey)) {
        if (!keyPressProcessed) {
            key_ativa = !key_ativa;
            keyPressProcessed = true; 
        }
    }
    else {
        keyPressProcessed = false; 
    }

    if (cfg::aimassist_ativo && key_ativa) {
        std::lock_guard<std::mutex> lock(fovMutex);
        currentFOV = cfg::aimassist_fov;
                double x_{ 0.0 }, y_{ 0.0 }, overflow_x{ 0.0 }, overflow_y{ 0.0 };

                double stepX = static_cast<double>(aimX) / smooth * cfg::assist_speed;
                double stepY = static_cast<double>(aimY) / smooth * cfg::assist_speed;

                for (int i = 1; i <= smooth; i++) {

                    int newX = static_cast<int>(stepX / i);
                    int newY = static_cast<int>(stepY / i);

                    add_overflow(newX, overflow_x);
                    add_overflow(newY, overflow_y);

                    int final_x{ static_cast<int>(newX - x_) };
                    int final_y{ static_cast<int>(newY - y_) };

                    XD.MouseEvent(final_x, final_y, Driver::None);
                    x_ = newX;
                    y_ = newY;
                }
    }
}

