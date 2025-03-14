#!/bin/bash

export BOARD="B-U585I-IOT02A"
# export BOARD="NUCLEO-G071RB"

export QC_PATH=$(pwd)

clear

echo $BOARD

# python3 $QC_PATH/sign.py --board-name=$BOARD

python3 $QC_PATH/sign.py --board-name=$BOARD --bin-file="../../../${BOARD}_FreeRTOS/network_data.bin" --output-file="../../../${BOARD}_FreeRTOS/signed_network_data.bin" --type="WEIGHTS"

