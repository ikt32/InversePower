#include "ScriptSettings.hpp"
#include "SettingsCommon.hpp"
#include "Util/Logger.hpp"

#include <SimpleIni.h>

#define CHECK_LOG_SI_ERROR(result, operation) \
    if ((result) < 0) { \
        Log::Write(ERROR, "[Settings] Failed to {}, SI_Error [{}]", \
        operation, (int)result); \
    }

#define LOAD_VAL(section, key, option) \
    option = GetValue(ini, section, key, option)

CScriptSettings::CScriptSettings(std::string settingsFile)
    : mSettingsFile(std::move(settingsFile)) {

}

void CScriptSettings::Load() {
    CSimpleIniA ini;
    ini.SetUnicode();
    SI_Error result = ini.LoadFile(mSettingsFile.c_str());
    CHECK_LOG_SI_ERROR(result, "load");

    LOAD_VAL("Main", "Enable", Main.Enable);
    LOAD_VAL("Main", "EnableForNPCs", Main.EnableForNPCs);
    LOAD_VAL("Main", "ReloadCheat", Main.ReloadCheat);
    LOAD_VAL("Main", "Debug", Main.Debug);

    LOAD_VAL("Configuration", "SpeedStart", Configuration.SpeedStart);
    LOAD_VAL("Configuration", "SpeedStartTorqueMod", Configuration.SpeedStartTorqueMod);
    LOAD_VAL("Configuration", "SpeedEnd", Configuration.SpeedEnd);

    LOAD_VAL("Configuration", "AngleStart", Configuration.AngleStart);
    LOAD_VAL("Configuration", "AngleStartTorqueMod", Configuration.AngleStartTorqueMod);
    LOAD_VAL("Configuration", "AngleEnd", Configuration.AngleEnd);
    LOAD_VAL("Configuration", "AngleEndTorqueMod", Configuration.AngleEndTorqueMod);

    LOAD_VAL("Configuration", "Gamma", Configuration.Gamma);
}
