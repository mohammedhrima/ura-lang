FROM debian:stable

WORKDIR /app
COPY ./compiler .
RUN apt update -y && apt upgrade -y && apt install -y gcc && apt install g++ -y

CMD ["tail", "-f", "/dev/null"]