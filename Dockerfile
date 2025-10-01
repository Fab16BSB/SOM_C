FROM ubuntu:20.04

LABEL author="Fab.16" \
	  version="1.0.0" \
	  description="Image de la version C de SOM"

RUN apt-get update && \
    apt-get install -y build-essential


WORKDIR /app

COPY . .

RUN gcc -o som main.c -lm

CMD ["./som"]
