/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  SaliCadServer configuration file

History
  07.10.2017 v0.1 created
  08.10.2017 v0.2 data base interface
  12.11.2017 v0.3 append user registration and login
  03.01.2018 v0.4 reconstructed interchange algorithm
  24.09.2018 v0.5 remove category from database
                  append library remote maintance
*/
#ifndef SDCSCONFIG_H
#define SDCSCONFIG_H

#define SALI_CAD_MAJOR   0
#define SALI_CAD_MINOR   5
#define SALI_CAD_VERSION "v0.5"

#define SALI_CAD_SERVER_INFO "A.Sibilev SaliCAD library server " SALI_CAD_VERSION

#define UPGRADE_PATH "upgrade/"
#define LIBRARY_PATH "/library/"

#define MACHINE_LIMIT 3

#endif // SDCSCONFIG_H
