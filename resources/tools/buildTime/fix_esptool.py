Import("env")

import importlib.metadata
import subprocess
import sys

required_version = "1.1.4"

try:
    installed_version = importlib.metadata.version("esp-pylib")
except importlib.metadata.PackageNotFoundError:
    installed_version = None

if installed_version != required_version:
    subprocess.run([
        sys.executable,
        "-m",
        "pip",
        "install",
        "--force-reinstall",
        f"esp-pylib=={required_version}"
    ], check=True)
