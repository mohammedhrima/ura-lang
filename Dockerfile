FROM debian:stable

WORKDIR /app
COPY ./compiler .
RUN apt update -y && apt install -y gcc
CMD sh
