open_project kaspa_miner
set_top kaspa_miner

add_files kaspa_miner.cpp
add_files heavyhash_hls.cpp
add_files xoshiro256pp.h
add_files blake3_hls.h

open_solution "solution1"
set_part {xc7z020clg400-1}
create_clock -period 10 -name default

csynth_design
export_design -format ip_catalog
