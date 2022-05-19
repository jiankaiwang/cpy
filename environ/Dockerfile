FROM ubuntu:18.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y \
        build-essential \
        python3.6 \
        python3.6-dev \
        cython \
        python3-pip \        
        cmake && \
    apt-get autoremove -y

RUN rm -rf /usr/bin/python && \
    ln -s /usr/bin/python3 /usr/bin/python

WORKDIR /cpy

CMD [ "bash" ]

