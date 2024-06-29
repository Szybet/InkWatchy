import subprocess
Import("env")

# Because cc1: warning: command-line option '-Wno-overloaded-virtual' is valid for C++/ObjC++ but not for C
env.Append(CXXFLAGS=["-Wno-overloaded-virtual"])

script_path = 'resources/tools/buildTime/preBuild.sh'
subprocess.run(['bash', script_path])