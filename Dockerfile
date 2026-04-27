FROM ubuntu:24.04

# -qq: No output except for errors
RUN apt-get update && \
    apt-get --yes -qq install \
    build-essential make cmake \
    llvm \
    bear \
 && rm -rf /var/lib/apt/lists/*

# User/group names/ids which will be overwritten: https://stackoverflow.com/a/44683248
ARG UNAME
ARG GNAME
ARG UID
ARG GID
# Add a user
RUN useradd -m -u $UID -g $GID -o -s /bin/bash $UNAME

# Set the user (for subsequent commands)
USER $UNAME

# Set the working directory (for subsequent commands)
WORKDIR /home/$UNAME/workspace

# Run when the container launches
CMD ["/bin/bash"]
