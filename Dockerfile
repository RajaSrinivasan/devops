FROM ubuntu:latest

ARG PASSWORD="devops"
ARG VS_CODE_SERVER_PORT=8800
ARG WGET_ARGS="-q --show-progress --progress=bar:force:noscroll"
ARG TARGETARCH
ARG TARGETOS
ARG ALRBINBASE="https://github.com/alire-project/alire/releases/download/v2.1.0/alr-2.1.0-bin"

# Set default shell during Docker image build to bash
SHELL [ "/bin/bash" , "-c" ]

# Base requirements.
RUN ln -fs /usr/share/zoneinfo/America/New_York /etc/localtime 
RUN apt-get update && \
    apt-get install -y \
        nano \
        curl \
        wget \
        git \
        sudo \
        tzdata \
        unzip \
        build-essential \
        openssh-server && \
    apt-get clean

# Install Python + pip
RUN apt-get install -y python3 python3-pip && \
    apt-get clean

# Set root password
RUN echo "root:${PASSWORD}" | chpasswd
# Allow root login and password authentication
RUN sed -i 's/#PermitRootLogin prohibit-password/PermitRootLogin yes/' /etc/ssh/sshd_config
RUN sed -i 's/#PasswordAuthentication yes/PasswordAuthentication yes/' /etc/ssh/sshd_config


ENV HOME=/root
RUN echo $HOME
RUN mkdir $HOME/Prj
RUN mkdir $HOME/bin
ENV PATH="$HOME/bin:$PATH"

RUN if [ "$TARGETARCH" = "arm64" ]; then \
        wget $ALRBINBASE-aarch64-$TARGETOS.zip -O alr.zip; \
    fi; \
    if [ "$TARGETARCH" = "amd64" ]; then \
        wget $ALRBINBASE-x86_64-$TARGETOS.zip -O alr.zip; \
    fi
RUN unzip alr.zip 
RUN mv bin/alr $HOME/bin

ENV PROJECTS=$HOME/Prj
WORKDIR $PROJECTS

# Build the native tools

RUN alr -n install gnat_native gprbuild
RUN alr -n toolchain --select gnat_native gprbuild


RUN git clone https://github.com/RajaSrinivasan/toolkit.git
RUN cd toolkit/adalib; alr -n build
RUN cd toolkit/utilities; python3 buildall.py; cp bin/* $HOME/bin
RUN rm -rf /root/.alire/lib64/libcc1.so
RUN alr --force install gnat_arm_elf
RUN alr -n toolchain --select gnat_arm_elf

COPY stm32f4crc/stm32f4crc $PROJECTS/stm32f4crc
RUN cd $PROJECTS/stm32f4crc; gcc -o stm32f4crc *.c ; cp stm32f4crc $HOME/bin

RUN apt-get install -y \
    usbutils \
    openocd \
    stlink-tools

RUN ssh-keygen -A

EXPOSE 22
CMD ["/usr/sbin/sshd" , "-D"]
