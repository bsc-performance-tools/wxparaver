#!/bin/sh
#PARAVER_HOME=/opt/cepbatools/wxparaver
PARAVER_HOME=$PWD

if [ -z "${PARAVER_HOME}" ] ; then
  echo "Please set \$PARAVER_HOME to the wxparaver installation root directory"
  exit 1

elif ! [ -d ${PARAVER_HOME}/lib/paraver-kernel ] ; then
  echo "paraver-kernel libraries not found in \$PARAVER_HOME/lib/paraver-kernel/"
  exit 1

elif ! [ -d ${PARAVER_HOME}/lib/wxparaver ] ; then
  echo "wxparaver libraries not found in \$PARAVER_HOME/lib/wxparaver/"
  exit 1

elif ! [ -e ${PARAVER_HOME}/bin/wxparaver.bin ] ; then
  echo "wxparaver binary not found in \$PARAVER_HOME/bin/"
  exit 1
  
fi

LD_LIBRARY_PATH=$PARAVER_HOME/lib/paraver-kernel:$PARAVER_HOME/lib/wxparaver:$LD_LIBRARY_PATH $PARAVER_HOME/bin/wxparaver.bin "$@"
	
