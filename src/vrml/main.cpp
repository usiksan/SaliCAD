
#include "SdScanerVrml.h"

#include <QCoreApplication>


int main(int argc, char *argv[])
  {
  QCoreApplication a(argc, argv);

  SdScanerVrml scaner;
  scaner.parseFile( QStringLiteral("/home/salilab/work/SaliCAD/libFiles/mtp2955v.wrl") );

  return a.exec();
  }
