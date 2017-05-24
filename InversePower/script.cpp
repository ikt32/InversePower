#include "script.h"
#include <vector>
#include <string>

//bool underGear = false;
Player player;
Ped playerPed;
Vehicle vehicle;

float speed = 0.0;
Vector3 rel_vector = { 0.0f, 0, 0.0f, 0, 0.0f, 0 };
float angle = 0.0;
float deadzone = 0.0;

float base = 35.0;
float power_adj = 1.0;
float torque_adj = 1.0;
float angle_impact = 3.0;
float speed_impact = 2.0;

float speed_mult = 0.0;
float power_mult = 1.0;
float torque_mult = 1.0;

int accelval = 127;
int brakeval = 127;

int disablep = 0;
int disablet = 0;
int debug = 0;

void readSettings() {
	disablep = GetPrivateProfileInt("DEBUG", "DisableP", 0, "./InversePower.ini");
	disablet = GetPrivateProfileInt("DEBUG", "DisableT", 0, "./InversePower.ini");

	power_adj = GetPrivateProfileInt("CONFIG", "Power", 100, "./InversePower.ini") / 100.0f;
	torque_adj = GetPrivateProfileInt("CONFIG", "Torque", 100, "./InversePower.ini") / 100.0f;

	angle_impact = GetPrivateProfileInt("CONFIG", "Angle", 300, "./InversePower.ini") / 100.0f;
	speed_impact = GetPrivateProfileInt("CONFIG", "Speed", 200, "./InversePower.ini") / 100.0f;

	base = GetPrivateProfileInt("CONFIG", "Slope", 35, "./InversePower.ini") / 1.0f;

	deadzone = GetPrivateProfileInt("CONFIG", "Deadzone", 0, "./InversePower.ini") / 1.0f;

	debug = GetPrivateProfileInt("DEBUG", "Debug", 0, "./InversePower.ini");
}

void showText(float x, float y, float scale, char * text) {
	UI::SET_TEXT_FONT(0);
	UI::SET_TEXT_SCALE(scale, scale);
	UI::SET_TEXT_COLOUR(255, 255, 255, 255);
	UI::SET_TEXT_WRAP(0.0, 1.0);
	UI::SET_TEXT_CENTRE(0);
	UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
	UI::SET_TEXT_EDGE(1, 0, 0, 0, 205);
	UI::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
	UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
	UI::END_TEXT_COMMAND_DISPLAY_TEXT(x, y);
}

void update()
{
	player = PLAYER::PLAYER_ID();
	playerPed = PLAYER::PLAYER_PED_ID();

	// check if player ped exists and control is on (e.g. not in a cutscene)
	if (!ENTITY::DOES_ENTITY_EXIST(playerPed) || !PLAYER::IS_PLAYER_CONTROL_ON(player))
		return;

	// check for player ped death and player arrest
	if (ENTITY::IS_ENTITY_DEAD(playerPed) || PLAYER::IS_PLAYER_BEING_ARRESTED(player, TRUE))
		return;

	vehicle = PED::GET_VEHICLE_PED_IS_USING(playerPed);
	if (!VEHICLE::IS_THIS_MODEL_A_CAR(ENTITY::GET_ENTITY_MODEL(vehicle)))
		return;

	if (CONTROLS::IS_CONTROL_JUST_RELEASED(0, ControlEnter))
		readSettings();

	//// Compatibility with Gears/Manual Transmission mod
	//if (DECORATOR::DECOR_GET_INT(vehicle, "hunt_score") == 2) {
	//	underGear = true;
	//}
	//else {
	//	underGear = false;
	//}

	if (base < 0.0)
		base = 35.0;

	// speed in m/s; 35m/s = 128km/h
	speed = ENTITY::GET_ENTITY_SPEED(vehicle);
	rel_vector = ENTITY::GET_ENTITY_SPEED_VECTOR(vehicle, true);
	
	angle = acos(rel_vector.y / speed)* 180.0f / 3.14159265f;
	if (isnan(angle))
		angle = 0.0;

	if (speed < base)
	{
		speed_mult = (base - speed) / base;
	}

	power_mult  = 1.0f + power_adj  * (((angle / 90) * angle_impact) + ((angle / 90) * speed_mult * speed_impact));
	torque_mult = 1.0f + torque_adj * (((angle / 90) * angle_impact) + ((angle / 90) * speed_mult * speed_impact));

	// works on keyboard and controller. param 1, don't know what it does
	// pressed/max = 254. default/depressed = 127.
	accelval = CONTROLS::GET_CONTROL_VALUE(0, ControlVehicleAccelerate);
	brakeval = CONTROLS::GET_CONTROL_VALUE(0, ControlVehicleBrake);

	// no need to worry about values since we compare them with each other
	if (!(angle > 135 && brakeval > accelval + 12) && angle > deadzone)
	{
		if (!disablet) {
			VEHICLE::_SET_VEHICLE_ENGINE_TORQUE_MULTIPLIER(vehicle, torque_mult);
		}
		if (!disablep) {
			VEHICLE::_SET_VEHICLE_ENGINE_POWER_MULTIPLIER(vehicle, power_mult);
		}
	}
	else
	{
		power_mult = 1.0;
		torque_mult = 1.0;
	}
	
	if (debug)
	{
		showDebugInfo3D(vehicle, rel_vector, speed, power_mult, torque_mult, angle);
	}
}

void showDebugInfo3D(Vehicle vehicle, Vector3 rel_vector, float speed, float power_mult, float torque_mult, float angle)
{
	Vector3 v = ENTITY::GET_ENTITY_COORDS(vehicle, TRUE);
	float x, y;
	if (GRAPHICS::GET_SCREEN_COORD_FROM_WORLD_COORD(v.x, v.y, v.z, &x, &y)) {
		char text[256];
		sprintf_s(text, "X %.02f\nY %.02f\nVel %.02f\nPowX %.02f\nTorX %.02f\nAngle %.02f",
			rel_vector.x, rel_vector.y, speed, power_mult, torque_mult, angle);
		UI::SET_TEXT_FONT(0);
		UI::SET_TEXT_SCALE(0.2f, 0.2f);
		UI::SET_TEXT_COLOUR(255, 255, 255, 255);
		UI::SET_TEXT_WRAP(0.0, 1.0);
		UI::SET_TEXT_CENTRE(0);
		UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
		UI::SET_TEXT_EDGE(1, 0, 0, 0, 205);
		UI::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
		UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
		UI::END_TEXT_COMMAND_DISPLAY_TEXT(x, y);
		GRAPHICS::DRAW_RECT(x + 0.027f, y + 0.043f, 0.058f, 0.096f, 75, 75, 75, 75);
	}
}

void main()
{
	readSettings();
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
