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
#include <QMatrix4x4>


class SdContainer;
class SdObject;
class SdPoint;
class SdPointList;
struct SdPropInt;
class SdPropString;
class SdPropLayer;
class SdPropLine;
class SdPropText;
class SdPropSymPin;
class SdPropPartPin;
class SdPropSymImp;
class SdPropPartImp;
class SdRect;
class SdPItemPart;
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
class Sd3drModel;
class SdFileUid;


template <typename T>
struct SdUndoFieldBase
  {
    T *mSrc;
    T  mCopy;

    explicit SdUndoFieldBase( T *src ) : mSrc(src), mCopy(*src) {}

    void undoOne()
      {
      if constexpr (std::is_trivially_copyable_v<T>)
        { qSwap( *mSrc, mCopy ); }
      else
        { mSrc->swap( mCopy ); }
      }
  };






template <typename ... Ts>
class SdUndoRecordField : public SdUndoRecord, private SdUndoFieldBase<Ts>...
  {
    using PostAction = std::function<void()>;
    PostAction mPredAction;
    PostAction mPostAction;
  public:
    explicit SdUndoRecordField( Ts*... ptrs ) : SdUndoRecord(), SdUndoFieldBase<Ts>(ptrs)..., mPredAction([]{}), mPostAction([]{}) {}

    virtual void undo() override
      {
      mPredAction();
      (SdUndoFieldBase<Ts>::undoOne(), ...);
      mPostAction();
      }

    SdUndoRecordField<Ts ...> &post( PostAction fun ) { mPostAction = std::move(fun); return *this; }

    SdUndoRecordField<Ts ...> &pred( PostAction fun ) { mPredAction = std::move(fun); return *this; }
  };





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

    //!
    //! \brief begin Appends "begin" record of group of undo commands. When undo then
    //!              group undo as single unit
    //! \param title Title for undo
    //! \param item  Item on which undo executed
    //! \param is3d  When true then item used in 3d view mode
    //!
    void begin( QString title, SdProjectItem *item, bool is3d );

    void projectItemInfo(SdProjectItem *item, QString *title, QString *author, SdFileUid *fileUid, bool *editEnable );

    void stringMapItem( SdStringMap *assoc, const QString key );

    //!
    //! \brief matrix3d Add undo for 3d matrix conversion
    //! \param matrix   Matrix for conversion
    //! \param part     Part on which conversion applied
    //!
    void matrix3d( QMatrix4x4 matrix, SdPItemPart *part );


    template <typename... Ts>
    SdUndoRecordField<Ts...>& prop( Ts* ... ptrs )
      {
      static_assert( (std::is_pointer_v<Ts*> && ...), "Must pass pointer " );

      auto *rec = new SdUndoRecordField<Ts...>(ptrs...);
      addUndo( rec );

      return *rec;
      }


    //do undo and redo
    void undoStep();
    void redoStep();

    //Update undo and redo commands status
    void undoRedoUpdate();
  private:
    void addUndo( SdUndoRecordPtr ptr );
  };

#endif // SDUNDO_H
