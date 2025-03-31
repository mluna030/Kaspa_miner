#!/bin/bash

docker run --rm -it \
  -v $(pwd):/workspace \
  -v /opt/Xilinx:/opt/Xilinx \
  --name kaspa_hls_dev \
  ubuntu:20.04 bash
