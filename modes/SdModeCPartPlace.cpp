/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdModeCPartPlace.h"
#include "objects/SdGraphPartImp.h"
#include "objects/SdPulsar.h"
#include "objects/SdEnvir.h"
#include "windows/SdPropBarPartImp.h"
#include "windows/SdWCommand.h"

#include <QTransform>

SdModeCPartPlace::SdModeCPartPlace(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdModeCommon( editor, obj )
  {

  }


void SdModeCPartPlace::activate()
  {
  if( !mBySheet ) {
    //Установить шаг 0
    setStep(psmNoSelect);
    //Установить панель свойств
    SetPropBar( GetPropBarId(), &localProp );
    //Накопить таблицу компонентов
    info.table.Clear();
    GetPlate()->ForEachComp( PMAccumIterator(info.table) );
    //Перейти к выбору следующего компонента
    NextComp();
    }
  else GetAllProp();
  }



void SdModeCPartPlace::drawStatic(SdContext *ctx)
    {
    }

void SdModeCPartPlace::drawDynamic(SdContext *ctx)
    {
    }




int SdModeCPartPlace::getPropBarId() const
  {
  return PB_PART_IMP;
  }




void SdModeCPartPlace::propGetFromBar()
  {
  saveStateOfSelectedObjects( QObject::tr("Properties changed") );

  if( mFragment.count() ) setDirty();

  //Get new props
  mLocalProp.clear();

  SdPropBarPartImp *barPartImp = dynamic_cast<SdPropBarPartImp*>(SdWCommand::getModeBar(getPropBarId()));
  barPartImp->getPropPartImp( &(mLocalProp.mPartImpProp) );

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





void SdModeCPartPlace::propSetToBar()
  {
  //Collect prop for selected objects
  mLocalProp.clear();
  mFragment.forEach( dctPartImp, [this] (SdObject *obj) -> bool {
    SdGraph *graph = dynamic_cast<SdGraph*>( obj );
    if( graph != nullptr )
      graph->getProp( mLocalProp );
    return true;
    });

  //Set new bar
  //SdWCommand::setModeBar( PB_PART_IMP );
  SdPropBarPartImp *barPartImp = dynamic_cast<SdPropBarPartImp*>(SdWCommand::getModeBar(getPropBarId()));
  barPartImp->setPropPartImp( &(mLocalProp.mPartImpProp) );
  }




void SdModeCPartPlace::enterPoint(SdPoint p)
  {
  switch( getStep() ) {
    case psmNoSelect : //Нет выделения
      mFirst = p;
      if( mBehindCursorTable.count() ) {
        //There are components behind cursor [Под курсором есть компоненты]
        SdGraphPartImp *part = dynamic_cast<SdGraphPartImp*>(mBehindCursorTable[mBehindCursorIndex]);
        if( part ) {
          part->select( &mFragment );
          //part->SelectNets( fragment );
          mPrevMove = p;
          propSetToBar();
          setStep( psmMove );
          }
        }
      break;
    case psmMove :
      movePoint( p );
      setStep( psmNoSelect );
      unselect();
      checkPoint( p );
      nextComponent();
      break;
    }
  setDirtyCashe();
  update();
  }




void SdModeCPartPlace::cancelPoint(SdPoint)
  {
  mBySheet = false;
  if( getStep() == psmNoSelect ) cancelMode();
  else if( getStep() == psmMove ) {
    unselect();
    setStep( psmNoSelect );
    }
  update();
  }




void SdModeCPartPlace::movePoint(SdPoint p)
  {
  if( getStep() == psmMove && mPrevMove != p ) {
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
  }





void SdModeCPartPlace::keyDown(int key, QChar ch)
  {
  switch( key ) {
    case Qt::Key_F2 :
      //Component rotation [Поворот]
      if( getStep() == psmMove ) {
        mLocalProp.mPartImpProp.mAngle += 90000;
        setPropertiesToComponent();
        }
      break;
    case Qt::Key_F3 :
      //Flip component to other pcb side [Перенос на другую сторону платы]
      if( getStep() == psmMove ) {
        mLocalProp.mPartImpProp.mSide = mLocalProp.mPartImpProp.mSide == stmTop ? stmBottom : stmTop;
        setPropertiesToComponent();
        }
      break;
//    case vkF4 :
//      //Зеркальность
//      if( GetStep() == psmMove ) {
//        localProp.prtImpProp.mirror = localProp.prtImpProp.mirror == dmNoMirror ? dmMirror : dmNoMirror;
//        DAutomation( DAC_SETPROP, 0, 0, &localProp );
//        PropChanged( dc );
//        }
//      break;
    case Qt::Key_F5 :
      //Операция раздвижки
      mInsertFlag = !mInsertFlag;
      break;
    case Qt::Key_F6 :
      //Roll components behind cursor [Перебор компонентов под курсором]
      if( getStep() == psmNoSelect && mBehindCursorTable.count() > 1 ) {
        mBehindCursorIndex++;
        mBehindCursorIndex %= mBehindCursorTable.count();
        //Show component, which will select by left button
        //Отобразить компонент, который будет выделен при нажатии левой кнопки
        mBehindCursorPrt = dynamic_cast<SdGraphPartImp*>(mBehindCursorTable[mBehindCursorIndex]);
        SdPulsar::sdPulsar->setStatusMessage( getStepHelp() );
        }
      break;
    case Qt::Key_F7 :
      //Component group rotation [Поворот как группы компонентов]
      if( mFragment.count() && getStep() == psmMove ) {
        //Save state of selected objects
        saveStateOfSelectedObjects( QObject::tr("Group rotation") );
        //Perform rotation
        mFragment.forEach( dctPartImp, [this] (SdObject *obj) -> bool {
          SdGraphPartImp *imp = dynamic_cast<SdGraphPartImp*>( obj );
          if( imp ) imp->rotate( mPrevMove, SdPropAngle(90000) );
          return true;
          });
        update();
        }
      break;
    case Qt::Key_F8 :
      place();
      break;
    case Qt::Key_Shift :
      mShiftKey = true;
      break;
    }
  SdModeCommon::keyDown( key, ch );
  }




void SdModeCPartPlace::keyUp(int key, QChar ch)
  {
  if( key == Qt::Key_Shift )
    mShiftKey = false;
  SdModeCommon::keyUp( key, ch );
  }



SdPoint SdModeCPartPlace::enterPrev()
  {
  if( getStep() == psmNoSelect ) {
    //Component selection by smart algorithm [Выбор компонента по алгоритму Smart]
    mObject->forEach( dctPartImp, [this] (SdObject *obj) -> bool {
      SdGraphPartImp *imp = dynamic_cast<SdGraphPartImp*>( obj );
      //If ident of component match to search, then perform selection of component
      if( imp && imp->getIdent() == mSmartName ) {
        imp->select( &mFragment );
        //Break iteration because component is found
        return false;
        }
      return true;
      } );
    if( mFragment.count() ) {
      //Component selected
      //Get its properties
      propSetToBar();
      //And start move it
      setStep( psmMove );
      update();
      }
    }
  }




void SdModeCPartPlace::beginDrag(SdPoint p)
  {
  if( getStep() == psmNoSelect ) {
    //No selection [Нет выделения]
    if( !mBehindCursorTable.count() ) {
      //Outside components [Вне компонента]
      mFirst    = p;
      mPrevMove = p;
      setStep( psmBeingSelRect );
      update();
      }
    }
  }




void SdModeCPartPlace::dragPoint(SdPoint p)
  {
  if( mBySheet ) movePoint( p );
  else {
    mPrevMove = p;
    update();
    }
  }




void SdModeCPartPlace::stopDrag(SdPoint p)
  {
  if( getStep() == psmBeingSelRect ) {
    //Selection by rect completed [Выделение прямоугольником завершено]
    SdRect r(mFirst,p);
    mObject->forEach( dctPartImp, [this,r] (SdObject *obj) -> bool {
      SdGraphPartImp *imp = dynamic_cast<SdGraphPartImp*>( obj );
      if( imp )
        imp->selectByRect( r, &mFragment );
      return true;
      });
    mPrevMove = p;
    mFragment.setOrigin( p );
    propSetToBar();
    setStep( mFragment.count() ? psmMove : psmNoSelect );
    setDirtyCashe();
    update();
    mFirst = p;
    }
  }




bool SdModeCPartPlace::getInfo(SdPoint p, QString &info)
    {
    }




QString SdModeCPartPlace::getStepHelp() const
  {
  if( getStep() == psmBeingSelRect )
    //Укажите второй угол для выделения группы компонентов
    return QObject::tr("Point second corner for selection components group");
  else if( getStep() == psmMove )
    //Размещай компоненты! Левая кнопка - твой вариант, средняя - мой, правая - отмена.
    return QObject::tr("Place components! Left button - at cursor position, middle - smart position, right - cancel");
  if( mBehindCursorTable.count() ) {
    if( mBehindCursorPrt ) {
      QString str = mBehindCursorPrt->getIdent();
      QString temp( QObject::tr("Left button: %1;   Middle button: %2") );
      return temp.arg(str).arg(mSmartName);
      }
    }
  //    "Наведи мышь и выбери компонент для расстановки, средняя - ",
  return QObject::tr("Press left button on component to select for placing, middle button: ") + mSmartName;
  }




QString SdModeCPartPlace::getModeThema() const
  {
  return QStringLiteral( MODE_HELP "ModeCPartPlace.htm" );
  }




QString SdModeCPartPlace::getStepThema() const
  {
  switch( getStep() ) {
    case psmNoSelect :     //Нет выделения
      return QStringLiteral( MODE_HELP "ModeCPartPlace.htm#Select" );
    case psmBeingSelRect : //Происходит выделение прямоугольником
      return QStringLiteral( MODE_HELP "ModeCPartPlace.htm#Corner" );
    case psmMove :         //Перенос одного или нескольких элементов
      return QStringLiteral( MODE_HELP "ModeCPartPlace.htm#Place" );
    }
  return getModeThema();
  }




int SdModeCPartPlace::getCursor() const
  {
  switch( getStep() ) {
    //Is selection by rect
    //[Происходит выделение прямоугольником]
    case psmBeingSelRect : return CUR_SEL;
    //No selection [Нет выделения]
    case psmNoSelect :
      //CheckPoint(prevMove);
      //Позаботиться о строке состояния
      //GetViewer()->SetMessage( GetStepHelp() );
      return mBehindCursorTable.count() ? CUR_TAKE : CUR_POINT;
    default :
    case psmMove : return CUR_PLACE;
    }
  }



int SdModeCPartPlace::getIndex() const
  {
  return MD_PLACE;
  }





void SdModeCPartPlace::unselect()
  {
  //Отменить выделение резинок
  GetPlate()->ForEachNet( DIPlateNetUnselect() );
  //Пометить развыделенные компоненты как размещенные и обновить обнаруженную группу
  fragment.ForEach( PMUnselectIterator( info.table, info.patSour ) );
//  SdGraphPartImp *imp = dynamic_cast<SdGraphPartImp*>( obj );

  //Расчитать следующий smart - копонент, за основу берем последний
  SdGraphPartImp *last = dynamic_cast<SdGraphPartImp*>( mFragment.count() ? mFragment.first() : nullptr );
  if( last )
    mSmartName = last->getIdent();
  else
    mSmartName = "R1";


  if( sdEnvir->mPlaceMode == dpmNextNumber ) {
    //Следующий по номеру в группе
    DName Name( smartName );
    CreateNextName( Name, Name );
    smartName = Name;
    }
  else {
    //Следующий ближайший не размещенный
    if( bigCompIndex >= 0 ) {
      int sheetIndex = info.table[bigCompIndex].sheet;
      int target = -1;
      DPoint from(info.table[bigCompIndex].inShem);
      int distance = illegalRatLen;
      for( NCount i = 0; i < info.table.GetNumber(); ++i )
        if( info.table[i].placed || info.table[i].sheet != sheetIndex ) continue;
        else if( from.GetDistance( info.table[i].inShem ) < distance ) {
          distance = from.GetDistance( info.table[target = i].inShem );
          }
      if( target >= 0 ) smartName = info.table[target].pos;
      else {
        DName Name( smartName );
        CreateNextName( Name, Name ); //В случае отсутствия варианта - следующий по порядку
        smartName = Name;
        }
      }
    else {
      DName Name( smartName );
      CreateNextName( Name, Name ); //В случае отсутствия варианта - следующий по порядку
      smartName = Name;
      }
    }

  //Убрать компоненты из выделения
  fragment.RemoveAll();  //Объекты становятся невыделенными
  GetAllProp();
  }



//Set properties to component
void SdModeCPartPlace::setPropertiesToComponent()
  {
  saveStateOfSelectedObjects( QObject::tr("Properties changed") );

  if( mFragment.count() ) setDirty();

  //Setup new properties
  mFragment.forEach( dctPartImp, [this] (SdObject *obj) -> bool {
    SdGraph *graph = dynamic_cast<SdGraph*>( obj );
    if( graph != nullptr )
      graph->setProp( mLocalProp );
    return true;
    });

  setDirtyCashe();
  update();
  }





//Save state of selected objects
void SdModeCPartPlace::saveStateOfSelectedObjects( const QString undoTitle )
  {
  if( mFragment.count() ) {
    mUndo->begin( undoTitle, mObject );
    //Сохранить состояние объектов до изменения свойств
    mFragment.forEach( dctPartImp, [this] (SdObject *obj) -> bool {
      SdGraph *graph = dynamic_cast<SdGraph*>( obj );
      if( graph != nullptr )
        graph->saveState( mUndo );
      return true;
      });
    }
  }




//Check components behind point
void SdModeCPartPlace::checkPoint(SdPoint p)
  {
  mBehindCursorTable.clear();     //Очистить таблицу компонентов под курсором
  mObject->forEach( dctPartImp, [this,p] (SdObject *obj) -> bool {
    SdGraphPartImp *imp = dynamic_cast<SdGraphPartImp*>( obj );
    if( imp && imp->behindCursor(p) )
      //TODO D052 mask components by side
      mBehindCursorTable.append( imp );
    return true;
    });
  mBehindCursorIndex = 0;
  if( mBehindCursorTable.count() )
    mBehindCursorPrt = dynamic_cast<SdGraphPartImp*>(mBehindCursorTable[mBehindCursorIndex]);
  else
    mBehindCursorPrt = nullptr;
  SdPulsar::sdPulsar->setStatusMessage( getStepHelp() );
  }



////Накопить свойства
//void
//DPlaceMode::GetAllProp() {
//  SetDirty();
//  GetUndo()->BeginAction();
//  //Сохранить состояние объектов до переноса
//  fragment.ForEach( DISaveState() );
//  //Очистить свойства
//  localProp.Clear();
//  //Накопить свойства
//  int mask; //Переменная заменитель, в данном режиме не используется
//  fragment.ForEach( GetPropIterator( localProp, mask ) );
//  GetUndo()->EndAction( "Размещение" );
//  //Обновить панель свойств
//  DAutomation( DAC_SETPROP, 0, 0, &localProp );
//  }


//DPrtImpPic*
//DPlaceMode::FindComp( CPChar name ) {
//  return GetPlate()->FindCompById( name );
//  }

//void
//DPlaceMode::PlaceGroupFun() {
//  //Размещение группы по образцу
//  NSeparator
//    sour(info.patSour),
//    dest(info.patDest);
//  while( sour.Separate( ',' ) && dest.Separate( ',' )  ) {
//    //Найти прототип
//    DPrtImpPic *partSour = FindComp( sour.GetLastItem() );
//    DPrtImpPic *partDest = FindComp( dest.GetLastItem() );
//    if( partSour && partDest ) {
//      localProp.Clear(); //Очистить свойства
//      partSour->GetPProp( localProp );   //Получить свойства
//      partDest->SetPProp( localProp );   //Установить свойства
//      partDest->Move( partSour->GetOrigin() - partDest->GetOrigin() ); //Перенести
//      partDest->SelectNets( fragment );   //Выделить
//      prevMove = partDest->GetOrigin(); //За точку привязки принять самый последний элемент
//      }
//    else break;
//    }
//  //Размещение закончилось, выделить
//  first = prevMove;
//  GetAllProp();
//  SetStep( psmMove ); //Инициировать перемещение
//  }


//void
//DPlaceMode::PlaceGroupFun() {
//  //Размещение группы по образцу
//  NSeparator
//    sour(info.patSour),
//    dest(info.patDest);
//  while( sour.Separate( ',' ) && dest.Separate( ',' )  ) {
//    //Найти прототип
//    DPrtImpPic *partSour = FindComp( sour.GetLastItem() );
//    DPrtImpPic *partDest = FindComp( dest.GetLastItem() );
//    if( partSour && partDest ) {
//      localProp.Clear(); //Очистить свойства
//      partSour->GetPProp( localProp );   //Получить свойства
//      partDest->SetPProp( localProp );   //Установить свойства
//      partDest->Move( partSour->GetOrigin() - partDest->GetOrigin() ); //Перенести
//      partDest->SelectNets( fragment );   //Выделить
//      prevMove = partDest->GetOrigin(); //За точку привязки принять самый последний элемент
//      }
//    else break;
//    }
//  //Размещение закончилось, выделить
//  first = prevMove;
//  GetAllProp();
//  SetStep( psmMove ); //Инициировать перемещение
//  }



//void
//DPlaceMode::SelectByDialog() {
//  while(1) {
//    switch( ExecuteDialog() ) {
//      case placeCancel :
//        //Прекращение режима
//        //CancelMode();
//        SetStep( psmNoSelect );
//        //CheckPoint( cursor );
//        break;
//      case placeComp :
//        //Размещение компонента
//        smartName = info.prev;
//        GetBase()->ForEach( SelectByNameIterator( smartName, fragment, prevMove ) );
//        if( fragment.GetNumber() ) {
//          GetAllProp();
//          SetStep( psmMove );
//          }
//        else continue;
//        break;
//      case placeGroup :
//        PlaceGroupFun();
//        //initMove();
//        break;
//      case placeSheet :
//        bySheet = true;
//        NextComp();
//        break;
//      }
//    break;
//    }
//  //Сбросить главный компонент
//  bigCompPins = 0;
//  Update();
//  }
