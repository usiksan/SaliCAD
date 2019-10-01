/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Part master with rectangle body and single pins line (row)
*/
#ifndef SDDMASTERPARTSINGLELINE_H
#define SDDMASTERPARTSINGLELINE_H

#include "SdDMasterPart.h"
#include "windows/SdIllustrator.h"

#include <QDialog>

namespace Ui {
  class SdDMasterPartSingleLine;
}

class SdDMasterPartSingleLine : public SdDMasterPart
  {
    Q_OBJECT

  public slots:
    //Update preview on any params changed
    void onEditChanged( const QString txt );

  public:
    explicit SdDMasterPartSingleLine(SdProjectItem *item, QWidget *parent = nullptr);
    ~SdDMasterPartSingleLine() override;

  private:
    Ui::SdDMasterPartSingleLine *ui;

    //Draw part preview
    void drawPart( SdIllustrator &il );

    // QDialog interface
  public slots:
    virtual void accept() override;
  };

#endif // SDDMASTERPARTSINGLELINE_H
