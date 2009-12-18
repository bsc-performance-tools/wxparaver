#!/bin/sh

echo "aclocal"
aclocal -I m4
echo "autoheader"
autoheader
echo "automake -a -c"
automake -a -c
echo "autoconf"
autoconf

