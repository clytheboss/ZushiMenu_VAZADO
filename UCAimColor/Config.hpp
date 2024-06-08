#include <Windows.h>

namespace cfg
{
	inline int color_mode = 0;
	inline bool advancednigger = false;

	inline bool aimbot_ativo{ true };
	inline int aimkey = { VK_MENU };
	inline int head_offset_x{ 1 };
	inline int head_offset_y{ 3 };
	inline int aimbot_fov{ 35 };
	inline float aimbot_smooth{ 1.5 };
	inline int speed{ 4 };
	inline int sleep{ 0 };

	inline bool recoil_ativo{ false };
	inline int recoil_length{ 0 };
	inline int recoil_sleep{ 1 };
	inline int time_to_start{ 155 };
	inline int recoil_key = { VK_MENU };
	inline float recoil_speed{ 0.055 };
	inline double recoil_offset{ 0 };


	inline bool aimassist_ativo{ false };
	inline int assist_aimkey = { VK_MENU };
	inline int assist_head_offset_x{ 2 };
	inline int assist_head_offset_y{ 3 };
	inline int aimassist_fov{ 1 };
	inline float aimassist_smooth{ 1.5 };
	inline int assist_speed{ 1 };
	inline int assist_sleep{ 0 };

	inline bool flicker_ativo{ false };
	inline int flicker_key{ VK_XBUTTON2 };
	inline int flicker_fov{ 100 };
	inline int flicker_delay_between_shots{ 1 };
	inline float flicker_distance = 2.5f;

	inline bool silent_ativo{ false };
	inline int silent_key{ VK_XBUTTON2 };
	inline int silent_head_offset_x{ 2 };
	inline int silent_head_offset_y{ 3 };
	inline int silent_fov{ 100 };
	inline int silent_delay_between_shots{ 1 };
	inline float distance = 2.5f;

	inline bool triggerbot_ativo{ false };
	inline int triggerbot_key{ VK_XBUTTON2 };
	inline int triggerbot_delay{ 1 };
	inline int triggerbot_fovX{ 3 };
	inline int triggerbot_fovY{ 3 };
	//cores
	inline int menorRGB[3] = { 70, 0, 120 };
	inline int maiorRGB[3] = { 255, 190, 255 };
	inline int menorHSV[3] = { 270, 38, 40 };
	inline int maiorHSV[3] = { 310, 100, 100 };
}
