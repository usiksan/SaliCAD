
#include "SdScanerVrml.h"

#include <QCoreApplication>


int main(int argc, char *argv[])
  {
  QCoreApplication a(argc, argv);

  SdScanerVrml scaner;
  scaner.parseFile( QStringLiteral("/home/asibilev/work/vrml/WF-02.wrl") );

  return a.exec();
  }
