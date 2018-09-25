/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Part master for rectangle part with pins on all four sides
*/
#ifndef SDDMASTERPARTQUADSIDE_H
#define SDDMASTERPARTQUADSIDE_H

#include "SdDMasterPart.h"
#include "windows/SdIllustrator.h"

namespace Ui {
  class SdDMasterPartQuadSide;
  }

class SdDMasterPartQuadSide : public SdDMasterPart
  {
    Q_OBJECT

  public:
    explicit SdDMasterPartQuadSide(SdProjectItem *item, QWidget *parent = nullptr);
    ~SdDMasterPartQuadSide() override;

  public slots:
    //Update preview on any params changed
    void onEditChanged( const QString txt );

  protected:
    virtual void changeEvent(QEvent *e) override;

  private:
    Ui::SdDMasterPartQuadSide *ui;

    //Draw part preview
    void drawPart( SdIllustrator &il );

    // QDialog interface
  public slots:
    //When accept we build part with current params
    virtual void accept() override;
  };

#endif // SDDMASTERPARTQUADSIDE_H
