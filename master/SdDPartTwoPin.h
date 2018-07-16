/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Master creation part with two pins (smd, through hole and so on)

  We receiv from user part dimensions and repaint part online
*/
#ifndef SDDPARTTWOPIN_H
#define SDDPARTTWOPIN_H

#include "windows/SdIllustrator.h"

#include <QDialog>

namespace Ui {
  class SdDPartTwoPin;
  }

class SdDPartTwoPin : public QDialog
  {
    Q_OBJECT

  public:
    explicit SdDPartTwoPin(QWidget *parent = nullptr);
    ~SdDPartTwoPin();

  protected:
    void changeEvent(QEvent *e);

  public slots:
    void onEditChanged( const QString str );

  private:
    Ui::SdDPartTwoPin *ui;

    void drawPart( SdIllustrator &il );
  };

#endif // SDDPARTTWOPIN_H
