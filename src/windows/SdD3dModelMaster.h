/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model master.

  It allow build new 3d and 2d projection model of part
*/
#ifndef SDD3DMODELMASTER_H
#define SDD3DMODELMASTER_H

#include "objects/SdPItemRich.h"
#include "objects/SdPItemPart.h"
#include "script/SdScriptProgramm.h"
#include "SdWView3d.h"

#include <QDialog>
#include <QTableWidget>

class SdScriptProgramm;

class SdD3dModelMaster : public QDialog
  {
    Q_OBJECT

    SdPItemPart                   *mPart;         //!< Created part object. It used also for preview
    SdScriptProgrammPtr            mProgramm;     //!< Compiled 3d model programm
    QTableWidget                  *mParamWidget;  //!< Table with 3d model parameters
    SdWView3d                     *mPreview;      //!< 3d model preview
    Sd3dModel                      mModel;        //!< Prebuilded model
    QString                        mSourceScript; //!< Source script
    bool                           mActive;       //!< Flag to preserve of recursing call
  public:
    //!
    //! \brief SdD3dModelMaster Dialog constructor. It creates dialog for build new 3d and 2d projection model of part
    //! \param script           Script of 3d model
    //! \param part             Pointer to created part object
    //! \param parent           Parent widget
    //!
    SdD3dModelMaster( const QString script, SdPItemPart *part, QWidget *parent );
    ~SdD3dModelMaster();

    //!
    //! \brief scriptRelease Return source script modified with input values
    //! \return              Modified script
    //!
    QString scriptRelease() const;
  public slots:
    //!
    //! \brief rebuild Rebuild resultat model on model param changed
    //!
    void    rebuild();

    //!
    //! \brief onCellClicked Called when user clicked on mParamWidget cell
    //! \param row           Row on which was clicked
    //! \param column        Column on which was clicked
    //!
    void    onCellClicked( int row, int column );

  };

#endif // SDD3DMODELMASTER_H
