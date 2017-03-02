#! /bin/bash

################################################################################
# This script:
#
#    1. Calls sphinx to transform xxx.rst to xxx.1.gz
#    2. Copy to wxparaver/docs/help_contents/man/
#
################################################################################

pushd . &> /dev/null

################################################################################
# 1. sphinx
################################################################################
make man 

SPHINX_OUTPUT=paraver-toolset.1

cd build/man

if ! [ -e "${SPHINX_OUTPUT}" ]; then
  echo "[ERROR!!!]: no documentation generated in ./build/man"
  popd &> /dev/null
  exit 1
fi


################################################################################
# 2. Copy to wxparaver/docs/help_contents/man/
################################################################################
gzip -cf ${SPHINX_OUTPUT} > ../../../../install/man/${SPHINX_OUTPUT}.gz

################################################################################

popd &> /dev/null

