/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Complex mode for selection, edit, move many graph objects
*/
#include "SdModeSelect.h"
#include "objects/SdEnvir.h"
#include "objects/SdPItemSheet.h"
#include "objects/SdPItemPlate.h"
#include "objects/SdGraphNetWire.h"
#include "objects/SdPulsar.h"
#include "objects/SdConverterOffset.h"

//All prop bars
#include "windows/SdPropBarLinear.h"
#include "windows/SdPropBarPartPin.h"
#include "windows/SdPropBarSymImp.h"
#include "windows/SdPropBarPartImp.h"
#include "windows/SdPropBarSymPin.h"
#include "windows/SdPropBarTextual.h"
#include "windows/SdPropBarRoad.h"
#include "windows/SdPropBarPolygon.h"
#include "windows/SdPropBarWire.h"

#include "windows/SdWCommand.h"
#include "windows/SdWEditorGraph.h"


#include <QObject>
#include <QDebug>

SdModeSelect::SdModeSelect(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdMode( editor, obj ),
  mShift(false),      //Shift key pressed [Нажата клавиша Shift]
  mControl(false),    //Control key pressed [Нажата клавиша Control]
  mPastePrj(nullptr)  //Project, fragment inserted from [Проект, из которого производится вставка]
  {

  }

SdModeSelect::~SdModeSelect()
  {
  if( mPastePrj ) delete mPastePrj;
  }





void SdModeSelect::activate()
  {
  propSetToBar();
  SdMode::activate();
  }



void SdModeSelect::reset()
  {
  unselect(true);
  setStep(smNoSelect);
  update();
  //UpdateTools();
  }




void SdModeSelect::drawStatic(SdContext *ctx)
  {
  //Draw all object except selected
  mObject->forEach( dctAll, [this, ctx] (SdObject *obj) -> bool {
    SdGraph *graph = dynamic_cast<SdGraph*>( obj );
    if( graph != nullptr && graph->getSelector() != &mFragment )
      graph->draw( ctx );
    return true;
    });
  }




void SdModeSelect::drawDynamic(SdContext *ctx)
  {
  //Draw all selected elements
  ctx->setOverColor( sdEnvir->getSysColor(scSelected) );
  mFragment.draw( ctx );

  {
  //Draw all copy in current point
  SdConverterOffset offset( mPrevMove.sub(mFirst) );
  ctx->setConverter( &offset );
  mPaste.draw( ctx );
  }

  //On according step
  switch( getStep() ) {
    case smSelRect : showRect( ctx ); break;
    }
  }




int SdModeSelect::getPropBarId() const
  {
  //If no selection return default prop bar
  if( mFragment.count() == 0 )
    return PB_NO_SELECTION;
  //Return prop bar according selected objects prop
  return mLocalProp.mPropBarId;
  }



void SdModeSelect::propGetFromBar()
  {

  if( mFragment.count() ) {
    setDirty();
    mUndo->begin( QObject::tr("Properties changed"), mObject );
    //Сохранить состояние объектов до изменения свойств
    mFragment.forEach( dctAll, [this] (SdObject *obj) -> bool {
      SdGraph *graph = dynamic_cast<SdGraph*>( obj );
      if( graph != nullptr )
        graph->saveState( mUndo );
      return true;
      });
    }

  //Get new props
  mLocalProp.clear();
  switch( mLocalProp.getPropBarId() ) {
    case PB_LINEAR : {
      SdPropBarLinear  *barLinear  = dynamic_cast<SdPropBarLinear*>(SdWCommand::getModeBar(PB_LINEAR));
      barLinear->getPropLine( &(mLocalProp.mLineProp), &(mLocalProp.mEnterType) );
      mLocalProp.setLayer( mLocalProp.mLineProp.mLayer );
      }
      break;
    case PB_TEXT : {
      SdPropBarTextual *barTextual = dynamic_cast<SdPropBarTextual*>(SdWCommand::getModeBar(PB_TEXT));
      barTextual->getPropText( &(mLocalProp.mTextProp) );
      mLocalProp.setLayer( mLocalProp.mTextProp.mLayer );
      }
      break;
    case PB_WIRE : {
      SdPropBarWire    *barWire    = dynamic_cast<SdPropBarWire*>(SdWCommand::getModeBar(PB_WIRE));
      QString wireName;
      barWire->getPropWire( &(mLocalProp.mWireProp), &(mLocalProp.mEnterType), &(wireName) );
      mLocalProp.setLayer( mLocalProp.mWireProp.mLayer );
      if( !wireName.isEmpty() )
        mLocalProp.mWireName = wireName;
      }
      break;
    case PB_SYM_PIN : {
      SdPropBarSymPin  *barSymPin  = dynamic_cast<SdPropBarSymPin*>(SdWCommand::getModeBar(PB_SYM_PIN));
      barSymPin->getPropSymPin( &(mLocalProp.mSymPinProp) );
      mLocalProp.setLayer( mLocalProp.mSymPinProp.mLayer );
      }
      break;
    case PB_PART_PIN : {
      SdPropBarPartPin *barPartPin = dynamic_cast<SdPropBarPartPin*>(SdWCommand::getModeBar(PB_PART_PIN));
      barPartPin->getPropPartPin( &(mLocalProp.mPartPinProp) );
      mLocalProp.setLayer( mLocalProp.mPartPinProp.mLayer );
      }
      break;
    case PB_SYM_IMP : {
      SdPropBarSymImp  *barSymImp  = dynamic_cast<SdPropBarSymImp*>(SdWCommand::getModeBar(PB_SYM_IMP));
      barSymImp->getPropSymImp( &(mLocalProp.mSymImpProp) );
      }
      break;
    case PB_PART_IMP : {
      SdPropBarPartImp *barPartImp = dynamic_cast<SdPropBarPartImp*>(SdWCommand::getModeBar(PB_PART_IMP));
      barPartImp->getPropPartImp( &(mLocalProp.mPartImpProp) );
      }
      break;
    case PB_ROAD : {
      SdPropBarRoad *barRoad = dynamic_cast<SdPropBarRoad*>(SdWCommand::getModeBar(PB_ROAD) );
      barRoad->getPropRoad( &(mLocalProp.mRoadProp), &(mLocalProp.mViaProp), &(mLocalProp.mEnterType) );
      }
      break;
    case PB_POLYGON : {
      SdPropBarPolygon *barPolygon = dynamic_cast<SdPropBarPolygon*>(SdWCommand::getModeBar(PB_POLYGON) );
      barPolygon->getPropPolygon( &(mLocalProp.mPolygonProp), &(mLocalProp.mEnterType) );
      }
      break;
    }

  //Setup new properties
  mFragment.forEach( dctAll, [this] (SdObject *obj) -> bool {
    SdGraph *graph = dynamic_cast<SdGraph*>( obj );
    if( graph != nullptr )
      graph->setProp( mLocalProp );
    return true;
    });

  setDirtyCashe();
  update();
  }




void SdModeSelect::propSetToBar()
  {
  //Collect prop for selected objects
  mLocalProp.clear();
  mFragment.forEach( dctAll, [this] (SdObject *obj) -> bool {
    SdGraph *graph = dynamic_cast<SdGraph*>( obj );
    if( graph != nullptr )
      graph->getProp( mLocalProp );
    return true;
    });

  //Union layer of selected objects
  mLocalProp.unionLayer();
  //Set new bar
  mLocalProp.getPropBarId();
  SdWCommand::setModeBar( getPropBarId() );
  //Set prop into bar
  switch( getPropBarId() ) {
    case PB_LINEAR : {
      SdPropBarLinear  *barLinear  = dynamic_cast<SdPropBarLinear*>(SdWCommand::getModeBar(PB_LINEAR));
      barLinear->setPropLine( &(mLocalProp.mLineProp), getPPM(), mLocalProp.mEnterType );
      }
      break;
    case PB_TEXT : {
      SdPropBarTextual *barTextual = dynamic_cast<SdPropBarTextual*>(SdWCommand::getModeBar(PB_TEXT));
      barTextual->setPropText( &(mLocalProp.mTextProp), getPPM() );
      }
      break;
    case PB_WIRE : {
      SdPropBarWire    *barWire    = dynamic_cast<SdPropBarWire*>(SdWCommand::getModeBar(PB_WIRE));
      barWire->setPropWire( &(mLocalProp.mWireProp), getPPM(), mLocalProp.mEnterType, mLocalProp.mWireName.str() );
      }
      break;
    case PB_SYM_PIN : {
      SdPropBarSymPin  *barSymPin  = dynamic_cast<SdPropBarSymPin*>(SdWCommand::getModeBar(PB_SYM_PIN));
      barSymPin->setPropSymPin( &(mLocalProp.mSymPinProp) );
      }
      break;
    case PB_PART_PIN : {
      SdPropBarPartPin *barPartPin = dynamic_cast<SdPropBarPartPin*>(SdWCommand::getModeBar(PB_PART_PIN));
      barPartPin->setPropPartPin( &(mLocalProp.mPartPinProp) );
      }
      break;
    case PB_SYM_IMP : {
      SdPropBarSymImp  *barSymImp  = dynamic_cast<SdPropBarSymImp*>(SdWCommand::getModeBar(PB_SYM_IMP));
      barSymImp->setPropSymImp( &(mLocalProp.mSymImpProp) );
      }
      break;
    case PB_PART_IMP : {
      SdPropBarPartImp *barPartImp = dynamic_cast<SdPropBarPartImp*>(SdWCommand::getModeBar(PB_PART_IMP));
      barPartImp->setPropPartImp( &(mLocalProp.mPartImpProp) );
      }
      break;
    case PB_ROAD : {
      SdPropBarRoad *barRoad = dynamic_cast<SdPropBarRoad*>(SdWCommand::getModeBar(PB_ROAD) );
      barRoad->setPropRoad( &(mLocalProp.mRoadProp), &(mLocalProp.mViaProp), getPPM(), mLocalProp.mEnterType );
      }
      break;
    case PB_POLYGON : {
      SdPropBarPolygon *barPolygon = dynamic_cast<SdPropBarPolygon*>(SdWCommand::getModeBar(PB_POLYGON) );
      barPolygon->setPropPolygon( &(mLocalProp.mPolygonProp), getPPM(), mLocalProp.mEnterType, mObject->getProject()->netList() );
      }
      break;
    }
  }




void SdModeSelect::enterPoint( SdPoint point )
  {
  //GetBase()->GetProject()->Dirty();
  switch( getStep() ) {
    case smPaste : enterPaste( point ); break;
    default :
      //Проверить клавишу Shift, если не нажата то удалить предыдущее выделение
      if( !(checkPoint(point) & SEL_ELEM) ) {
        if( !mShift ) unselect( false );
        //Пройтись по объектам и выделить точкой
        mObject->forEach( dctAll & mask(), [point,this] (SdObject *obj) ->bool {
          SdGraph *graph = dynamic_cast<SdGraph*>(obj);
          if( graph != nullptr )
            graph->selectByPoint( point, &mFragment );
          return true;
          });
        //Обновить свойства и обновить изображение
        setDirtyCashe();
        propSetToBar();
        update();
        //Go to sel or unsel step
        setStep( mFragment.count() ? smSelPresent : smNoSelect );
        }
    }
  //Определить состояние курсора в текущей точке
  checkPoint( point );
  mEditor->setSelectionStatus( mFragment.count() != 0 );
  }





void SdModeSelect::clickPoint( SdPoint point )
  {
  if( getStep() != smPaste ) {
    //Проверить клавишу Shift, если не нажата то удалить предыдущее выделение
    if( !mShift ) unselect( false );
    //Пройтись по объектам и выделить точкой
    mObject->forEach( dctAll & mask(), [point,this] (SdObject *obj) ->bool {
      SdGraph *graph = dynamic_cast<SdGraph*>(obj);
      if( graph != nullptr ) {
        graph->selectByPoint( point, &mFragment );
        }
      return true;
      });
    //Обновить свойства и обновить изображение
    propSetToBar();
    //Определить состояние курсора в текущей точке
    checkPoint( point );
    }
  mEditor->setSelectionStatus( mFragment.count() != 0 );
  }



void SdModeSelect::cancelPoint( SdPoint point )
  {
  switch( getStep() ) {
    case smPaste : cancelPaste(); break;
    default :
      if( checkPoint(point) ) {
        activateMenu();
        return;
        }
      if( mFragment.count() ) unselect( true );
      else {
        cancelMode();
        return;
        }
    }
  setStep( smNoSelect );
  //Определить состояние курсора в текущей точке
  checkPoint( point );
  mEditor->setSelectionStatus( mFragment.count() != 0 );
  }




void SdModeSelect::movePoint( SdPoint p )
  {
  mCurPoint = p;
  switch( getStep() ) {
    case smPaste : dragCopy( p ); break;
    default      : checkPoint( p ); //Определить состояние курсора в текущей точке
    }
  }



bool SdModeSelect::wheel(SdPoint)
  {
  return false;
  }



void SdModeSelect::beginDrag( SdPoint point )
  {
  if( checkPoint(point) ) {
    //Над объектом
    if( mControl ) beginCopy( point );
    else beginMove( point );
    }
  else beginRect( point );
  }



void SdModeSelect::dragPoint( SdPoint point )
  {
  switch( getStep() ) {
    case smCopy    : dragCopy( point ); break;
    case smMove    : dragMove( point ); break;
    case smSelRect : dragRect( point ); break;
    }
  }



void SdModeSelect::stopDrag(SdPoint point)
  {
  dragPoint( point );
  switch( getStep() ) {
    case smCopy    : stopCopy( point ); break;
    case smMove    : stopMove( point ); break;
    case smSelRect : stopRect( point ); break;
    }
  mEditor->setSelectionStatus( mFragment.count() != 0 );
  }



bool SdModeSelect::enableCopy() const
  {
  return mFragment.count() != 0;
  }



bool SdModeSelect::enablePaste(quint64 pasteMask) const
  {
  return (mObject->getAcceptedObjectsMask() & pasteMask) != 0l;
  }



bool SdModeSelect::getInfo(SdPoint p, QString &info)
  {
  info.clear();
  mObject->forEach( dctAll, [p,&info] (SdObject *obj) ->bool {
    SdGraph *graph = dynamic_cast<SdGraph*>(obj);
    if( graph != nullptr )
      return !graph->getInfo( p, info, true );
    return true;
    });
  return !info.isEmpty();
  }




QString SdModeSelect::getStepHelp() const
  {
  switch( getStep() ) {
    default :
    case smNoSelect   : return QObject::tr( "No selected elements" );
    //Происходит выделение прямоугольником
    case smSelRect    : return QObject::tr( "Enter second corner for selection objects group" );
    //Есть выделение
    case smSelPresent : return QObject::tr( "There selected elements. You may them move, copy and edit");
    //Копирование
    case smCopy       : return QObject::tr( "Enter position for new elements" );
    //Перенос
    case smMove       : return QObject::tr( "Enter new position for selected elements" );
    //Указание базы для поворота
    case smRotateBase : return QObject::tr( "Enter center point for elements group rotation");
    //Указание направления поворота
    case smRotateDir  : return QObject::tr( "Enter point to define angle of rotation" );
    //Вставка из карамана
    case smPaste      : return QObject::tr( "Enter position for pasted elements" );
    }
  }




QString SdModeSelect::getModeThema() const
  {
  return QStringLiteral( MODE_HELP "ModeSelect.htm" );
  }




QString SdModeSelect::getStepThema() const
  {
  switch( getStep() ) {
    default :
    case smNoSelect   : return QStringLiteral( MODE_HELP "ModeSelect.htm#SelectNone" );
    //Происходит выделение прямоугольником
    case smSelRect    : return QStringLiteral( MODE_HELP "ModeSelect.htm#SelectSecondCorner" );
    //Есть выделение
    case smSelPresent : return QStringLiteral( MODE_HELP "ModeSelect.htm#SelectPresent" );
    //Копирование
    case smCopy       : return QStringLiteral( MODE_HELP "ModeSelect.htm#SelectInsertPoint" );
    //Перенос
    case smMove       : return QStringLiteral( MODE_HELP "ModeSelect.htm#SelectPlacePoint" );
    //Указание базы для поворота
    case smRotateBase : return QStringLiteral( MODE_HELP "ModeSelect.htm#SelectRotateCenter" );
    //Указание направления поворота
    case smRotateDir  : return QStringLiteral( MODE_HELP "ModeSelect.htm#SelectRotateAngle" );
    //Вставка из карамана
    case smPaste      : return QStringLiteral( MODE_HELP "ModeSelect.htm#SelectPaste" );
    }
  }



int SdModeSelect::getCursor() const
  {
  switch( getStep() ) {
    case smNoSelect :
    case smSelPresent :
      if( mState & SEL_ELEM ) return mControl ? CUR_TAKE_COPY : CUR_TAKE;
      else if( mState & UNSEL_ELEM ) return CUR_HAND;
      return CUR_POINT;
    case smSelRect    : return CUR_SEL;
    case smCopy       : return CUR_COPY;
    case smMove       : return CUR_MOVE;
    case smRotateBase : return CUR_ROTATE;
    case smRotateDir  : return CUR_ROTATE;
    case smPaste      : return CUR_ARROW;
    }
  return CUR_ARROW;
  }




int SdModeSelect::getIndex() const
  {
  return MD_SELECT;
  }





void SdModeSelect::keyDown(int key, QChar ch)
  {
  switch( key ) {
    case Qt::Key_Copy    : copy();  break;
    case Qt::Key_Paste   : paste(); break;
    case Qt::Key_Cut     : cut();   break;
    case Qt::Key_Select  : selectAll(); break;
    case Qt::Key_Shift   : mShift = true; break;
    case Qt::Key_Control : mControl = true; break;
    case Qt::Key_Delete  : deleteSelected(); break;
      //TODO D008 special codes
//    case vkDubl     : SetDirty(); Dublicate(); break;
//    case vkParam    : SetDirty(); Param(); break;
//    case vkNumerate : SetDirty(); Numerate(); break;
    case Qt::Key_F7 :
      setDirty();
      //Поворот как группы компонентов
      if( mFragment.count() && getStep() == smSelPresent ) {
        mFragment.forEach( dctAll, [this] (SdObject *obj) -> bool {
          SdGraph *graph = dynamic_cast<SdGraph*>(obj);
          if( graph != nullptr )
            graph->rotate( mCurPoint, da90 );
          return true;
          });
        }
      break;
    default : SdMode::keyDown( key, ch );
    }
  mEditor->viewport()->setCursor( loadCursor(getCursor()) );
  update();
  }



void SdModeSelect::keyUp(int key, QChar ch)
  {
  Q_UNUSED(ch)
  switch( key ) {
    case Qt::Key_Shift   : mShift = false; break;
    case Qt::Key_Control : mControl = false; break;
    }
  mEditor->viewport()->setCursor( loadCursor(getCursor()) );
  }




void SdModeSelect::copy()
  {
  //If selection present, then put it into clipboard
  if( mFragment.count() ) {
    //Calculate and write base fragment origin
    //Вычислить и записать базовую точку фрагмента
    SdRect over = mFragment.getOverRect();
    //Align to current grid
    SdPoint a = over.getBottomLeft();
    SdPoint grid = mEditor->gridGet();
    a.setX( (a.x() / grid.x()) * grid.x() );
    a.setY( (a.y()  / grid.y()) * grid.y() );

    mFragment.setOrigin( a );

    //Write to clipboard
    mFragment.putToClipboard( mObject->getProject(), mEditor->scaleGet() );
    }
  }




void SdModeSelect::cut()
  {
  setDirty();
  copy();
  deleteSelected();
  }




void SdModeSelect::paste()
  {
  setDirty();
  setDirtyCashe();
  if( mFragment.count() )
    unselect( false );
  cancelPaste();
  mPastePrj = mPaste.getFromClipboard();
  if( mPastePrj != nullptr ) {
    mFirst = mPaste.getOrigin();
    mPrevMove = mFirst;
    setStep( smPaste );
    }
  update();
  }




void SdModeSelect::selectAll()
  {
  //Select all objects
  mObject->forEach( dctAll & mask(), [this] (SdObject *obj) -> bool {
    SdGraph *graph = dynamic_cast<SdGraph*>(obj);
    if( graph != nullptr )
      graph->select( &mFragment );
    return true;
    });
  //update
  propSetToBar();
  //Go to sel or unsel step
  setStep( mFragment.count() ? smSelPresent : smNoSelect );
  //Update edit menu (copy, paste, delete)
  mEditor->setSelectionStatus( mFragment.count() != 0 );
  }







void SdModeSelect::deleteSelected()
  {
  if( mFragment.count() ) {
    mUndo->begin( QObject::tr("Deletion elements"), mObject );
    mFragment.forEach( dctAll, [this] (SdObject *obj) ->bool {
      SdGraph *graph = dynamic_cast<SdGraph*>(obj);
      if( graph != nullptr )
        graph->deleteObject( mUndo );
      return true;
      });
    mFragment.removeAll();

    setDirty();
    setDirtyCashe();
    update();
    }
  }




void SdModeSelect::unselect(bool update)
  {
  if( mFragment.count() ) {

    //mUndo->begin( QObject::tr("Finish selection"), mObject );
    moveComplete();

    mFragment.removeAll();

    //Special case for pcb's
    SdPItemPlate *plate = dynamic_cast<SdPItemPlate*>(mObject);
    if( plate != nullptr ) plate->setDirtyRatNet();

    if( update ) {
      setDirty();
      setDirtyCashe();
      }
    }
  }




SdRect SdModeSelect::getFragmentOver()
  {
  return mFragment.getOverRect();
  }




SdSelector *SdModeSelect::getFragment()
  {
  return &mFragment;
  }




int SdModeSelect::checkPoint(SdPoint p)
  {
  int prevStatus = mState;
  //Reset status
  mState = 0;

  //Get state object behind cursor
  mObject->forEach( dctAll & mask(), [this,p] (SdObject *obj) ->bool {
    SdGraph *graph = dynamic_cast<SdGraph*>(obj);
    if( graph != nullptr )
      mState |= graph->behindCursor(p);
    return true;
    });

  if( sdEnvir->mShowMessageRemark ) {
    //If need display extended remark then scan objects behind cursor and get their info
    QString info;
    getInfo( p, info );

    //If message received then display it other wise display help
    if( info.isEmpty() )
      SdPulsar::sdPulsar->setStatusMessage( getStepHelp() );
    else
      SdPulsar::sdPulsar->setStatusMessage( info );
    }

  if( mState != prevStatus )
    mEditor->viewport()->setCursor( loadCursor(getCursor()) );

  return mState;
  }




void SdModeSelect::beginCopy(SdPoint p)
  {
  setDirty();
  //Поскольку это копирование, то нужно выделить объекты целиком (пользователь мог выделить и частично)
  mFragment.forEach( dctAll, [this] (SdObject *obj) -> bool {
    SdGraph *graph = dynamic_cast<SdGraph*>(obj);
    if( graph != nullptr )
      graph->select( &mFragment );
    return true;
    });
  mFragment.setOrigin( p );
  cancelPaste();
  mPaste = mFragment;
  mFirst = p;
  mPrevMove = p;
  setStep( smCopy );
  //Пусть рисуется заново
  update();
  }




void SdModeSelect::dragCopy(SdPoint p)
  {
  mPrevMove = p;
  update();
  }




void SdModeSelect::stopCopy(SdPoint p)
  {
  dragCopy( p );
  if( p != mFirst ) {
    //Произвести вставку
    //Perform inserting
    insertCopy( p.sub( mFirst ), false );
    }
  setStep( smSelPresent );
  //When get from bar in this point then changed properties of selected components
  //propGetFromBar();
  }




//Moving prepare
void SdModeSelect::beginMove(SdPoint p)
  {
  if( mFragment.count() ) {
    mUndo->begin( QObject::tr("Move begin"), mObject );

    //Moving prepare
    mFragment.forEach( dctAll, [this] (SdObject *obj) ->bool {
      SdGraph *graph = dynamic_cast<SdGraph*>(obj);
      if( graph != nullptr )
        graph->prepareMove( mUndo );
      return true;
      });

    //Save state of all object before moving
    mFragment.forEach( dctAll, [this] (SdObject *obj) ->bool {
      SdGraph *graph = dynamic_cast<SdGraph*>(obj);
      if( graph != nullptr )
        graph->saveState( mUndo );
      return true;
      });

    mFirst = p;
    mPrevMove = p;
    setStep( smMove );
    setDirty();
    setDirtyCashe();
    update();
    }
  }



//Moving process
void SdModeSelect::dragMove(SdPoint p)
  {
  SdPoint offset = p.sub(mPrevMove);
  mFragment.forEach( dctAll, [offset] (SdObject *obj) ->bool {
    SdGraph *graph = dynamic_cast<SdGraph*>(obj);
    if( graph != nullptr )
      graph->move( offset );
    return true;
    });
  mPrevMove.move( offset );
  update();
  }




//Moving complete
void SdModeSelect::stopMove(SdPoint p)
  {
  dragMove(p);
  //moveComplete();
  setStep(smSelPresent);
  }



//Begin selection by rect
void SdModeSelect::beginRect(SdPoint point)
  {
  mFirst = point;
  mPrevMove = point;
  setStep( smSelRect );
  update();
  }



//Selection by rect - selection process
void SdModeSelect::dragRect(SdPoint p)
  {
  mPrevMove = p;
  update();
  }



//Selection by rect - selection complete, accumulate selected elements
void SdModeSelect::stopRect(SdPoint p)
  {
  //Проверить клавишу Shift, если не нажата то удалить предыдущее выделение
  if( !mShift ) mFragment.removeAll();
  //Пройтись по объектам и выделить прямоугольником
  SdRect r(mFirst,p);
  mObject->forEach( dctAll & mask(), [this,r] (SdObject *obj) ->bool {
    SdGraph *graph = dynamic_cast<SdGraph*>(obj);
    if( graph != nullptr )
      graph->selectByRect( r, &mFragment );
    return true;
    });
  //Обновить свойства и обновить изображение
  propSetToBar();
  //Определить состояние курсора в текущей точке
  checkPoint( p );
  //Go to sel or unsel step
  setStep( mFragment.count() ? smSelPresent : smNoSelect );
  }








void SdModeSelect::enterPaste(SdPoint point)
  {
  if( mPaste.count() ) {
    setDirty();
    //Unselect selected objects
    unselect(false);
    mUndo->begin( QObject::tr("Insert from clipboard"), mObject );
    //Insert copy of pasted elements into object without selection them
    mObject->insertObjects( point.sub( mFirst ), &mPaste, mUndo, mEditor, &mFragment, false );
    cancelPaste();
    }
  }




void SdModeSelect::cancelPaste()
  {
  mPaste.clear();
  if( mPastePrj != nullptr )
    delete mPastePrj;
  mPastePrj =  nullptr;
  setStep( mFragment.count() ? smSelPresent : smNoSelect );
  }




void SdModeSelect::showRect(SdContext *ctx)
  {
  ctx->setPen( 0, sdEnvir->getSysColor(scEnter), dltDotted );
  ctx->rect( SdRect(mFirst, mPrevMove) );
  }




void SdModeSelect::insertCopy(SdPoint offset, bool next)
  {
  setDirty();
  setDirtyCashe();
  mUndo->begin( QObject::tr("Copy insertion"), mObject );
  //Произвести вставку
  //Perform insertion
  unselect(false);
  mObject->insertObjects( offset, &mPaste, mUndo, mEditor, &mFragment, next );
  cancelPaste();
  }




void SdModeSelect::activateMenu()
  {
  mEditor->contextMenu( SdWCommand::menuSelect );
  }




void SdModeSelect::moveComplete()
  {
  mFragment.forEach( dctAll, [this] (SdObject *obj) -> bool {
    SdGraph *graph = dynamic_cast<SdGraph*>(obj);
    if( graph != nullptr )
      graph->moveComplete( mEditor->gridGet(), mUndo );
    return true;
    });
  }




//Return current disable mask
quint64 SdModeSelect::mask() const
  {
  quint64 msk = 0l;
  if( !sdEnvir->mEnableComp ) msk = dctSymImp | dctPartImp;
  if( !sdEnvir->mEnableNet ) msk |= dctNetName | dctNetWire | dctTracePolygon | dctTraceRoad | dctTraceVia;
  if( !sdEnvir->mEnablePic ) msk |= dctPicture;
  return ~msk;
  }
