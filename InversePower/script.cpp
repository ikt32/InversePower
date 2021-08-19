#include "Script.h"
#include "ScriptSettings.hpp"
#include "Util/Logger.hpp"
#include "Util/Math.hpp"
#include "Util/UI.hpp"

#include <inc/natives.h>

#include <algorithm>
#include <format>
#include <memory>
#include <string>

namespace {
    std::unique_ptr<CScriptSettings> settings;
}

void Update(Vehicle vehicle, bool player) {
    float speed = ENTITY::GET_ENTITY_SPEED(vehicle);
    Vector3 relVector = ENTITY::GET_ENTITY_SPEED_VECTOR(vehicle, true);

    float angle = acos(relVector.y / speed) * 180.0f / 3.14159265f;
    if (isnan(angle))
        angle = 0.0;

    float speedMod = map(speed,
        settings->Configuration.SpeedStart, settings->Configuration.SpeedEnd,
        settings->Configuration.SpeedStartTorqueMod, 0.0f);
    speedMod = std::clamp(speedMod, 0.0f, settings->Configuration.SpeedStartTorqueMod);

    float torqueMod = map(angle,
        settings->Configuration.AngleStart, settings->Configuration.AngleEnd,
        settings->Configuration.AngleStartTorqueMod, settings->Configuration.AngleEndTorqueMod);
    torqueMod = std::clamp(torqueMod, settings->Configuration.AngleStartTorqueMod, settings->Configuration.AngleEndTorqueMod);

    float torqueMult = 1.0f + speedMod * torqueMod;

    if (angle > settings->Configuration.AngleStart) {
        VEHICLE::SET_VEHICLE_CHEAT_POWER_INCREASE(vehicle, torqueMult);
    }
    else {
        torqueMult = 1.0;
    }

    if (settings->Main.Debug && player) {
        Vector3 pos = ENTITY::GET_ENTITY_COORDS(vehicle, true);
        UI::ShowText3D(pos, 5.0f, {
            std::format("Angle: {:.2f}", angle),
            std::format("Speed: {:.2f}", speed),
            std::format("SpeedMod: {:.2f}", speedMod),
            std::format("TorqueMod: {:.2f}", torqueMod),
            std::format("TorqueMult: {:.2f}", torqueMult),
        });
    }
}

void Update() {
    if (!settings->Main.Enable)
        return;

    Ped playerPed = PLAYER::PLAYER_PED_ID();
    Vehicle playerVehicle = PED::GET_VEHICLE_PED_IS_USING(playerPed);

    if (!ENTITY::DOES_ENTITY_EXIST(playerVehicle) ||
        !VEHICLE::IS_THIS_MODEL_A_CAR(ENTITY::GET_ENTITY_MODEL(playerVehicle)) ||
        playerPed != VEHICLE::GET_PED_IN_VEHICLE_SEAT(playerVehicle, -1, false)) {
        playerVehicle = 0;
    }

    if (playerVehicle != 0) {
        Update(playerVehicle, true);
    }

    if (settings->Main.EnableForNPCs) {
        constexpr int maxNpcVehicles = 1024;
        std::vector<Vehicle> allVehicles(maxNpcVehicles);
        int numVehicles = worldGetAllVehicles(allVehicles.data(), maxNpcVehicles);
        allVehicles.resize(numVehicles);

        for (auto vehicle : allVehicles) {
            if (vehicle == playerVehicle)
                continue;

            if (!ENTITY::DOES_ENTITY_EXIST(vehicle))
                continue;

            if (!VEHICLE::IS_THIS_MODEL_A_CAR(ENTITY::GET_ENTITY_MODEL(vehicle)))
                continue;

            if (!VEHICLE::GET_IS_VEHICLE_ENGINE_RUNNING(vehicle))
                continue;

            Update(vehicle, false);
        }
    }
}

void UpdateCheat() {
    Hash cheatHash = MISC::GET_HASH_KEY(settings->Main.ReloadCheat.c_str());
    if (MISC::_HAS_CHEAT_STRING_JUST_BEEN_ENTERED(cheatHash)) {
        settings->Load();
        UI::Notify("Reloaded settings.");
        Log::Write(DEBUG, "Reloaded settings");
    }
}

void ScriptMain() {
    Log::SetMinLevel(INFO);
    Log::Write(INFO, "Script started");
    settings = std::make_unique<CScriptSettings>(".\\InversePower.ini");
    Log::Write(DEBUG, "Instantiated settings");
    settings->Load();
    Log::Write(DEBUG, "Loaded settings");

    while (true) {
        UpdateCheat();
        Update();
        WAIT(0);
    }
}
