FROM debian:bookworm-slim

LABEL MAINTAINER  "Andreas Nazlidis"
LABEL APP	  "raymann"
LABEL DESCRIPTION "Ray-Tracer"

USER root

ENV DEBIAN_FRONTEND=noninteractive 
RUN apt update -qq  && \
    apt upgrade -y -qq --no-install-recommends build-essential cmake && \
    useradd guest

ADD test raymann/test
ADD src raymann/src
ADD scenes raymann/scenes
ADD LICENSE raymann
ADD README.md raymann
ADD CMakeLists.txt raymann

RUN chown -R guest:guest raymann 
USER guest

WORKDIR raymann
RUN mkdir -p build && cd build && \
    cmake .. -DBUILD_TESTING=OFF  && \
    make && cd .. && ./bin/raymann
