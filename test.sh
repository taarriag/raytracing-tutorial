#!/bin/bash
make clean && make && ./out/main >> out/image.ppm && open out/image.ppm
