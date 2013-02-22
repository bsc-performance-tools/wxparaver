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

#DIMEMAS_HOME=

{ command -v ${DIMEMAS_HOME}/bin/prv2dim > /dev/null && export DIMEMAS_ENV="${DIMEMAS_HOME}/bin/"; } || \
{ command -v prv2dim > /dev/null && export DIMEMAS_ENV=""; } || \
{ echo "Unable to find Dimemas. Define \$DIMEMAS_HOME or place it in \$PATH"; exit 1; }

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
EXTRA_PARAMETERS=""
while [ -n "$1" ]; do
  EXTRA_PARAMETERS="$EXTRA_PARAMETERS $1"
  shift
done

# Change directory to see .trf
DIMEMAS_TRACE_DIR=`dirname ${DIMEMAS_TRACE}`/
pushd .
cd ${DIMEMAS_TRACE_DIR}


# Translate from prv to trf
echo
echo "${DIMEMAS_ENV}prv2dim ${PARAVER_TRACE} ${DIMEMAS_TRACE} &> ${PRV2DIM_LOG}"
echo
PRV2DIM_LOG=${DIMEMAS_TRACE_DIR}/prv2dim.log
${DIMEMAS_ENV}prv2dim ${PARAVER_TRACE} ${DIMEMAS_TRACE} &> ${PRV2DIM_LOG}
echo

# Simulate
# parameter -S 32K fixed by default
echo
echo "${DIMEMAS_ENV}Dimemas ${EXTRA_PARAMETERS} -S 32K -p ${SIMULATED_PARAVER_TRACE} ${DIMEMAS_CFG}"
echo
${DIMEMAS_ENV}Dimemas ${EXTRA_PARAMETERS} -S 32K -p ${SIMULATED_PARAVER_TRACE} ${DIMEMAS_CFG} 
echo

popd

