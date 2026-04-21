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

WORKDIR /workspace

# Build anvil and avatar at image time, install both to PATH. Sources are
# baked in; the mounted /workspace copy stays available for rebuilds.
COPY config/anvil /opt/anvil
RUN make -C /opt/anvil \
    && install -m 755 /opt/anvil/anvil /usr/local/bin/anvil \
    && rm -rf /opt/anvil

COPY src/tools/avatar /opt/avatar
RUN make -C /opt/avatar \
    && install -m 755 /opt/avatar/avatar /usr/local/bin/avatar \
    && rm -rf /opt/avatar

# Minimal interactive shell setup: a friendly alias, no config.sh sourcing
# (config.sh is gone — anvil replaces it).
RUN printf '%s\n' \
        '[[ $- == *i* ]] || return' \
        'alias clean="clear && printf '\''\e[3J'\''"' \
        >> /root/.bashrc

CMD ["bash"]
