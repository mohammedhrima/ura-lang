FROM debian:stable

ENV DEBIAN_FRONTEND=noninteractive

WORKDIR /src

RUN apt-get update -y && apt-get full-upgrade -y && \
   apt-get install -y gcc g++  && \
   apt-get install -y vim man git python3  && \
   apt-get install -y astyle clang llvm llvm-dev && \
   apt-get install -y gdb make  && \
   apt-get autoremove -y && apt-get clean

CMD ["tail", "-f", "/dev/null"]
