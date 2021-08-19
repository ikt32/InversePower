#pragma once
#include <string>
#include <vector>

class CScriptSettings {
public:
    CScriptSettings(std::string settingsFile);

    void Load();

    struct {
        bool Enable = true;
        bool EnableForNPCs = true;
        std::string ReloadCheat = "rip";
        bool Debug = false;
    } Main;

    struct {
        float SpeedStart = 0.0f;
        float SpeedStartTorqueMod = 2.0f;
        float SpeedEnd = 36.0f;

        float AngleStart = 0.0f;
        float AngleStartTorqueMod = 1.0f;
        float AngleEnd = 90.0f;
        float AngleEndTorqueMod = 2.0f;

        float Gamma = 1.0f;
    } Configuration;

private:
    std::string mSettingsFile;
};

