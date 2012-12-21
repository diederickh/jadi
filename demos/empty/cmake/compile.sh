#!/bin/sh
d=${PWD}
bd=${PWD}/build

if [ ! -d ${bd} ] ; then 
    mkdir ${bd}
fi

if [ ! -d ${d}/bin ] ; then 
    mkdir ${d}/bin
fi

cd build
cmake ../ 
make
make install
