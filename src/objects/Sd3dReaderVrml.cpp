/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d object reader for VRML files
*/
#include "Sd3dReaderVrml.h"
#include "vrml/SdScanerVrml.h"

#include <QMessageBox>

inline Sd3dPoint fromVrmlVector( VrmlVector vec )
  {
  return Sd3dPoint( vec.mX * 1000000.0, vec.mY * 1000000.0, vec.mZ * 1000000.0 );
  }

//!
//! \brief importVrmlFromFile Read model from VRML file which represented by its path
//! \param fname              Full path to VRML file
//! \return                   Pointer to Sd3dFaceSet object if import was successfull or nullptr in other case
//!
Sd3dFaceSet *Sd3dReaderVrml::importVrmlFromFile(QString fname, QWidget *parent)
  {
  SdScanerVrml scanerVrml;
  if( scanerVrml.parseFile(fname) ) {
    //Generate faces

    //3d object to put faces in
    Sd3dFaceSet *faseSet = new Sd3dFaceSet();

    //Generation
    scanerVrml.generateFaces( [faseSet] ( const VrmlVectorList &vertexList, VrmlVector normal, quint32 color ) {
      QList<Sd3dPoint> point3dList;
      Sd3dPoint normal3d( fromVrmlVector(normal) );
      for( auto vec : vertexList )
        point3dList.append( fromVrmlVector(vec) );
      faseSet->faceAdd( Sd3dFace( point3dList, normal3d, color )  );
      });

    //Return 3d object
    return faseSet;
    }
  QMessageBox::warning( parent, QObject::tr("Error happens when read VRML file!"), scanerVrml.errorGet() );
  return nullptr;
  }
