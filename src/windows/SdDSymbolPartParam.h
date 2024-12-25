/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Dialog for symbol param editing and viewing from given table. All changes are made in local.
  If nessesery result param table apply manualy.
  For symbol also An installable part is available
*/
#ifndef SDDSYMBOLPARTPARAM_H
#define SDDSYMBOLPARTPARAM_H

#include "objects/SdPItemSymbol.h"
#include "objects/SdUndo.h"
#include "SdWEditorGraphView.h"
#include "SdDParamBase.h"

#include <QPushButton>
#include <QLineEdit>
#include <QTableWidget>
#include <QDialog>

class SdWEditor3d;

class SdDSymbolPartParam : public SdDParamBase
  {
    Q_OBJECT

    SdPItemSymbol        *mComponent;         //!< Editing component
    SdUndo               *mUndo;              //!< Undo for editing

    SdWEditorGraphView   *mPartViewer;        //!< View for part of this symbol
    SdWEditor3d          *m3dView;            //!< 3d part view window

    QLineEdit            *mPart;              //!< Part name for symbol
    QPushButton          *mPartSelect;        //!< Button for part select from library
    QString               mPartUid;           //!< Current part uid

  public:
    SdDSymbolPartParam( SdPItemSymbol *symbol, QWidget *parent );

  public slots:
    void partSelect();

  private:
    void fillPart();

    // QDialog interface
  public slots:
    virtual void accept() override;
  };

#endif // SDDSYMBOLPARTPARAM_H
