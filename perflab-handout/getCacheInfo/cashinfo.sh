#!/bin/bash

for i in  {1..3}
do
    echo "L$i Cache Size: "
    cat /sys/devices/system/cpu/cpu0/cache/index$i/size

    echo "L$i Cache ways_of_associativity: "
    cat /sys/devices/system/cpu/cpu0/cache/index$i/ways_of_associativity
    
    echo "L$i Cache coherency_line_size: "
    cat /sys/devices/system/cpu/cpu0/cache/index$i/coherency_line_size
    echo
done