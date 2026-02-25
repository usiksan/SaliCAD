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
#include "library/SdStringMap.h"

#include <QFile>
#include <QWidget>
#include <QMap>
#include <QList>

struct DLayer;
struct DFont;
class  SdLayer;
class  SdPropText;
class  SdPItemComponent;
class  SdGraphIdent;
class  SdRect;

typedef QMap<int,SdObjectPtr> SdImportMap;

class SdPasCadImport
  {
    SdProject         *mProject;
    QFile             *mFile;
    QWidget           *mOwner;
    DLayer            *mLayerTable;
    int                mLayerNumber;
    DFont             *mFontTable;
    int                mFontNumber;
    SdImportMap        mObjectMap;
    QList<SdStringMap> mPinsPack;
  public:
    SdPasCadImport( SdProject *prj, QWidget *owner );
    ~SdPasCadImport();

    //Import entry point
    bool import( const QString fname );

  private:
    bool         project();
    bool         projectItem( SdProjectItem *item );
    SdObject    *buildObject( int id );
    SdLayer     *readLayer();

    int          readInt8();
    int          readInt16();
    int          readInt32();
    bool         readBool();
    QString      readNString();
    QString      readConstString( int len );
    QString      readWName() { return readConstString(32); }
    QString      readName() { return readConstString(8); }
    bool         error( QString msg );
    bool         readObjectTable( SdContainer *container );
    bool         readSingleObject( SdContainer *container );
    bool         readSymbol( SdObject *obj );
    bool         readPart( SdObject *obj );
    bool         readSheet( SdObject *obj );
    SdPoint      readPoint();
    SdRect       readRectangle();
    bool         readLinear( SdObject *obj );
    bool         readRect( SdObject *obj );
    bool         readLine( SdObject *obj );
    bool         readTextProp(SdPropText *prp , SdPoint *origin);
    bool         readText( SdObject *obj );
    bool         readCircle( SdObject *obj );
    bool         readArc( SdObject *obj );
    bool         readSymPin( SdObject *obj );
    bool         readIdent( SdGraphIdent *ident );
    bool         readPartPin( SdObject *obj );
  };

#endif // SDPASCADIMPORT_H
