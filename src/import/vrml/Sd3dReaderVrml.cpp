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
#include "SdScanerVrml.h"
#include "objects/Sd3dGraphModel.h"
#include "VrmlNodeMaterial.h"

#include <QMessageBox>

#if 0
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
    Sd3dFaceSet *faceSet = new Sd3dFaceSet();

    //Generation
    scanerVrml.generateFaces( [faceSet] ( const QVector3DList &vertexList, const QVector3DList &normalList, const VrmlNodeMaterial *material ) {
      //Convert from VrmlNodeMaterial to Sd3dFaceMaterial
      Sd3dFaceMaterial faceMaterial;
      if( material != nullptr ) {
        faceMaterial.setAmbientIntensity( material->ambientIntensity() );
        faceMaterial.setDiffuseColor( material->diffuseColor(0), material->diffuseColor(1), material->diffuseColor(2) );
        faceMaterial.setEmissiveColor( material->emissiveColor(0), material->emissiveColor(1), material->emissiveColor(2) );
        faceMaterial.setShininnes( material->shininnes() );
        faceMaterial.setSpecularColor( material->specularColor(0), material->specularColor(1), material->specularColor(2) );
        faceMaterial.setTransparency( material->transparency() );
        }
      faceSet->faceAdd( Sd3dFaceEx( vertexList, normalList, faceMaterial ) );
      });

    //Return 3d object
    return faceSet;
    }
  QMessageBox::warning( parent, QObject::tr("Error happens when read VRML file!"), scanerVrml.errorGet() );
  return nullptr;
  }
#endif




//!
//! \brief importVrml Read model from VRML file which represented by its path
//! \param fname      Full path to VRML file
//! \param parent     Parent widget. Is used to display messages and progress bar
//! \return           Pointer to Sd3dGraph object if import was successfull or nullptr in other case
//!
Sd3dGraph *Sd3dReaderVrml::importVrmlFromFile(QString fname, QWidget *parent)
  {
  SdScanerVrml scanerVrml;
  if( scanerVrml.parseFile(fname) ) {
    //Generate faces

    //3d object to put faces in
    Sd3dGraphModel *model = new Sd3dGraphModel();

    //Generation
    scanerVrml.generateFaces( [model] ( const QVector3DList &vertexList, const QVector3DList &normalList, const VrmlNodeMaterial *material ) {
      Q_UNUSED(normalList)
      //Convert to Sd3dFace and append it to model
      Sd3dRegion region(vertexList);
      model->faceAdd( Sd3dFace( region, QColor::fromRgbF(material->diffuseColor(0), material->diffuseColor(1), material->diffuseColor(2))));
      });

    //Return 3d object
    return model;
    }
  QMessageBox::warning( parent, QObject::tr("Error happens when read VRML file!"), scanerVrml.errorGet() );
  return nullptr;
  }





//!
//! \brief asProgramm Read model from VRML file and represents it as 3d programm
//! \param fname      Full path to VRML file
//! \param parent     Parent widget. Is used to display messages and progress bar
//! \return           String contains programm which represents VRML model
//!
QString Sd3dReaderVrml::asProgramm(QString fname, QWidget *parent)
  {
  SdScanerVrml scanerVrml;
  if( scanerVrml.parseFile(fname) ) {
    //Generate faces

    //String to generate programm in
    QString programm( QString("\n#VRML model from file %1\nvrmlModel = [").arg(fname) );

    //Generation
    bool first = true;
    scanerVrml.generateFaces( [&programm,&first] ( const QVector3DList &vertexList, const QVector3DList &normalList, const VrmlNodeMaterial *material ) {
      Q_UNUSED(normalList)
      //Append only triangles
      if( vertexList.count() == 3 ) {
        if( !first ) programm.append( ",\n" );
        programm.append( "faceTriangle( " );
        for( int i = 0; i < 3; i++ ) {
          //Take next vertex
          QVector3D v( vertexList.at(i) );
          //print it
          programm.append( QString("vertex( %1, %2, %3 ), ").arg( v.x() ).arg( v.y() ).arg( v.z() ) );
          }
        //print color
        programm.append( QString("color( %1, %2, %3 ) )").arg(material->diffuseColor(0)).arg(material->diffuseColor(1)).arg(material->diffuseColor(2)) );
        }
      });

    //Complete model
    programm.append( "]\n\n\n" );
    return programm;
    }
  QMessageBox::warning( parent, QObject::tr("Error happens when read VRML file!"), scanerVrml.errorGet() );
  return QString{};
  }
