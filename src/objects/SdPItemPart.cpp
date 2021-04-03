/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphics representation part of component
*/
#include "SdPItemPart.h"
#include "SdGraphIdent.h"
#include "SdProject.h"
#include "Sd3dGraphModel.h"

SdPItemPart::SdPItemPart() :
  SdProjectItem()
  {

  }


//!
//! \brief matrixMap Apply matrix to all 3d objects of part
//! \param matrix    Matrix which applied to all 3d objects
//! \param undo      Undo to which writed undo for this operation. If no
//!                  undo then no undo for this operation
//!
void SdPItemPart::matrixMap(QMatrix4x4 matrix, SdUndo *undo)
  {
  //If undo applied then append undo for this operation
  if( undo != nullptr ) {
    undo->begin( QObject::tr("3d model position and/orientation changed"), this );
    undo->matrix3d( matrix, this );
    }

  //Apply map for each GraphModel in part
  forEach( dct3D, [matrix] ( SdObject *obj ) -> bool {
    SdPtr<Sd3dGraphModel> model( obj );
    if( model.isValid() )
      model->matrixMapInPlace(matrix);
    return false;
    });
  }


QString SdPItemPart::getType() const
  {
  return QString( SD_TYPE_PART );
  }




quint64 SdPItemPart::getClass() const
  {
  return dctPart;
  }




QString SdPItemPart::getIconName() const
  {
  if( !isEditEnable() ) {
    if( mThereNewer )
      return QStringLiteral(":/pic/iconPartLockedNew.png");
    return QStringLiteral(":/pic/iconPartLocked.png");
    }
  return QStringLiteral(":/pic/iconPart.png");
  }




quint64 SdPItemPart::getAcceptedObjectsMask() const
  {
  return dctPartObjects;
  }




