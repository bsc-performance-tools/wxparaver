#! /bin/bash

#
# Simple wrapper for Dimemas
#

set -e

function usage
{
  echo "Usage: $0  source_trace  dimemas_cfg  output_trace  reuse_dimemas_trace [extra_parameters] [-n]"
  echo "  source_trace:        Paraver trace"
  echo "  dimemas_cfg:         Simulation parameters"
  echo "  output_trace:        Output trace of Dimemas; must end with '.prv'"
  echo "  reuse_dimemas_trace: 0 -> don't reuse, rerun prv2dim"
  echo "                       1 -> reuse, don't rerun prv2dim"
  echo "  extra_parameters:    See complete list of Dimemas help with 'Dimemas -h'"
  echo "  -n:                  prv2dim -n parameter => no generate initial idle states"
}


# Read and check parameters
if [ $# -lt 4 ]; then
  usage
  exit 1
fi

#PARAVER_TRACE=${1}
PARAVER_TRACE=`readlink -eqs "${1}"`
DIMEMAS_CFG=${2}
OUTPUT_PARAVER_TRACE=${3}
DIMEMAS_REUSE_TRACE=${4}


if [[ ${DIMEMAS_REUSE_TRACE} != "0"  && ${DIMEMAS_REUSE_TRACE} != "1" ]]; then
  usage
  exit 1
fi

echo "===============================================================================" 

# Check Dimemas availability

#DIMEMAS_HOME=

{ command -v ${DIMEMAS_HOME}/bin/prv2dim > /dev/null && export DIMEMAS_ENV="${DIMEMAS_HOME}/bin/"; } || \
{ command -v prv2dim > /dev/null && export DIMEMAS_ENV=""; } || \
{ echo "[ERR] Unable to find prv2dim or Dimemas binaries. Please check installation, define \$DIMEMAS_HOME or place it in \$PATH"; exit 1; }


# Get tracename, without extensions
TRACENAME=$(echo "$PARAVER_TRACE" | sed "s/\.[^\.]*$//")
EXTENSION=$(echo "$PARAVER_TRACE" | sed "s/^.*\.//")

#Is gzipped?
if [[ ${EXTENSION} = "gz" ]]; then
  echo
  echo -n "[MSG] Decompressing $PARAVER_TRACE trace..."
  gunzip ${PARAVER_TRACE}
  TRACENAME=$(echo "${TRACENAME}" | sed "s/\.[^\.]*$//")
  PARAVER_TRACE=${TRACENAME}.prv
  echo "...Done!"
fi

DIMEMAS_TRACE=${TRACENAME}.dim

# Adapt Dimemas CFG with new trace name
DIMEMAS_CFG_NAME=$(echo "$DIMEMAS_CFG" | sed "s/\.[^\.]*$//")

DIMEMAS_COPY_CFG_NAME=`basename ${DIMEMAS_CFG_NAME}`
OLD_DIMEMAS_TRACENAME=`grep "mapping information" ${DIMEMAS_CFG} | grep ".dim" | awk -F'"' {'print $4'}`
NEW_DIMEMAS_TRACENAME=`basename ${DIMEMAS_TRACE}`
DIMEMAS_CFG_PATH=`dirname ${DIMEMAS_TRACE}`

# DETECTION OF DIMEMAS VERSION
# Transforms "Dimemas version x.x.x" to a number xxx
DIMEMAS_VERSION=`Dimemas -h | grep version | head -n 1 | awk -F ' ' '{ print $3; };' | sed 's/\.//g' | sed 's/[a-zA-Z]//g'`
DIMEMAS_MINIMUM_VERSION=512

if [ "$DIMEMAS_VERSION" -gt "$DIMEMAS_MINIMUM_VERSION" ]; then
  DIMEMAS_DIM_PARAMETER=1
else
  DIMEMAS_DIM_PARAMETER=0

  # Am I executing old Dimemas with new CFG?
  SHEBANG_OLD_CFG=`grep "SDDFA" ${DIMEMAS_CFG}`
  if [[ ${SHEBANG_OLD_CFG} != "SDDFA" ]]; then
    echo "[ERR] Trying to simulate using old version of Dimemas with new incompatible Dimemas cfg"
    echo "[ERR] Please update Dimemas package."
    exit 1 
  fi

  cat ${DIMEMAS_CFG} | sed "s/${OLD_DIMEMAS_TRACENAME}/${NEW_DIMEMAS_TRACENAME}/g" > ${DIMEMAS_CFG_PATH}/${DIMEMAS_COPY_CFG_NAME}.mod.cfg
  DIMEMAS_CFG=${DIMEMAS_CFG_PATH}/${DIMEMAS_COPY_CFG_NAME}.mod.cfg
fi

# Append extra parameters if they exist
shift
shift
shift
shift
EXTRA_PARAMETERS=""
PRV2DIM_N=""
while [ -n "$1" ]; do
  if [[ ${1} == "-n" ]]; then # caution! this works because no -n parameters exists in Dimemas
    PRV2DIM_N="-n"
  else
    EXTRA_PARAMETERS="$EXTRA_PARAMETERS $1"
  fi
  shift
done

# Change directory to see .dim
DIMEMAS_TRACE_DIR=`dirname ${DIMEMAS_TRACE}`/
pushd . > /dev/null
cd ${DIMEMAS_TRACE_DIR}


# Translate from .prv to .dim
if [[ ${DIMEMAS_REUSE_TRACE} = "0" || \
      ${DIMEMAS_REUSE_TRACE} = "1" && ! -f ${DIMEMAS_TRACE} ]]; then

  if [[ ${DIMEMAS_REUSE_TRACE} = "1" ]]; then
    echo
    echo "[WARN] Unable to find ${DIMEMAS_TRACE}"
    echo "[WARN] Generating it."
  fi

  PRV2DIM_LOG=${DIMEMAS_TRACE_DIR}/prv2dim.log
  echo
  echo "[COM] ${DIMEMAS_ENV}prv2dim ${PRV2DIM_N} ${PARAVER_TRACE} ${DIMEMAS_TRACE} &> ${PRV2DIM_LOG}"
  echo
  ${DIMEMAS_ENV}prv2dim ${PRV2DIM_N} ${PARAVER_TRACE} ${DIMEMAS_TRACE} &> ${PRV2DIM_LOG}
  echo
fi

# Simulate
# parameter -S 32K fixed by default
if [[ ${DIMEMAS_DIM_PARAMETER} = "0" ]]; then
  echo
  echo "[COM] ${DIMEMAS_ENV}Dimemas ${EXTRA_PARAMETERS} -S 32K -p ${OUTPUT_PARAVER_TRACE} ${DIMEMAS_CFG}"
  echo
  ${DIMEMAS_ENV}Dimemas ${EXTRA_PARAMETERS} -S 32K -p ${OUTPUT_PARAVER_TRACE} ${DIMEMAS_CFG} 
else
  echo
  echo "[COM] ${DIMEMAS_ENV}Dimemas --dim ${NEW_DIMEMAS_TRACENAME} ${EXTRA_PARAMETERS} -S 32K -p ${OUTPUT_PARAVER_TRACE} ${DIMEMAS_CFG}"
  echo
  ${DIMEMAS_ENV}Dimemas --dim ${NEW_DIMEMAS_TRACENAME} ${EXTRA_PARAMETERS} -S 32K -p ${OUTPUT_PARAVER_TRACE} ${DIMEMAS_CFG} 
fi

echo "===============================================================================" 

popd > /dev/null

