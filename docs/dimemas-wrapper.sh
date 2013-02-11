#! /bin/bash

#
# Simple wrapper for Dimemas
#


if [ $# -lt 3 ]; then
  echo "Usage: $0 tracename simulated_tracename dimemas_cfg [extra_parameters]"
  echo "  tracename:        paraver trace"
  echo "  dimemas_cfg:      simulation parameters"
  echo "  simulated_trace:  output of Dimemas; must end with '.prv'"
  echo "  extra_parameters: See complete list of Dimemas help with 'Dimemas -h'"
  exit
fi

set -e

PARAVER_TRACE=${1}
DIMEMAS_CFG=${2}
SIMULATED_PARAVER_TRACE=${3}

echo

if [ ! -f ${PARAVER_TRACE} ]; then
  echo "Error: ${PARAVER_TRACE} not found!"
  exit
elif [ ! -f ${DIMEMAS_CFG} ]; then
  echo "Error: ${DIMEMAS_CFG} not found!"
  exit
fi

# Get tracename, without extensions
TRACENAME=$(echo "$PARAVER_TRACE" | sed 's/\.[^\.]*$//')
EXTENSION=$(echo "$PARAVER_TRACE" | sed 's/^.*\.//')

# Is gzipped?
if [[ ${EXTENSION} = "gz" ]]; then
  echo
  echo -n "Decompressing $PARAVER_TRACE trace..."
  gunzip ${PARAVER_TRACE}
  TRACENAME=$(echo "${TRACENAME}" | sed 's/\.[^\.]*$//')
  PARAVER_TRACE=${TRACENAME}.prv
  echo "...Done!"
fi

DIMEMAS_TRACE=${TRACENAME}.trf

# Adapt Dimemas CFG with new trace name
DIMEMAS_CFG_NAME=$(echo "$DIMEMAS_CFG" | sed 's/\.[^\.]*$//')
OLD_DIMEMAS_TRACENAME=`grep "mapping information" ${DIMEMAS_CFG} | grep ".trf" | awk -F'"' {'print $4'}`
NEW_DIMEMAS_TRACENAME=`basename ${DIMEMAS_TRACE}`
sed 's/${OLD_DIMEMAS_TRACENAME}/${NEW_DIMEMAS_TRACENAME}/g' ${DIMEMAS_CFG} > ${DIMEMAS_CFG_NAME}.mod.cfg
DIMEMAS_CFG=${DIMEMAS_CFG_NAME}.mod.cfg

# Append extra parameters if they exist
shift
shift
shift
FINAL_PARAMETERS=""
while [ -n "$1" ]; do
  FINAL_PARAMETERS="$FINAL_PARAMETERS $1"
  shift
done

# Change directory to see .trf
DIMEMAS_TRACE_DIR=`dirname ${DIMEMAS_TRACE}`/
pushd .
cd ${DIMEMAS_TRACE_DIR}


# Translate from prv to trf
echo
echo "prv2dim ${PARAVER_TRACE} ${DIMEMAS_TRACE}"
echo
PRV2DIM_LOG=${DIMEMAS_TRACE_DIR}/prv2dim.log
prv2dim ${PARAVER_TRACE} ${DIMEMAS_TRACE} &> ${PRV2DIM_LOG}
echo

# Simulate
# parameter -S 32K fixed by default
echo
echo "Dimemas -S 32K -p ${SIMULATED_PARAVER_TRACE} ${DIMEMAS_CFG} ${FINAL_PARAMETERS}"
echo
Dimemas -S 32K -p ${SIMULATED_PARAVER_TRACE} ${DIMEMAS_CFG} ${FINAL_PARAMETERS} 
echo

popd

