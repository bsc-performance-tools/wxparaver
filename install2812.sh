#!/bin/sh

./configure --with-boost-libdir=/usr/lib/x86_64-linux-gnu/ --enable-openmp --with-wxpropgrid-name=wxcode_gtk2u_propgrid-2.8 --with-wx-config=/home/mmoreno/wxWidgets/latest/bin/wx-config --with-wxpropgrid-dir=/home/mmoreno/propgrid/latest            && make clean && make -j && sudo make install && echo "INSTALL SUCCESSFUL" && echo "Run with wx2.8.12paraver !!!"
