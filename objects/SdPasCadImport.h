/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SDPASCADIMPORT_H
#define SDPASCADIMPORT_H

#include "SdProject.h"

#include <QFile>
#include <QWidget>
#include <QMap>

class DLayer;
class DFont;

typedef QMap<int,SdObjectPtr> SdImportMap;

class SdPasCadImport
  {
    SdProject   *mProject;
    QFile       *mFile;
    QWidget     *mOwner;
    DLayer      *mLayerTable;
    int          mLayerNumber;
    DFont       *mFontTable;
    int          mFontNumber;
    SdImportMap  mObjectMap;
  public:
    SdPasCadImport( SdProject *prj, QWidget *owner );
    ~SdPasCadImport();

    //Import entry point
    bool import( const QString fname );

  private:
    bool      project();
    bool      symbol();
    bool      part();
    bool      alias();
    bool      sheet();
    bool      plate();
    bool      text();
    bool      projectItem( SdProjectItem *item );
    SdObject *readObject( const QString type );

    int       readInt32();
    bool      readBool();
    QString   readNString();
    QString   readConstString( int len );
    QString   readWName() { return readConstString(32); }
    QString   readName() { return readConstString(8); }
    bool      error( QString msg );
  };

#endif // SDPASCADIMPORT_H
