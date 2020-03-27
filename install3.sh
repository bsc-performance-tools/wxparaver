#!/bin/sh

./configure --with-boost-libdir=/usr/lib/x86_64-linux-gnu/ --enable-openmp            && make clean && make -j && sudo make install && echo "INSTALL SUCCESSFUL" 
