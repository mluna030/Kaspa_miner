FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    wget curl build-essential cmake python3 python3-pip git vim sudo libncurses5 libx11-6 \
    && rm -rf /var/lib/apt/lists/*

ENV PATH="/opt/Xilinx/Vitis_HLS/2022.1/bin:$PATH"

WORKDIR /workspace
