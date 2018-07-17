/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SDDMASTERPARTDOUBLERECT_H
#define SDDMASTERPARTDOUBLERECT_H

#include "SdDMasterPart.h"
#include "windows/SdIllustrator.h"

namespace Ui {
  class SdDMasterPartDoubleRect;
}

class SdDMasterPartDoubleRect : public SdDMasterPart
  {
    Q_OBJECT

  public:
    explicit SdDMasterPartDoubleRect( SdProjectItem *item, QWidget *parent = nullptr);
    ~SdDMasterPartDoubleRect() override;

  public slots:
    void onEditChanged( const QString txt );

  private:
    Ui::SdDMasterPartDoubleRect *ui;

    void drawPart( SdIllustrator &il );

    // QDialog interface
  public slots:
    virtual void accept() override;
  };

#endif // SDDMASTERPARTDOUBLERECT_H
