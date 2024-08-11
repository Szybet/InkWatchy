#!/bin/bash

source resources/tools/globalFunctions.sh

pio_env=$(get_pio_env .vscode/launch.json)

#cd .pio/build/$pio_env

#ninja -k 0 || true
#exit 0

# check_and_run() {
#     TARGET="$1"
#     FILE="$2"

#     if [ ! -f "$FILE" ]; then
#         echo "Missing $FILE" >&2
#         ninja "$TARGET" -k 0 || true
#     fi
# }

#check_and_run "https_server.crt.S" "https_server.crt.S"
#check_and_run "rmaker_mqtt_server.crt.S" "rmaker_mqtt_server.crt.S"
#check_and_run "rmaker_claim_service_server.crt.S" "rmaker_claim_service_server.crt.S"
#check_and_run "rmaker_ota_server.crt.S" "rmaker_ota_server.crt.S"

#cp managed_components/espressif__esp_insights/server_certs/https_server.crt .pio/build/$pio_env/https_server.crt
#cp managed_components/espressif__esp_rainmaker/server_certs/rmaker_mqtt_server.crt .pio/build/$pio_env/
#cp managed_components/espressif__esp_rainmaker/server_certs/rmaker_claim_service_server.crt .pio/build/$pio_env/
#cp managed_components/espressif__esp_rainmaker/server_certs/rmaker_ota_server.crt .pio/build/$pio_env/

if [ ! -f "sdkconfig.defaults" ]; then
    rm -f "sdkconfig.${pio_env}"
fi

cp resources/tools/buildTime/sdkconfigs/sdkconfig_general.defaults sdkconfig.defaults
cat resources/tools/buildTime/sdkconfigs/sdkconfig_${pio_env}.defaults >> sdkconfig.defaults

for dir in managed_components/*; do
    if [ -d "$dir" ] && [ -f "$dir/.component_hash" ]; then
        rm -f "$dir/.component_hash"
    fi
done