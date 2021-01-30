
#include "SdScanerVrml.h"

#include <QCoreApplication>


int main(int argc, char *argv[])
  {
  QCoreApplication a(argc, argv);

  SdScanerVrml scaner;
  scaner.parseFile( QStringLiteral("/home/salilab/work/SaliCAD/libFiles/User Library-SOIC-8-1.wrl") );

  return a.exec();
  }
