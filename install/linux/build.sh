#!/bin/bash

~/Qt/Tools/QtInstallerFramework/3.0/bin/archivegen  packages/SaliCAD/data/sali ~/SaliCADinstall/*

~/Qt/Tools/QtInstallerFramework/3.0/bin/binarycreator -c config/config.xml -p packages SaliCAD_0_63_Linux_x86_64_Install

rm packages/SaliCAD/data/sali.7z
