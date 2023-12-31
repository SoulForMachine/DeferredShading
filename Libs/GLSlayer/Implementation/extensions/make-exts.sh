#!/bin/sh
echo Generating source for loading core functionality and extensions...
ruby glelcg-xml.rb -l -f extensions.txt -u .. gl.xml glext_linux

echo Generating source for loading GLX extensions...
ruby glelcg-xml.rb -l -f glx_extensions.txt -u .. glx.xml glxext
