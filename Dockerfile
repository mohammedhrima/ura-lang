FROM --platform=linux/amd64 ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive \
    TZ=UTC \
    LANG=C.UTF-8 \
    ASAN_OPTIONS=detect_leaks=0:abort_on_error=0:halt_on_error=1

RUN apt-get update && apt-get install -y --no-install-recommends \
        llvm-14 \
        llvm-14-dev \
        llvm-14-tools \
        clang-14 \
        clang-format-14 \
        lld-14 \
        build-essential \
        git \
        python3 \
        gdb \
        valgrind \
        curl \
        ca-certificates \
        bash-completion \
    && rm -rf /var/lib/apt/lists/*

# Point unversioned tools at the -14 variants (matches what install() does on Linux)
RUN for t in clang clang++ llc llvm-config clang-format lld ld.lld; do \
        if [ -x "/usr/bin/${t}-14" ]; then \
            ln -sf "/usr/bin/${t}-14" "/usr/local/bin/${t}"; \
        fi; \
    done

WORKDIR /ura-lang

# anvil is NOT baked into the image. It is built into the mounted volume
# (config/anvil/build/linux/anvil) so source edits on the host are visible
# both ways. The top-level Makefile ensures the binary exists before
# `make shell`. PATH below picks it up.
RUN printf '%s\n' \
        '[[ $- == *i* ]] || return' \
        'alias clean="clear && printf '\''\e[3J'\''"' \
        'export PATH=/ura-lang/config/anvil/build/linux:$PATH:/ura-lang/build' \
        'export URA_LIB=/ura-lang/src/ura-lib' \
        'export TERM=xterm-256color' \
        'PS1='\''${debian_chroot:+($debian_chroot)}\[\033[01;32m\]\u@\h\[\033[00m\]:\[\033[01;34m\]\w\[\033[00m\]\$ '\''' \
        'alias ls="ls --color=auto"' \
        'alias grep="grep --color=auto"' \
        'alias ll="ls -alF --color=auto"' \
        'alias la="ls -A --color=auto"' \
        'alias l="ls -CF --color=auto"' \
        >> /root/.bashrc

CMD ["bash"]
