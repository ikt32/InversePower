/*
THIS FILE IS A PART OF GTA V SCRIPT HOOK SDK
http://dev-c.com
(C) Alexander Blade 2015
*/

#pragma once

#include "..\..\ScriptHookV_SDK\inc\natives.h"
#include "..\..\ScriptHookV_SDK\inc\types.h"
#include "..\..\ScriptHookV_SDK\inc\enums.h"

#include "..\..\ScriptHookV_SDK\inc\main.h"

void ScriptMain();
// jesus fuck isn't there a better way?
void showDebugInfo3D(Vehicle vehicle, Vector3 rel_vector, float speed, float power_mult, float torque_mult, float angle);