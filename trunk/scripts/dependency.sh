#!/bin/bash

# this file shoudl be called from CMakeLists.txt
RS_WORKDIR=$1
RS_LIBSDIR=${RS_WORKDIR}/src/libs

echo ${RS_WORKDIR}
echo ${RS_LIBSDIR}

rm -rf ${RS_LIBSDIR}
mkdir -p ${RS_LIBSDIR}

# install the dependency for rtmp server

# google test frame
echo "ready to install google test frame"
(
    cd ${RS_WORKDIR}/3rdlibrary
    tar xvf googletest-release-1.8.0.tar
)

echo "google test frame installed successfully"
