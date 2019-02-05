/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo system.
  With every action it store in undo stack by set undo records.
  Every undo step separated with special undo record "title" witch serves as step marker
*/

#ifndef SDUNDO_H
#define SDUNDO_H
#include "SdConfig.h"
#include "SdUndoRecord.h"
#include "library/SdStringMap.h"
#include "SdPad.h"
#include "SdRuleBlock.h"
#include "SdPointList.h"
#include "SdPolyWindowList.h"
#include <QStack>
#include <QMap>
#include <QStringList>


class SdContainer;
class SdObject;
class SdPoint;
class SdPointList;
class SdPropInt;
class SdPropString;
class SdPropLayer;
class SdPropLine;
class SdPropText;
class SdPropSymPin;
class SdPropPartPin;
class SdPropSymImp;
class SdPropPartImp;
class SdRect;
class SdPItemPlate;
typedef SdPItemPlate *SdPItemPlatePtr;
class SdGraphSymImp;
class SdGraphPartImp;
class SdProjectItem;
struct SdSymImpPin;
typedef QMap<QString,SdSymImpPin> SdSymImpPinTable;
struct SdPartImpPin;
typedef QMap<QString,SdPartImpPin> SdPartImpPinTable;
struct SdPropPolygon;

class SdUndo
  {
    QStack<SdUndoRecordPtr> mUndo;
    QStack<SdUndoRecordPtr> mRedo;
    int                     mUndoCount;
  public:
    SdUndo();

    //info
    bool isUndoPresent() const { return mUndo.count() != 0; }
    bool isRedoPresent() const { return mRedo.count() != 0; }

    //save undo
    void insertObject( SdContainer *container, SdObject *object );
    void deleteObject( SdContainer *container, SdObject *object );
    void propLineAnd3Point(SdPropLine *prp, SdPoint *p1, SdPoint *p2 , SdPoint *p3 = nullptr);
    void propLinePointInt( SdPropLine *prp, SdPoint *p, int *val );
    void propLinePointTable( SdPropLine *prp, SdPointList *list );
    void propLineRect2Int( SdPropLine *prp, SdRect *r, int *val1, int *val2 );
    void propTextAndText( SdPropText *prp, SdPoint *org, SdRect *r, QString *str );
    void propSymPin( SdPropSymPin *prp, SdPoint *org );
    void propPartPin( SdPropPartPin *prp, SdPoint *org );
    void platePointer( SdPItemPlatePtr *ptr );
    void pinSymImpStatus( SdGraphSymImp *sym, const QString symPinName );
    void symImpPins( SdSymImpPinTable *table );
    void partImpPins( SdPartImpPinTable *table );
    void linkSection( int section, SdGraphSymImp *sym, SdGraphPartImp *part, bool link );
    void symImp( SdPoint *origin, SdPropSymImp *imp, int *logSection, int *logNumber, SdRect *over );
    void partImp(SdPoint *origin, SdPropPartImp *imp, int *logNumber, SdRect *over);
    void wire( SdPropLine *prop, SdPoint *p1, SdPoint *p2, bool *dot1, bool *dot2 );
    void point( SdPoint *src );
    void begin(QString title , SdProjectItem *item);
    void projectItemInfo(SdProjectItem *item, QString *title, QString *author, int *timeCreation, bool *editEnable );
    void string2( QString *str1, QString *str2 );
    void stringMapItem( SdStringMap *assoc, const QString key );
    void stringMap( SdStringMap *assoc );
    void padAssociation( QString *id, QString *srcName, SdPadMap *srcMap );
    void road( SdPropInt *width, SdPoint *p1, SdPoint *p2 );
    void via( SdPropString *pad, SdPoint *pos );
    void rule( SdRuleBlock *pcbSrc, SdRuleBlockMap *mapSrc );
    void stringList( int *val, QStringList *list );
    void polygon( SdPropPolygon *propSource, SdPointList *regionSource, SdPolyWindowList *windowsSource );

    //do undo and redo
    void undoStep();
    void redoStep();

    //Update undo and redo commands status
    void undoRedoUpdate();
  private:
    void addUndo( SdUndoRecordPtr ptr );
  };

#endif // SDUNDO_H
