#!/bin/bash

~/Qt/Tools/QtInstallerFramework/4.6/bin/archivegen  packages/SaliCAD/data/sali ~/SaliCADinstall/*

~/Qt/Tools/QtInstallerFramework/4.6/bin/binarycreator -c config/config.xml -p packages SaliCAD_2_7_Linux_x86_64_Install

rm packages/SaliCAD/data/sali.7z
