#!/usr/bin/env sh

################################################################################
# This script:
#
#    1. Calls sphinx to transform xxx.rst to xxx.html.
#    2. Downgrades the resulting HTML to let wxparaver browser widget (2.8.12) 
#       open it.
#    3. Rewrites both wxparaver/docs/help_contents/install/${DIR}/{html,man}.
#
################################################################################

PWD=`pwd`

################################################################################
# 1. sphinx
################################################################################
#HTML_TYPE=html
HTML_TYPE=singlehtml

make $HTML_TYPE

SPHINX_OUTPUT=paraver_toolset.quick_reference.html

cd build/$HTML_TYPE
if ! [ -e "${SPHINX_OUTPUT}" ]; then
  echo "[ERROR!!!]: no .html documentation found in ./build/$HTML_TYPE"
  cd ${PWD}
  exit 1
fi


################################################################################
# 2. Downgrade html:
#    * Some minor deletes:
#      + <a class="headerlink".*<\/a>
#      + <script type=.*<\/script>
#      + <link rel="stylesheet".*\/>
#
#    * Some changes:
#      + Link and anchor added; also 'name' property instead of 'id'.
#        WARNING: 'name' property is currently deprecated and not supported in 
#        HTML5, so updgrading wxparaver widget may imply keeping it as 'id' (it
#        means substitute every 'name' in the following sed command with an 'id'.
#        -  orig:  <div class="section" id=" xxx 
#        -  final: <div class="section"> <a name=" xxx <\/a>
#
#    * Some addings:
#      + Link from section titles to index.
#        -  <a name="my-contents"><\/a>
#        -  <a class="toc-backref" href="#my-contents"
# TODO:
#    * Final cleaning:
#      + Delete unused css data
################################################################################
sed 's/<a class="headerlink".*<\/a>//; s/<script type=.*<\/script>//; s/<link rel="stylesheet".*\/>//' "${SPHINX_OUTPUT}" | \
  sed 's/<div class="section" id="/<div class="section"> <a name="/' | \
  sed '/^<div class="section"> <a name=/ s/$/<\/a>/' | \
  sed 's/<span id="my-contents"><\/span>/<a name="my-contents"><\/a>/' | \
  sed 's/<a class="toc-backref" href=".*"/<a class="toc-backref" href="#my-contents"/' > index.html

rm ${SPHINX_OUTPUT}

################################################################################
# 3. Copy to wxparaver/docs/help_contents/install/
################################################################################
cp -ap ./index.html ../../../../install/html/2.paraver_toolset/
cp -ap ./_images    ../../../../install/html/2.paraver_toolset/

################################################################################

cd ${PWD}

