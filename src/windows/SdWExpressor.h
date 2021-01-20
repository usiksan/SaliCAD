/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Expression calculation widget

  With this I introduce expression calculator for various schematic calculations

  Widget contains two parts:
    - left part for edit expressions
    - right part for expression calculation
  and footer with some buttons
*/
#ifndef SDWEXPRESSOR_H
#define SDWEXPRESSOR_H

#include <QWidget>

class SdWExpressor : public QWidget
  {
    Q_OBJECT
  public:
    explicit SdWExpressor(QWidget *parent = nullptr);

  signals:

  };

#endif // SDWEXPRESSOR_H
