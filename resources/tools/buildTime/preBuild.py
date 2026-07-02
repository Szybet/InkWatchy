import subprocess
import os
import sys
Import("env")

# print(env.Dump())

port = env.get("UPLOAD_PORT")
if port:
    with open("/tmp/inkwatchy_port", "w") as f:
        f.write(port)

pio_env = env.get('PIOENV', '')
print("Current pio env: " + str(pio_env))

if str(pio_env) == "Unknown":
     print("Select the proper PlatformIO environment; it is currently unknown!", file=sys.stderr)
     sys.exit(1)

# Lp core check
if str(pio_env) == "Yatchy":
    dir_path = "resources/tools/fs/littlefs/other/lpcore/"
    has_files = False
    if os.path.isdir(dir_path):
        files = [f for f in os.listdir(dir_path) if f != ".keep"]
        if len(files) > 0:
            has_files = True

    if not has_files:
        with open("src/defines/condition.h") as f:
            for line in f:
                if "#define LP_CORE" in line:
                    if "1" in line:
                        print("LP_CORE is defined as 1 but the lpcore directory is empty")
                        sys.exit(1)
                    break

file_path = '/tmp/inkwatchy_env'
if os.path.exists(file_path):
     os.remove(file_path)

with open(file_path, 'w') as file:
     file.write(pio_env)

# Because cc1: warning: command-line option '-Wno-overloaded-virtual' is valid for C++/ObjC++ but not for C
env.Append(CXXFLAGS=["-Wno-overloaded-virtual"])

script_path = 'resources/tools/buildTime/preBuild.sh'
subprocess.run(['bash', script_path])

script_path = 'components/rusty/build_rust.sh'
subprocess.run(['bash', script_path], check=True)
