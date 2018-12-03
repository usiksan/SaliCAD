/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Rule editor
*/
#ifndef SDDRULEEDITOR_H
#define SDDRULEEDITOR_H

#include "objects/SdRuleBlock.h"

#include <QDialog>
#include <QStringList>
#include <QLineEdit>

namespace Ui {
  class SdDRuleEditor;
}

class SdPItemPlate;

class SdDRuleEditor : public QDialog
  {
    Q_OBJECT

    SdPItemPlate   *mPlate;   //Plate which rules
    SdRuleBlock     mPcb;     //Rules block for pcb
    SdRuleBlockMap  mNet;     //Rules block map for all nets
    QStringList     mNetList; //Selected net list
  public:
    explicit SdDRuleEditor( SdPItemPlate *plate, QWidget *parent = nullptr);
    ~SdDRuleEditor() override;

  private:
    Ui::SdDRuleEditor *ui;

    //Setup visual fields for net block from all selected nets
    void setNetBlock();

    //Store from visual fields to all selected nets
    void getNetBlock();

    //Write value to visual editor
    void writeToLineEdit( int v, QLineEdit *edit );

    //Read value from visual editor
    int  readFromLineEdit( QLineEdit *edit, bool *isEmpty );

    // QDialog interface
  public slots:
    virtual void accept() override;
  };

#endif // SDDRULEEDITOR_H
