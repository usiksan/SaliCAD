/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Part master for round part with exactly two pins
*/
#ifndef SDDMASTERPARTDOUBLEROUND_H
#define SDDMASTERPARTDOUBLEROUND_H

#include "SdDMasterPart.h"
#include "windows/SdIllustrator.h"

namespace Ui {
  class SdDMasterPartDoubleRound;
  }

class SdDMasterPartDoubleRound : public SdDMasterPart
  {
    Q_OBJECT

  public:
    explicit SdDMasterPartDoubleRound(SdProjectItem *item, QWidget *parent = nullptr);
    ~SdDMasterPartDoubleRound() override;

  public slots:
    //Update preview on any params changed
    void onEditChanged( const QString txt );

  protected:
    virtual void changeEvent(QEvent *e) override;

  private:
    Ui::SdDMasterPartDoubleRound *ui;

    //Draw part preview
    void drawPart( SdIllustrator &il );

    // QDialog interface
  public slots:
    //When accept we build part with current params
    virtual void accept() override;
  };

#endif // SDDMASTERPARTDOUBLEROUND_H
