# Kaspa FPGA Miner

This project implements a hardware-accelerated **Kaspa cryptocurrency miner** using **High-Level Synthesis (HLS)**. The miner is written in C++ and designed to be synthesized for FPGA platforms such as Xilinx Zynq-7000 using Vivado HLS. It includes a testbench for functional simulation, a Docker development environment, and HLS synthesizable modules for the HeavyHash algorithm.

---

## Overview

Kaspa uses a unique proof-of-work algorithm known as **HeavyHash**. This project implements a parallelized and hardware-efficient version of that algorithm using:

- Custom deterministic PRNG (`xoshiro256++`)
- Lightweight BLAKE3 stub (for demonstration or replacement)
- Matrix-based hash mixing
- AXI-compatible FPGA interfacing

---

## Architecture

![Kaspa Miner Architecture](kaspa_miner_architecture.png)

---

## Project Structure

| File | Purpose |
|------|---------|
| `kaspa_miner.cpp` | Top-level miner kernel for HLS |
| `heavyhash_hls.cpp` | Core HeavyHash implementation |
| `blake3_hls.h` | Placeholder BLAKE3 hash (replace with real hash for production) |
| `xoshiro256pp.h` | High-speed PRNG used for matrix generation |
| `kaspa_miner.tcl` | Vivado HLS build script |
| `sim_ap_int.h` | CPU-simulation version of HLS integer types |
| `heavyhash_testbench.cpp` | Testbench for validating functionality |
| `Dockerfile` / `run.sh` | Portable build environment setup |

---
## Architecture

![image](https://github.com/user-attachments/assets/048c26b1-4eeb-4021-ac81-271015a93f96)

## Getting Started

### Prerequisites

- [Xilinx Vivado HLS 2022.1](https://www.xilinx.com/)
- Docker (for portable development)
- A compatible FPGA board (e.g., Zynq-7000 series)

### Build Instructions

```bash
# Launch development container
./run.sh

# Inside container:
vivado_hls -f build_kaspa_miner.tcl
```

---

## Test

A basic testbench is provided:

```bash
g++ heavyhash_testbench.cpp -o test && ./test
```

Expected digest output should match known Kaspa hash outputs. For more robust validation, integrate a reference Kaspa node or hash oracle.

---

## Notes

- This project includes a simplified BLAKE3 hash function for HLS testing. In production, integrate a verified BLAKE3 core or pre-process the input on CPU.
- Real nonce sweeping and mining requires persistent execution and a full networking stack.

