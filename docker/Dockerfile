FROM --platform=linux/amd64 debian:bullseye

WORKDIR /src

RUN apt-get update -y && apt-get full-upgrade -y && \
    apt-get install -y gcc g++ vim man git python3 astyle gdb && \
    apt-get autoremove -y

CMD ["sh", "-c", "tail -f /dev/null"]
