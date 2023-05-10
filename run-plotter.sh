#!/bin/bash

python_file=libraries/aircraft-simulator/plotter.py
mlat=applications/utilities/plotters/mlat.txt
real=applications/utilities/plotters/real.txt
filter=applications/utilities/plotters/filter.txt
speed_filter=applications/utilities/plotters/speed_filter.txt
speed_real=applications/utilities/plotters/speed_real.txt
acceleration_filter=applications/utilities/plotters/acceleration_filter.txt
acceleration_real=applications/utilities/plotters/acceleration_real.txt

python3 $python_file $mlat $real $filter $speed_filter $speed_real $acceleration_filter $acceleration_real