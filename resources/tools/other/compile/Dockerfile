FROM --platform=linux/amd64 ubuntu:24.04

# InkWatchy
RUN apt-get update && apt-get install -y \
    mupdf-tools \
    xxd \
    jq \
    python3-unidecode \
    libfreetype-dev \
    libfreetype6 \
    libfreetype6-dev \
    python3-venv \
    imagemagick \
    libltdl-dev \
    make \
    build-essential \
    automake \
    ninja-build \
    pip \
    dialog \
    fuse \
    libfuse2 \
    unzip \
    ffmpeg \
    sudo

# Esp idf
RUN apt-get install -y \
    git \
    wget \
    flex \
    bison \
    gperf \
    python3 \
    python3-pip \
    python3-venv \
    cmake \
    ninja-build \
    ccache \
    libffi-dev \
    libssl-dev \
    dfu-util \
    libusb-1.0-0

# Misc & Pio
RUN apt-get install -y \
    silversearcher-ag \
    curl \
    clang \
    udev \
    locales

# Watchy scom & xorg
# To check if x11 works, and to make it work run "xhost local:root" on the host then check if it works by running xclock in the container
# Also select the non xorg linux dev container
# https://stackoverflow.com/questions/60733288/devcontainer-how-to-make-x-display-work-mount-graphics-inside-docker-in-visual
# https://github.com/thewtex/docker-opengl/blob/master/Dockerfile
RUN apt-get install -y \
    x11-apps \
    libudev-dev \
    pkg-config \
    libgl1-mesa-dri \
    libxkbcommon-dev

# https://linuxdecoded.com/fix-unsupported-locale-setting-in-python-or-cli-alternative-for-dpkg-reconfigure-locales/
RUN echo "en_US.UTF-8 UTF-8" | sudo tee -a /etc/locale.gen; sudo locale-gen en_US.utf8

# Adds things to bashrc, to be sure
RUN echo 'export PATH="$PATH:$HOME/.root/.platformio/penv/bin"' | tee -a /root/.bashrc /root/.zshrc
RUN echo 'export PYTHONPATH="$PYTHONPATH:/root/.platformio/packages/tool-esptoolpy"' | tee -a /root/.bashrc /root/.zshrc
RUN git config --global --add safe.directory '*'

RUN mkdir -p /root/esp-idf/
RUN mkdir -p /root/.espressif/

#RUN curl -fsSL -o /tmp/get-platformio.py https://raw.githubusercontent.com/platformio/platformio-core-installer/master/get-platformio.py
#RUN python3 /tmp/get-platformio.py

# Platformio folder prerequisite for compliling Watchy firmware via Terminal/Docker
ENV PATH=$PATH:/root/.platformio/penv/bin

# Adding rust to path
ENV PATH=$PATH:/root/.cargo/bin
ENV PATH=$PATH:$HOME/.cargo/bin

# For rusty stuff
# Idk weird fix with update
RUN apt-get update && apt-get install -y gcc build-essential curl pkg-config libinput10

# Compression
RUN pip install --no-input --break-system-packages tamp==1.10.1
