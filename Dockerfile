FROM debian:bookworm-slim

LABEL MAINTAINER  "Andreas Nazlidis"
LABEL APP	  "raymann"
LABEL DESCRIPTION "Ray-Tracer"

USER root

ENV DEBIAN_FRONTEND=noninteractive 
RUN apt update -qq  && \
    apt upgrade -y -qq --no-install-recommends \
    ca-certificates build-essential cmake vim python3 git &&   \
    useradd guest

ADD test raymann/test
ADD src raymann/src
ADD scenes raymann/scenes
ADD LICENSE raymann
ADD README.md raymann
ADD CMakeLists.txt raymann
ADD scene.json raymann
ADD config.json raymann

RUN chown -R guest:guest raymann 
USER guest

WORKDIR raymann
RUN cmake -S . -B build -DBUILD_TESTING=OFF && \
    cmake --build build && \
    ./bin/raymann
