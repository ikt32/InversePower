/*
THIS FILE IS A PART OF GTA V SCRIPT HOOK SDK
http://dev-c.com
(C) Alexander Blade 2015
*/

#include "script.h"
#include <vector>

//DWORD	vehUpdateTime;
//DWORD	pedUpdateTime;

void update()
{
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	// check if player ped exists and control is on (e.g. not in a cutscene)
	if (!ENTITY::DOES_ENTITY_EXIST(playerPed) || !PLAYER::IS_PLAYER_CONTROL_ON(player))
		return;

	// check for player ped death and player arrest
	if (ENTITY::IS_ENTITY_DEAD(playerPed) || PLAYER::IS_PLAYER_BEING_ARRESTED(player, TRUE))
		return;

	Vehicle vehicle = PED::GET_VEHICLE_PED_IS_USING(playerPed);
	if (!VEHICLE::IS_THIS_MODEL_A_CAR(ENTITY::GET_ENTITY_MODEL(vehicle)))
		return;

	// default values; overwritten by config

	// how much power or torque are adjusted
	float power_adj = 1.0;
	float torque_adj = 1.0;

	// how much angle or speed impacts power/torque adjustment
	float angle_impact = 3.00;
	float speed_impact = 2.00;

	// we want 0 additional pwr @ >= 35m/s, full additional pwr @ 0m/s
	// speed where adjustment adjustment by speed stops
	float base = 35.0;

	power_adj = GetPrivateProfileInt("CONFIG", "Power", 100, "./InversePower.ini") / 100.0f;
	torque_adj = GetPrivateProfileInt("CONFIG", "Torque", 100, "./InversePower.ini") / 100.0f;

	angle_impact = GetPrivateProfileInt("CONFIG", "Angle", 300, "./InversePower.ini") / 100.0f;
	speed_impact = GetPrivateProfileInt("CONFIG", "Speed", 200, "./InversePower.ini") / 100.0f;

	base = GetPrivateProfileInt("CONFIG", "Slope", 35, "./InversePower.ini") / 1.0f;
	if (base < 0.0)
		base = 35.0;

	// starting values; don't change
	float power_mult = 1.0;
	float torque_mult = 1.0;

	// speed in m/s; 35m/s = 128km/h
	float speed = ENTITY::GET_ENTITY_SPEED(PED::GET_VEHICLE_PED_IS_USING(playerPed));
	


	Vector3 rel_vector = ENTITY::GET_ENTITY_SPEED_VECTOR(PED::GET_VEHICLE_PED_IS_USING(playerPed), TRUE);
	
	float angle = acos(rel_vector.y / speed)* 180.0f / 3.14159265f;
	if (isnan(angle))
		angle = 0.0;

	if (angle > 90.0)
		angle = 180.0f - angle;

	float speed_mult = 0.0;
	if (speed < base)
	{
		speed_mult = (base - speed) / base;
	}

	power_mult = power_mult + power_adj * (((angle / 90) * angle_impact) + ((angle / 90) * speed_mult * speed_impact));
	torque_mult = torque_mult + torque_adj * (((angle / 90) * angle_impact) + ((angle / 90) * speed_mult * speed_impact));

	VEHICLE::_SET_VEHICLE_ENGINE_POWER_MULTIPLIER(vehicle, power_mult);
	VEHICLE::_SET_VEHICLE_ENGINE_TORQUE_MULTIPLIER(vehicle, torque_mult);
	
	int debug = GetPrivateProfileInt("DEBUG", "Debug", 0, "./InversePower.ini");

	if (debug)
	{
		Vector3 v = ENTITY::GET_ENTITY_COORDS(vehicle, TRUE);
		float x, y;
		if (GRAPHICS::_WORLD3D_TO_SCREEN2D(v.x, v.y, v.z, &x, &y)) {
			char text[256];
			sprintf_s(text, "^\n X %.02f\n Y %.02f\n Z %.02f\n Vel %.02f\n Mult %.02f\n Angle %.02f\n",
				rel_vector.x, rel_vector.y, rel_vector.z, speed, power_mult, angle);
			UI::SET_TEXT_FONT(0);
			UI::SET_TEXT_SCALE(0.2f, 0.2f);
			UI::SET_TEXT_COLOUR(255, 255, 255, 255);
			UI::SET_TEXT_WRAP(0.0, 1.0);
			UI::SET_TEXT_CENTRE(0);
			UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
			UI::SET_TEXT_EDGE(1, 0, 0, 0, 205);
			UI::_SET_TEXT_ENTRY("STRING");
			UI::_ADD_TEXT_COMPONENT_STRING(text);
			UI::_DRAW_TEXT(x, y);
			GRAPHICS::DRAW_RECT(x + 0.027f, y + 0.043f, 0.058f, 0.066f, 75, 75, 75, 75);
		}
	}
}

void main()
{
	while (true)
	{
		update();
		WAIT(0);
	}
}

void ScriptMain()
{
	srand(GetTickCount());
	main();
}
