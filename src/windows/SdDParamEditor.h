/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Dialog for param editing and viewing from given table. All changes are made in local.
  If nessesery result param table apply manualy

  29.01.2024 Append stm32 alternative pin function parser
*/
#ifndef SDDPARAMEDITOR_H
#define SDDPARAMEDITOR_H

#include "objects/SdContainer.h"
#include "SdDParamBase.h"

#include <QDialog>

class SdProject;


class SdDParamEditor : public SdDParamBase
  {
    Q_OBJECT

    SdProject            *mProject;             //!< Project if item is project

    QPushButton          *mParamFields;         //!< Accum fields from all sheets

    QPushButton          *mStm32AlterPinAdd;    //!< stm32 cube alternative pin function parser
    QPushButton          *mStm32AlterPinRemove; //!< stm32 alternative pin remove

    bool                  mEditEnable;
  public:
    SdDParamEditor(const QString title, const SdStringMap &map, SdProject *prj, bool editEnable, bool isProject, QWidget *parent = nullptr );
    //~SdDParamEditor() override;

  public slots:
    void paramFields();


    void stm32AlterPinAdd();
    void stm32AlterPinRemove();

  };

#endif // SDDPARAMEDITOR_H
