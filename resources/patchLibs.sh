#!/bin/bash

source global_functions.sh

pio_env=$(get_pio_env ../.vscode/launch.json)

rtc_path="../.pio/libdeps/$pio_env/Rtc_Pcf8563/src/Rtc_Pcf8563.h"

wrong_st="#define RTCC_VERSION  \"Pcf8563 v1.0.3\""

new_str="#define RTCC_VERSION  (char*)\"Pcf8563 v1.0.3\""

sed -i "s/^${wrong_st}.*/$new_str/" "$rtc_path"