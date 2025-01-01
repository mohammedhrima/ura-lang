FROM debian:bullseye-slim

WORKDIR /code

RUN apt update -y && apt full-upgrade -y && \
    apt install -y gcc g++ vim man git python3 && \
    apt clean && rm -rf /var/lib/apt/lists/*

COPY . .

CMD ["sh", "setup.sh"]
