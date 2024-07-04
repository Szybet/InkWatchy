#!/bin/bash

# This sometimes may not work, try a few times but:

# in /home/szybet/.espressif/python_env/idf5.2_py3.12_env/lib/python3.12/site-packages/esp_coredump/corefile/loader.py
# remove these lines:

# if core_sha_trimmed != app_sha_trimmed:
#     raise ESPCoreDumpLoaderError(
#         'Invalid application image for coredump: coredump SHA256({}) != app SHA256({}).'
#         .format(core_sha_trimmed, app_sha_trimmed))
# if coredump_sha256.ver != self.version:
#     raise ESPCoreDumpLoaderError(
#         'Invalid application image for coredump: coredump SHA256 version({}) != app SHA256 version({}).'
#         .format(coredump_sha256.ver, self.version))

# In /workspaces/InkWatchy/.esp-idf/espressif/python_env/idf5.1_py3.12_env/lib64/python3.12/site-packages/esp_coredump/corefile/loader.py
# Change ESP_COREDUMP_PART_TABLE_OFF to 0x19000
# And part_table_offset too

source ../../globalFunctions.sh
source ~/.platformio/packages/framework-espidf/export.sh

pio_env=$(get_pio_env ../../../../.vscode/launch.json)

. /workspaces/InkWatchy/.esp-idf/esp-idf/export.sh 

espcoredump.py --chip esp32 info_corefile /workspaces/InkWatchy/.pio/build/$pio_env/firmware.elf