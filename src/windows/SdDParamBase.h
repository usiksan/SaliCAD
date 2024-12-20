/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Base dialog for param editing
*/
#ifndef SDDPARAMBASE_H
#define SDDPARAMBASE_H

#include "objects/SdContainer.h"

#include <QDialog>
#include <QTableWidget>
#include <QPushButton>

class SdDParamBase : public QDialog
  {
    Q_OBJECT
  protected:
    SdStringMap           mParam;             //!< Edit param table - internal representation

    QTableWidget         *mParamTable;        //!< Table of user params
    QPushButton          *mParamAdd;          //!< Append single param
    QPushButton          *mParamAddDefault;   //!< Append param from default list
    QPushButton          *mParamDelete;       //!< Delete param
    QPushButton          *mParamCopy;         //!< Copy param list from other component or symbol

    QPushButton          *mValueSelector;     //!< Select value

  public:
    SdDParamBase( QWidget *parent );

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

    void selectValue();

  protected:
    void fillParams();
    void paramAppend(int row, const QString key, const QString value );
    void paramAddInt( const QString key, const QString value = QString() );

  };

#endif // SDDPARAMBASE_H
