FROM debian:stable

WORKDIR /code

RUN apt update -y && apt full-upgrade -y && \
    apt install -y gcc g++ vim man git python3 \
    gdb && \
    apt clean && rm -rf /var/lib/apt/lists/*

COPY . .

CMD sh -c "tail -f /dev/null"