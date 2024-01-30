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

#include <QDialog>
#include <QTableWidget>
#include <QPushButton>
#include <functional>

class SdProject;


class SdDParamEditor : public QDialog
  {
    Q_OBJECT

    SdProject            *mProject;             //!< Project if item is project
    SdStringMap           mParam;               //!< Edit param table - internal representation

    QTableWidget         *mParamTable;          //!< Table of user params - visual represantion
    QPushButton          *mParamAdd;
    QPushButton          *mParamAddDefault;
    QPushButton          *mParamDelete;
    QPushButton          *mParamCopy;
    QPushButton          *mParamFields;         //!< Accum fields from all sheets
    QPushButton          *mValueSelector;       //!< Select value

    QPushButton          *mStm32AlterPinAdd;    //!< stm32 cube alternative pin function parser
    QPushButton          *mStm32AlterPinRemove; //!< stm32 alternative pin remove

    bool                  mEditEnable;
  public:
    SdDParamEditor(const QString title, const SdStringMap &map, SdProject *prj, bool editEnable, bool isProject, QWidget *parent = nullptr );
    //~SdDParamEditor() override;

    const SdStringMap  paramTable() const { return mParam; }

    //Default params
    static QStringList defParamList();

    //Default param description
    static QString     defParamDescription( QString paramName );

    //Default param value
    static QString     defParamValue( QString paramName, SdProjectItem *item, QWidget *parent );

  public slots:
    void onParamChanged( int row, int column );

    void paramAdd();
    void paramAddDefault();
    void paramDelete();
    void paramCopy();
    void paramFields();

    void selectValue();

    void stm32AlterPinAdd();
    void stm32AlterPinRemove();

  private:
    void fillParams();
    void paramAppend(int row, const QString key, const QString value );
    void paramAddInt( const QString key, const QString value = QString() );
  };

#endif // SDDPARAMEDITOR_H
