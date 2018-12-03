/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Part master for rectangle part with exactly two pins
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
    //Update preview on any params changed
    void onEditChanged( const QString txt );

  protected:
    virtual void changeEvent(QEvent *e) override;

  private:
    Ui::SdDMasterPartDoubleRect *ui;

    //Draw part preview
    void drawPart( SdIllustrator &il );

    // QDialog interface
  public slots:
    //When accept we build part with current params
    virtual void accept() override;
  };

#endif // SDDMASTERPARTDOUBLERECT_H
