/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#ifndef SDWVIEW_H
#define SDWVIEW_H

#include "objects/SdPoint.h"
#include "objects/SdRect.h"
#include "objects/SdScaler.h"
#include "modes/SdMode.h"
#include <QWidget>

class SdWView : public QWidget
  {
    Q_OBJECT

  public:
    explicit SdWView(QWidget *parent = 0);

  signals:

  public slots:
  };

#endif // SDWVIEW_H
