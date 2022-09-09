#pragma once
#include <SimpleIni.h>

void SetValue(CSimpleIniA& ini, const char* section, const char* key, int val);
void SetValue(CSimpleIniA& ini, const char* section, const char* key, const std::string& val);
void SetValue(CSimpleIniA& ini, const char* section, const char* key, bool val);
void SetValue(CSimpleIniA& ini, const char* section, const char* key, float val);
void SetValue(CSimpleIniA& ini, const char* section, const char* key, double val);

int GetValue(CSimpleIniA& ini, const char* section, const char* key, int val);
std::string GetValue(CSimpleIniA& ini, const char* section, const char* key, const std::string& val);
bool GetValue(CSimpleIniA& ini, const char* section, const char* key, bool val);
float GetValue(CSimpleIniA& ini, const char* section, const char* key, float val);
double GetValue(CSimpleIniA& ini, const char* section, const char* key, double val);
