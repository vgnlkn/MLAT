#!/bin/bash

python_file=libraries/aircraft-simulator/plotter.py
mlat=applications/utilities/plotters/mlat.txt
real=applications/utilities/plotters/real.txt
filter=applications/utilities/plotters/filter.txt
filter_standard=applications/utilities/plotters/filter_standard.txt
speed_filter=applications/utilities/plotters/speed_filter.txt
speed_real=applications/utilities/plotters/speed_real.txt
speed_standard_filter=applications/utilities/plotters/speed_filter_standard.txt
acceleration_filter=applications/utilities/plotters/acceleration_filter.txt
acceleration_standard_filter=applications/utilities/plotters/acceleration_filter_standard.txt
acceleration_real=applications/utilities/plotters/acceleration_real.txt

python3 $python_file $mlat $real $filter $filter_standard $speed_filter $speed_real $speed_standard_filter $acceleration_filter $acceleration_real $acceleration_standard_filter