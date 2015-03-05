#! /bin/bash

if [ $# -ne 1 ]; then
  echo; echo "  Usage: $0 <new-version-tag>"
  echo; echo "    Substitutes version tag in configure.ac and paravermain.h"
        echo "    Before changes, they're copied to configure.ac.oldvers and paravermain.h.oldvers"
        echo "    No quotes allowed"
  echo; echo "  Example: $0 6.6.6"
  echo
  exit 1
fi

VERSION=${1}

if [ -f "configure.ac" ]; then
  cp configure.ac configure.ac.oldvers
  export TAG_CONF=AC_INIT
  awk -F ',' \
    '$0  ~ /^'${TAG_CONF}'/ { print $1 ", '${VERSION}'," $3; } ; \
     $0 !~ /^'${TAG_CONF}'/ { print $0; }' configure.ac > configure.ac.newchgvers
  mv configure.ac.newchgvers configure.ac
  echo; echo "[UPDATE-VERSION] Changes in configure.ac:"
  diff -s configure.ac.oldvers configure.ac
else
  echo; echo "[UPDATE-VERSION] ERROR: configure.ac not found!"
fi

if [ -f "configure.ac" ]; then
  cp paravermain.h paravermain.h.oldvers
  export TAG_CONF=VERSION
  awk -F ' ' \
     '$0  ~ /define '${TAG_CONF}'/ { print $1 " '${TAG_CONF}' \"'${VERSION}'\"" ; } ;  \
      $0 !~ /define '${TAG_CONF}'/ { print $0; }'  paravermain.h > paravermain.h.newchgvers
  mv paravermain.h.newchgvers paravermain.h
  echo; echo "[UPDATE-VERSION] Changes in paravermain.h:"
  diff -s paravermain.h.oldvers paravermain.h
else
  echo; echo "[UPDATE-VERSION] ERROR: paravermain.h not found!"
fi

