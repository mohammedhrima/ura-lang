FROM debian:latest

WORKDIR /code
COPY . .

RUN apt update -y && apt full-upgrade -y && apt install -y gcc g++ vim man git python3

CMD ["sh", "setup.sh"]