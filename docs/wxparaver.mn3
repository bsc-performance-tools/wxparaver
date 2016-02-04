#! /bin/bash

REAL_PATH=`readlink -f "${0}"`
PARAVER_HOME="$( cd "$( dirname "$REAL_PATH" )" && pwd )"
export PARAVER_HOME=${PARAVER_HOME%/bin}

#if [ -z "${PARAVER_HOME}" ] ; then
#  echo "Please set \$PARAVER_HOME to the wxparaver installation root directory"
#  exit 1

if ! [ -d "${PARAVER_HOME}/lib/paraver-kernel" ] ; then
  echo "paraver-kernel libraries not found in \$PARAVER_HOME/lib/paraver-kernel/"
  exit 1

##elif ! [ -d ${PARAVER_HOME}/lib/wxparaver ] ; then
##  echo "wxparaver libraries not found in \$PARAVER_HOME/lib/wxparaver/"
##  exit 1

elif ! [ -e "${PARAVER_HOME}/bin/wxparaver.bin" ] ; then
  echo "wxparaver binary not found in \$PARAVER_HOME/bin/"
  exit 1
  
fi

LD_LIBRARY_PATH=$PARAVER_HOME/lib/ptools_common_files/:/apps/BOOST/1.52.0/lib:$PARAVER_HOME/lib/paraver-kernel:/apps/CEPBATOOLS/deps/wxpropgrid/lib/:$PARAVER_HOME/lib/wxparaver:$LD_LIBRARY_PATH $PARAVER_HOME/bin/wxparaver.bin "$@"
	
