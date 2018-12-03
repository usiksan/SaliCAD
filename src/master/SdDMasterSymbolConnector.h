/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Symbol creation master for connectors
*/
#ifndef SDDMASTERSYMBOLCONNECTOR_H
#define SDDMASTERSYMBOLCONNECTOR_H

#include "SdDMasterSymbol.h"
#include "windows/SdIllustrator.h"

namespace Ui {
  class SdDMasterSymbolConnector;
}

class SdDMasterSymbolConnector : public SdDMasterSymbol
  {
    Q_OBJECT

  public:
    explicit SdDMasterSymbolConnector( SdProjectItem *item, QWidget *parent = nullptr);
    ~SdDMasterSymbolConnector() override;


  public slots:
    //Update preview on any params changed
    void onEditChanged( const QString txt );

  private:
    Ui::SdDMasterSymbolConnector *ui;

    //Draw symbol preview
    void drawSymbol( SdIllustrator &il );

    // QDialog interface
  public slots:
    virtual void accept() override;
  };


#endif // SDDMASTERSYMBOLCONNECTOR_H
