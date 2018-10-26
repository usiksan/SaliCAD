/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Pad master dialog
*/
#ifndef SDDPADMASTER_H
#define SDDPADMASTER_H

#include "objects/SdPad.h"
#include "SdIllustrator.h"

#include <QDialog>

namespace Ui {
  class SdDPadMaster;
}

class SdDPadMaster : public QDialog
  {
    Q_OBJECT

    SdPad mPad;
  public:
    explicit SdDPadMaster(SdPad pad, QWidget *parent = nullptr);
    ~SdDPadMaster();

    SdPad pad() const { return mPad; }

    //Build pad from source name, edit it and return new name
    static QString build( const QString name, QWidget *parent );

  private:
    Ui::SdDPadMaster *ui;

    void updatePadProfile();

    void updatePadSchematic();

    void drawPadSchematic( SdIllustrator &ill );

  public slots:
    //Toggle between circle pad and rectangle pad
    void onPadCircleToggle( bool isCircle );

    //Enable-disable through pin (else planar)
    void onThroughPin( bool isThrough );

    //Enable-disable stensil array on isEna and current settings of stensil cols and rows
    void onArrayEnable( bool isEna );
  };

#endif // SDDPADMASTER_H
