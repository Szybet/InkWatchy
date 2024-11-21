#ifndef WIFI_TASK_H
#define WIFI_TASK_H

#include "defines.h"

extern TaskHandle_t wifiTask;
extern std::mutex wifiTaskMutex;

void createWifiTask(uint8_t tries, void (*functionToRunAfterConnection)(), uint8_t wifiPriority);
void turnOnWifiTask(void *parameter);
void turnOffWifiMinimal();
void turnOffWifi();

bool isWifiTaskCheck();
String wifiStatus();
wifiStatusSimple wifiStatusWrap();

#endif
