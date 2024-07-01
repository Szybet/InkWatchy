#!/bin/bash

# This sometimes may not work, try a few times but:

# for sure you need to edit the library
# in /home/szybet/.espressif/python_env/idf5.2_py3.12_env/lib/python3.12/site-packages/esp_coredump/corefile/loader.py
# (your path may be different)
# remove these lines:

# if core_sha_trimmed != app_sha_trimmed:
#     raise ESPCoreDumpLoaderError(
#         'Invalid application image for coredump: coredump SHA256({}) != app SHA256({}).'
#         .format(core_sha_trimmed, app_sha_trimmed))
# if coredump_sha256.ver != self.version:
#     raise ESPCoreDumpLoaderError(
#         'Invalid application image for coredump: coredump SHA256 version({}) != app SHA256 version({}).'
#         .format(coredump_sha256.ver, self.version))

# this is only supported on arch linux esp-idf aur package

source ../../globalFunctions.sh
source ~/.platformio/packages/framework-espidf/export.sh

pio_env=$(get_pio_env ../../../../.vscode/launch.json)

#/root/.platformio/penv/bin/python -m pip install --break-system-packages setuptools

. /workspaces/InkWatchy/.esp-idf/esp-idf/export.sh 

#esp-coredump info_corefile ../../../../.pio/build/$pio_env/firmware.elf
#./esp-coredump info_corefile /workspaces/InkWatchy/.pio/build/Watchy_2/firmware.elf
espcoredump.py --chip esp32 info_corefile /workspaces/InkWatchy/.pio/build/Watchy_2/firmware.elf