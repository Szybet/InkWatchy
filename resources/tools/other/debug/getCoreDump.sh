#!/bin/bash

# In /workspaces/InkWatchy/.esp-idf/espressif/python_env/idf5.1_py3.12_env/lib64/python3.12/site-packages/esp_coredump/corefile/loader.py
# Change ESP_COREDUMP_PART_TABLE_OFF to 0x19000
# And part_table_offset to 0x19000 too 

# So it will work something like this
# class ESPCoreDumpFlashLoader(EspCoreDumpLoader):
#     ESP_COREDUMP_PART_TABLE_OFF = 0x19000

#     def init(self, offset, target=None, port=None, baud=None, part_table_offset=0x19000):
#         # type: (Optional[int], Optional[str], Optional[str], Optional[int], Optional[int]) -> None

source ../../globalFunctions.sh
source ~/.platformio/packages/framework-espidf/export.sh

pio_env=$(get_pio_env ../../../../.vscode/launch.json)

. /workspaces/InkWatchy/.esp-idf/esp-idf/export.sh 

espcoredump.py --chip esp32 info_corefile /workspaces/InkWatchy/.pio/build/$pio_env/firmware.elf