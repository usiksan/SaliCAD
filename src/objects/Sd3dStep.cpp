#include "Sd3dStep.h"
#include "step/SdStepReader.h"

#include <QFile>
#include <QDebug>

Sd3dStep::Sd3dStep()
  {

  }





//!
//! \brief importStep Read model from STEP file
//! \param file       STEP file
//! \result           true if import successfull or false in other case
//!
bool Sd3dStep::importStep(QIODevice *file)
  {
  //Reader which contains full file map
  SdStepReader reader;
  //Read file map
  if( reader.read(file) ) {
    //Styled items - are faces
    QStringList styledItemList = reader.styledItemList();
    for( auto styledItemId : styledItemList ) {
      //qDebug() << styledItemId << reader.value(styledItemId);
      Sd3dFace face;
      if( face.readStep( styledItemId, reader ) )
        mFaceList.append( face );
      }
    }
  return false;
  }





//!
//! \brief importStepFromFile Read model from STEP file which represented by its path
//! \param fname              Full path to STEP file
//! \return                   Pointer to Sd3dStep object if import was successfull or nullptr in other case
//!
Sd3dStep *Sd3dStep::importStepFromFile(QString fname)
  {
  QFile file(fname);

  if( file.open( QIODevice::ReadOnly ) ) {
    Sd3dStep *step = new Sd3dStep();
    if( step->importStep( &file ) )
      return step;
    delete step;
    }

  return nullptr;
  }




void Sd3dStep::writeObject(QJsonObject &obj) const
  {
  Sd3dObject::writeObject( obj );
  QJsonArray ar;
  for( const auto &face : mFaceList ) {
    ar.append( face.write() );
    }
  obj.insert( QStringLiteral("faces"), ar );
  }




void Sd3dStep::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  Sd3dObject::readObject( map, obj );
  QJsonArray ar = obj.value( QStringLiteral("faces") ).toArray();
  mFaceList.clear();
  Sd3dFace face;
  for( const auto value : ar ) {
    face.read( value.toObject() );
    mFaceList.append( face );
    }
  }




SdRect Sd3dStep::getOverRect() const
  {
  //TODO over rect for 3d model
  return SdRect{};
  }




void Sd3dStep::draw3d(QOpenGLFunctions_2_0 *f) const
  {
  for( const auto &face : mFaceList )
    face.paint( f );
  }
