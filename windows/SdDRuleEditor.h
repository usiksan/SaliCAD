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

#include <QDialog>

namespace Ui {
  class SdDRuleEditor;
}

class SdPItemPlate;

class SdDRuleEditor : public QDialog
  {
    Q_OBJECT

    SdPItemPlate *mPlate; //Plate which rules
  public:
    explicit SdDRuleEditor( SdPItemPlate *plate, QWidget *parent = nullptr);
    ~SdDRuleEditor();

  private:
    Ui::SdDRuleEditor *ui;
  };

#endif // SDDRULEEDITOR_H
