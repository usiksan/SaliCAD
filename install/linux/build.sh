#!/bin/bash

~/Qt/Tools/QtInstallerFramework/3.1/bin/archivegen  packages/SaliCAD/data/sali ~/SaliCADinstall/*

~/Qt/Tools/QtInstallerFramework/3.1/bin/binarycreator -c config/config.xml -p packages SaliCAD_1_35_Linux_x86_64_Install

rm packages/SaliCAD/data/sali.7z
