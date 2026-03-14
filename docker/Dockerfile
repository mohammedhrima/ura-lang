FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# sudo is needed by setup.sh — grant it to root without password
RUN apt-get update -qq && apt-get install -y sudo && \
    echo "root ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers

WORKDIR /app
COPY . .

# Run the full setup (installs llvm-14, clang-14, uncrustify, creates symlinks)
RUN bash setup.sh

# Build the Ura compiler, then run tests — all in one shell so config.sh functions are available
RUN bash -c "source /app/config.sh && build && tests"
