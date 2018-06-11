#! /bin/bash
#cd .. ; make html ; cd source ; firefox ../build/html/index.html & 
pushd .
#cd .. 
make html ; make singlehtml ; make man 




# browse: firefox
cd source
#firefox ../build/html/paraver_toolset.quick_reference.html &
firefox ../build/singlehtml/paraver_toolset.quick_reference.html &


# install in /opt/ to browse in wxparaver
rm -rf /opt/bsctools/wxparaver/devel/share/doc/wxparaver_help/2.paraver_toolset
mkdir -p /opt/bsctools/wxparaver/devel/share/doc/wxparaver_help/2.paraver_toolset

cd ../build/
cp -ap ./singlehtml/* /opt/bsctools/wxparaver/devel/share/doc/wxparaver_help/2.paraver_toolset/
mv /opt/bsctools/wxparaver/devel/share/doc/wxparaver_help/2.paraver_toolset/paraver_toolset.quick_reference.html \
   /opt/bsctools/wxparaver/devel/share/doc/wxparaver_help/2.paraver_toolset/index.html

cd /opt/bsctools/wxparaver/devel/share/doc/wxparaver_help/2.paraver_toolset

# clear some html lines: headerlink, script
sed 's/<a class="headerlink".*<\/a>//; s/<script type=.*<\/script>//; s/<link rel="stylesheet".*\/>//' index.html | \
  sed 's/<div class="section" id="/<div class="section"> <a name="/' | \
  sed '/^<div class="section"> <a name=/ s/$/<\/a>/' | \
  sed 's/<span id="my-contents"><\/span>/<a name="my-contents"><\/a>/' | \
  sed 's/<a class="toc-backref" href=".*"/<a class="toc-backref" href="#my-contents"/' > index.clean.html
#  sed 's/" id=".*"/" name="my-contents"/' > index.clean.html

# Following "sed" version only differs in changing 'id' property with 'name' (not supported in html5)
# future documentation may need this change, or maybe two version, one for wxparaver embedded browser and external browser
#
#sed 's/<a class="headerlink".*<\/a>//; s/<script type=.*<\/script>//; s/<link rel="stylesheet".*\/>//' index.html | \
#  sed 's/<div class="section" id="/<div class="section"> <a id="/' | \
#  sed '/^<div class="section"> <a id=/ s/$/<\/a>/' > index.clean.html

mv index.html index.old.html
mv index.clean.html index.html
#diff index.old.html index.html


#firefox index.html &
# browse: wxparaver
#/opt/bsctools/wxparaver/devel/bin/wxparaver &
#wxparaver &
popd

