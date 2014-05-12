#! /bin/bash

#
# Simple wrapper for stats 
#

set -e

function usage
{
  echo "Usage: $0  source_trace [extra_parameters]"
  echo "  source_trace: Paraver trace; it's ungzipped if needed."
  echo "  parameters:   See complete list of stats help with 'stats --help'"
}

PARAVER_TRACE=`readlink -eqs "${1}"`

# Read and check parameters
if [ $# -lt 1 ]; then
  usage
  exit 1
fi

# Check Dimemas availability

{ command -v ${PARAVER_HOME}/bin/stats > /dev/null && export STATS_ENV="${PARAVER_HOME}/bin/"; } || 
{ command -v stats > /dev/null && export STATS_ENV=""; } || \
{ echo "Unable to find stats. Define \$PARAVER_HOME or place it in \$PATH"; exit 1; }


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

# Append extra parameters if they exist
shift
EXTRA_PARAMETERS=""
while [ -n "$1" ]; do
  EXTRA_PARAMETERS="$EXTRA_PARAMETERS $1"
  shift
done

echo
echo "${STATS_ENV}stats ${PARAVER_TRACE} ${EXTRA_PARAMETERS}"
echo
${STATS_ENV}stats ${PARAVER_TRACE} ${EXTRA_PARAMETERS}
echo



