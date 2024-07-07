import subprocess
import os
Import("env")

#print(env.Dump())

pio_env = env.get('PIOENV', '')
print("Current pio env: " + str(pio_env))

file_path = '/tmp/inkwatchy_env'
if os.path.exists(file_path):
     os.remove(file_path)

with open(file_path, 'w') as file:
     file.write(pio_env)

# Because cc1: warning: command-line option '-Wno-overloaded-virtual' is valid for C++/ObjC++ but not for C
env.Append(CXXFLAGS=["-Wno-overloaded-virtual"])

script_path = 'resources/tools/buildTime/preBuild.sh'
subprocess.run(['bash', script_path])