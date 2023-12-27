FROM debian:stable

WORKDIR /app
COPY ./compiler/* .
RUN apt update -y && apt upgrade -y && apt install -y gcc && apt install g++ -y

CMD ["echo", ">>", "~/.bashrc"]
CMD ["/bin/bash", "-c", 'echo "alias run=\"./_run.sh\"" >> ~/.bashrc && /bin/bash']
CMD ["/bin/bash", "-c", 'echo "alias tests=\"./_tests.sh\"" >> ~/.bashrc && /bin/bash']
CMD ["/bin/bash", "-c", 'echo "alias copy=\"./_copy.sh\"" >> ~/.bashrc && /bin/bash']
CMD ["echo", ">>", "~/.bashrc"]
CMD ["source", "~/.bashrc"]

CMD ["tail", "-f", "/dev/null"]