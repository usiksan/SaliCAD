#include <QCoreApplication>

#include "SdM3dParser.h"

const char *programm =
R"VVV(
#Prog

f1 = 10.0

)VVV";



int main(int argc, char *argv[])
  {
  QCoreApplication a(argc, argv);

  SdM3dParser parser;

  SdM3dProgramm *prog = parser.parse( QString::fromLatin1(programm) );

  prog->execute();

  delete prog;

  //return a.exec();
  }
