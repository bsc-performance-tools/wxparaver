#!/usr/bin/env sh

#
# Simple wrapper for prvstats 
#

set -e

usage()
{
  echo "Usage: $0  source_trace [extra_parameters]"
  echo "  source_trace: Paraver trace; it's ungzipped if needed."
  echo "  parameters:   See complete list of prvstats help with 'prvstats --help'"
}

PARAVER_TRACE=`readlink -eqs "${1}"`

# Read and check parameters
if [ $# -lt 1 ]; then
  usage
  exit 1
fi

# Check prvstats availability

{ command -v ${PARAVER_HOME}/bin/prvstats > /dev/null && export PRVSTATS_ENV="${PARAVER_HOME}/bin/"; } || 
{ command -v prvstats > /dev/null && export PRVSTATS_ENV=""; } || \
{ echo "Unable to find prvstats. Define \$PARAVER_HOME or place it in \$PATH"; exit 1; }


# Get tracename, without extensions
TRACENAME=$(echo "$PARAVER_TRACE" | sed 's/\.[^\.]*$//')
EXTENSION=$(echo "$PARAVER_TRACE" | sed 's/^.*\.//')

# Is gzipped?
if [ ${EXTENSION} = "gz" ]; then
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
echo "${PRVSTATS_ENV}prvstats ${PARAVER_TRACE} ${EXTRA_PARAMETERS}"
echo
${PRVSTATS_ENV}prvstats ${PARAVER_TRACE} ${EXTRA_PARAMETERS}
echo



