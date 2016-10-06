/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  SdWMain - Main application window
*/

#ifndef SBWMAIN_H
#define SBWMAIN_H

#include "SdConfig.h"
#include <QMainWindow>
#include <QSplitter>
#include <QStringList>

class SdWMain : public QMainWindow
  {
    Q_OBJECT

    QSplitter *mSplitter; //Central part of programm windows - is splitter with projects and redactors
  public:
    explicit SdWMain( const QStringList &arg, QWidget *parent = 0);

  signals:

  public slots:
  };

#endif // SBWMAIN_H
