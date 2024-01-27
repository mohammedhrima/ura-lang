FROM debian:stable

WORKDIR /wcc
COPY . .

RUN apt update -y && apt full-upgrade -y \
    && apt install -y gcc g++ vim man git python3

CMD ["sh", "_setup.sh"]
