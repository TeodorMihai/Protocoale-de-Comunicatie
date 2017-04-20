#!/bin/bash

make clean
make

./link_emulator/link
./server
./client