/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d mode used to change color for any face of model
*/
#ifndef SD3DMODEFACECOLOR_H
#define SD3DMODEFACECOLOR_H

#include "Sd3dMode.h"
#include "objects/SdPItemPart.h"
#include "objects/Sd3dGraphModel.h"

#include <QTableWidget>

class Sd3dModeFaceColorPrivate : public QObject
  {
    Q_OBJECT

    QWidget        *mView;      //!< View with 3d model
    QTableWidget   *mTable;     //!< Visual table of face with color for each face
    Sd3dGraphModel *mModel;     //!< Model whose faces are viewed and changed
    int             mFaceIndex; //!< Active face index
    QColor          mFaceColor; //!< Active face color
    bool            mActive;    //!< Flag to suppress recursion
  public:
    Sd3dModeFaceColorPrivate(QWidget *parent, SdPItemPart *part );

  public slots:
    void onCellActivated( int row, int column );
    void onCellChanged( int row, int column );
  };




class Sd3dModeFaceColor : public Sd3dMode
  {
    Sd3dModeFaceColorPrivate *mPrivate; //!< Object derived from QObject which receiv signals from visual table
  public:
    Sd3dModeFaceColor( QWidget *parent, SdPItemPart *part );
    ~Sd3dModeFaceColor();

    // Sd3dMode interface
  public:
    virtual bool    draw3d(QOpenGLFunctions_2_0 *f) override;
    virtual int     modeId() const override;
    virtual QString getStepHelp() const override;
    virtual QString getModeThema() const override;
  };

#endif // SD3DMODEFACECOLOR_H
