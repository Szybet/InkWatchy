FROM ubuntu:24.04

RUN apt-get update

# InkWatchy
RUN apt-get install -y \
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
    udev 

#RUN apt-get clean
#RUN rm -rf /var/lib/apt/lists/*

RUN echo 'export PATH="$PATH:$HOME/.platformio/penv/bin"' | tee -a /root/.bashrc /root/.zshrc
RUN echo 'export PYTHONPATH="$PYTHONPATH:/root/.platformio/packages/tool-esptoolpy"' | tee -a /root/.bashrc /root/.zshrc
RUN git config --global --add safe.directory '*'

RUN mkdir -p /root/esp-idf/
RUN mkdir -p /root/.espressif/

#RUN curl -fsSL -o /tmp/get-platformio.py https://raw.githubusercontent.com/platformio/platformio-core-installer/master/get-platformio.py
#RUN python3 /tmp/get-platformio.py