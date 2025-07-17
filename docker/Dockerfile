FROM --platform=linux/amd64 debian:bullseye

WORKDIR /src

RUN apt update -y && apt full-upgrade -y && \
    apt install -y gcc g++ vim man git python3 astyle gdb && \
    apt autoremove -y

CMD ["sh", "-c", "tail -f /dev/null"]
