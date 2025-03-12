FROM debian:stable

WORKDIR /src

RUN apt update -y && apt full-upgrade -y && \
    apt install -y gcc g++ vim man git python3 astyle \
    gdb && apt autoremove -y

# RUN chmod -R 777 /src /import /random /tests /root/.bashrc

CMD ["sh", "-c", "tail -f /dev/null"]
