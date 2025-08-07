#!/bin/bash

# Nevermind!
# In .esp-idf/espressif/python_env/idf5.1_py3.12_env/lib64/python3.12/site-packages/esp_coredump/corefile/loader.py
# Or if it doesn't exist .root/.espressif/python_env/idf5.3_py3.12_env/lib/python3.12/site-packages/esp_coredump/corefile/loader.py
# Change ESP_COREDUMP_PART_TABLE_OFF to 0x19000
# And part_table_offset to 0x19000 too 

# So it will work something like this
# class ESPCoreDumpFlashLoader(EspCoreDumpLoader):
#     ESP_COREDUMP_PART_TABLE_OFF = 0x19000

#     def init(self, offset, target=None, port=None, baud=None, part_table_offset=0x19000):
#         # type: (Optional[int], Optional[str], Optional[str], Optional[int], Optional[int]) -> None

source ../../globalFunctions.sh

cd ../../../../
.root/.platformio/packages/framework-espidf/install.sh
.root/.platformio/penv/bin/python .root/.platformio/packages/framework-espidf/tools/idf_tools.py install
ln -s /usr/bin/python3 /usr/bin/python
#env -i .root/.platformio/packages/framework-espidf/tools/idf_tools.py install-python-env
source ~/.platformio/packages/framework-espidf/export.sh
idf_tools.py install-python-env
source ~/.platformio/packages/framework-espidf/export.sh

pio_env=$(get_pio_env ../../../../.vscode/launch.json)

PYTHONPATH=~/.espressif/python_env/idf5.4_py3.12_env/lib/python3.12/site-packages:$PYTHONPATH ~/.espressif/python_env/idf5.4_py3.12_env/bin/esp-coredump info_corefile ../../../../.pio/build/$pio_env/firmware.elf