FROM docker.io/fedora:34 AS builder
ARG POSTGRES=13.1
ARG USERNAME=vscode
ARG USER_UID=1000
ARG USER_GID=$USER_UID

RUN dnf install -y \
    bzip2 \
    diffutils \
    findutils \
    gcc \
    make \
    readline-devel \
    tar \
    wget \
    zlib-devel

RUN wget https://ftp.postgresql.org/pub/source/v${POSTGRES}/postgresql-${POSTGRES}.tar.bz2 -nv -O postgres.tar.bz2 \
    && tar xvf postgres.tar.bz2 \
    && rm postgres.tar.bz2

RUN groupadd --gid $USER_GID $USERNAME \
     && useradd -s /bin/bash --uid $USER_UID --gid $USER_GID -m $USERNAME

RUN mkdir build \
    && chown -R $USER_UID:$USER_GID build

USER ${USERNAME}

RUN cd build \
    && ../postgresql-${POSTGRES}/configure \
        --enable-cassert \
        --prefix=/opt/postgresql \
    && make world \
    && make check

USER root

RUN cd build && make install-world

FROM rvianello/fedora-rdkit-cpp:34-e32810c

# This Dockerfile image has a non-root user with sudo access. Use the "remoteUser"
# # property in devcontainer.json to use it. On Linux, the container user's GID/UIDs
# will be updated to match your local UID/GID (when using the dockerFile property).
# See https://aka.ms/vscode-remote/containers/non-root-user for details.
ARG USERNAME=vscode
ARG USER_UID=1000
ARG USER_GID=$USER_UID

# Configure and install packages
RUN dnf install -y \
    boost-devel \
    #cairo-devel \
    catch-devel \
    cmake \
    cppcheck \
    diffutils \
    findutils \
    eigen3-devel \
    g++ \
    gcc \
    gdb \
    git \
    make \
    redhat-rpm-config \
    valgrind \
    which \
    zlib-devel \
    \
    # Create a non-root user to use if preferred - see https://aka.ms/vscode-remote/containers/non-root-user.
    && groupadd --gid $USER_GID $USERNAME \
    && useradd -s /bin/bash --uid $USER_UID --gid $USER_GID -m $USERNAME \
    # [Optional] Add sudo support for the non-root user
    && echo $USERNAME ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/$USERNAME\
    && chmod 0440 /etc/sudoers.d/$USERNAME

COPY --from=builder /opt/postgresql /opt/postgresql
RUN chown -R $USER_UID:$USER_GID /opt/postgresql \
    && mkdir /opt/pgdata \
    && chown -R $USER_UID:$USER_GID /opt/pgdata \
    && ldconfig /opt/postgresql/lib



