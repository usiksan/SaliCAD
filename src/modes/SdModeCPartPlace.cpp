/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Part implement placement mode
*/
#include "SdModeCPartPlace.h"
#include "objects/SdGraphPartImp.h"
#include "objects/SdPulsar.h"
#include "objects/SdEnvir.h"
#include "objects/SdProject.h"
#include "objects/SdPItemSheet.h"
#include "objects/SdPItemPlate.h"
#include "objects/SdPulsar.h"
#include "objects/SdConverterOffset.h"
#include "windows/SdPropBarPartPlace.h"
#include "windows/SdWCommand.h"
#include "windows/SdWEditorGraph.h"



SdModeCPartPlace::SdModeCPartPlace(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdModeCommon( editor, obj ),
  mBehindCursorPrt(nullptr),   //Компонент, подлежащий выделению при нажатии левой кнопки мыши
  mShiftKey(false),
  mInsertFlag(false),        //Автораздвижка компонентов
  mSmartOrNextId(false),
  mBySheet(false),           //Истина, когда производится выбор из листа
  mBigCompPins(0),       //Количество выводов главного компонента
  mBigCompIndex(-1),      //Индекс главного компонента в таблице компонентов
  mSideMask(stmTop | stmBottom)
  //DLineProp         lineProp;          //Свойства для выделения прямоугольником
  {

  }


void SdModeCPartPlace::activate()
  {

  SdPoint grid = plate()->mPlaceGrid;
  if( grid.x() <= 0 || grid.y() <= 0 )
    plate()->mPlaceGrid = mEditor->gridGet();
  mPreviousGrid = mEditor->gridGet();
  mEditor->gridSet( plate()->mPlaceGrid );

  mPreviousCursor = SdEnvir::instance()->mCursorAlignGrid;
  SdEnvir::instance()->mCursorAlignGrid = plate()->mPlaceCursorGrid;

  if( !mBySheet )
    reset();

  }





void SdModeCPartPlace::deactivate()
  {
  //Store to plate current grid and cursor alignment mode
  plate()->mPlaceGrid = mEditor->gridGet();
  plate()->mPlaceCursorGrid = SdEnvir::instance()->mCursorAlignGrid;

  //Restore previous grid and cursor alignment mode
  mEditor->gridSet( mPreviousGrid );
  SdEnvir::instance()->mCursorAlignGrid = mPreviousCursor;
  }




void SdModeCPartPlace::reset()
  {
  unselect();
  //Установить шаг 0
  setStep(psmNoSelect);
  //update step definite params
  SdPulsar::sdPulsar->emitSetStatusMessage( getStepHelp() );
  mEditor->viewport()->setCursor( loadCursor(getCursor()) );

  //setup mode step properties bar
  SdWCommand::setModeBar( getPropBarId() );
  //setup properties in bar
  propSetToBar();

  SdPropBarPartPlace *barPartPlace = dynamic_cast<SdPropBarPartPlace*>(SdWCommand::getModeBar(getPropBarId()));
  barPartPlace->setSmartMode( mSmartOrNextId );
  mSideMask = barPartPlace->sideMask();

  //Accum existing sheet lists
  QStringList sheetList;
  if( mObject->getProject() )
    mObject->getProject()->forEach( dctSheet, [&sheetList] (SdObject *obj) ->bool {
      SdPItemSheet *sheet = dynamic_cast<SdPItemSheet*>(obj);
      if( sheet )
        sheetList.append( sheet->getTitle() );
      return true;
      });
  //Setup sheet list and current sheet
  barPartPlace->setSheetList( sheetList, mCurrentSheet );

  //Accum component list
  QStringList compList;
  mObject->forEach( dctPartImp, [&compList] (SdObject *obj) -> bool {
    SdGraphPartImp *imp = dynamic_cast<SdGraphPartImp*>(obj);
    if( imp )
      compList.append( imp->ident() );
    return true;
    });
  //Setup component list
  barPartPlace->setComponentList( compList );

  update();
  }





void SdModeCPartPlace::drawStatic(SdContext *ctx)
  {
  //Draw all object except selected
  mObject->forEach( dctAll, [this, ctx] (SdObject *obj) -> bool {
    SdGraph *graph = dynamic_cast<SdGraph*>( obj );
    if( graph != nullptr && graph->getSelector() != &mFragment )
      graph->draw( ctx );
    return true;
    });
  }




void SdModeCPartPlace::drawDynamic(SdContext *ctx)
  {
  //Draw all selected elements
  ctx->setOverColor( SdEnvir::instance()->getSysColor(scSelected) );
  mFragment.draw( ctx );

  //If show rat net and selection present
  if( SdEnvir::instance()->mShowRatNet && mFragment.count() ) {
    //Draw rat net pairs for selected components
    ctx->setPen( 0, SdEnvir::instance()->getSysColor(scRatNet), dltSolid );
    mFragment.forEach( dctPartImp, [this,ctx] (SdGraph *graph) -> bool {
      SdGraphPartImp *imp = dynamic_cast<SdGraphPartImp*>(graph);
      if( imp )
        imp->drawRatNet( ctx, mNetList );
      return true;
      });
    }

  //On according step
  if( getStep() == psmBeingSelRect ) {
    ctx->setPen( 0, SdEnvir::instance()->getSysColor(scEnter), dltDotted );
    ctx->rect( SdRect(mFirst, mPrevMove) );
    }
  }




int SdModeCPartPlace::getPropBarId() const
  {
  return PB_PART_PLACE;
  }




void SdModeCPartPlace::propGetFromBar()
  {
  //saveStateOfSelectedObjects( QObject::tr("Properties changed") );

  SdPropBarPartPlace *barPartPlace = dynamic_cast<SdPropBarPartPlace*>(SdWCommand::getModeBar(getPropBarId()));

  //Common assignments
  mBySheet       = barPartPlace->isSheetSelection();
  mCurrentSheet  = barPartPlace->sheet();
  mSmartOrNextId = barPartPlace->isSmartMode();
  mSideMask      = barPartPlace->sideMask();

  if( mFragment.count() ) {

    setDirty();

    //Get new props
    mLocalProp.clear();

    barPartPlace->getPropPartImp( mLocalProp.mPartImpProp );

    //Setup new properties
    mFragment.forEach( dctAll, [this] (SdObject *obj) -> bool {
      SdGraph *graph = dynamic_cast<SdGraph*>( obj );
      if( graph != nullptr )
        graph->setProp( mLocalProp );
      return true;
      });
    }

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
  barPartImp->setPropPartImp( mLocalProp.mPartImpProp );

  dirtyRatNet();
  }




void SdModeCPartPlace::partSelect(QStringList list)
  {
  for( QString &id : list ) {
    if( !id.isEmpty() ) {
      //Enter component name
      mObject->forEach( dctPartImp, [this,id] (SdObject *obj) -> bool {
        SdPtr<SdGraphPartImp> imp(obj);
        if( imp && imp->ident() == id ) {
          imp->select( &mFragment );
          return false;
          }
        return true;
        });
      }
    }

  if( mFragment.count() ) {
    //Component found. Save component state
    //Save state of selected objects
    saveStateOfSelectedObjects( QObject::tr("Moving components") );
    //Fix its start position
    SdPtr<SdGraphPartImp> imp(mFragment.first());
    if( imp ) {
      SdPoint p = mPrevMove;
      mFirst = mPrevMove = imp->getOrigin();
      setDirtyCashe();
      setStep( psmMove );
      movePoint( p );
      }
    }
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
          //Save state of selected objects
          saveStateOfSelectedObjects( QObject::tr("Moving components") );
          }
        }
      else if( mBySheet ) {
        //Get sheet
        SdPtr<SdPItemSheet> sheet( mObject->getProject()->itemByName( dctSheet, mCurrentSheet) );
        if( sheet.isValid() )
          SdPulsar::sdPulsar->emitBrowseSheetPart( sheet.ptr(), mObject );
        }
      break;
    case psmMove :
      movePoint( p );
      setStep( psmNoSelect );
      unselect();
      checkPoint( p );
      break;
    }
  dirtyRatNet();
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
    QTransform map( p.transformMoveFrom(mPrevMove) );
    mFragment.forEach( dctAll, [map] (SdObject *obj) ->bool {
      SdGraph *graph = dynamic_cast<SdGraph*>(obj);
      if( graph != nullptr )
        graph->transform( map, SdPvAngle{} );
      return true;
      });
    mPrevMove = p;
    update();
    }
  else {
    mPrevMove = p;
    if( getStep() == psmNoSelect )
      checkPoint(p);
    }
  }





void SdModeCPartPlace::keyDown(int key, QChar ch)
  {
  switch( key ) {
    case Qt::Key_F2 :
      //Component rotation [Поворот]
      if( mFragment.count() && getStep() == psmMove ) {
        //Save state of selected objects
        //saveStateOfSelectedObjects( QObject::tr("Component rotation") );
        auto &angle = mLocalProp.mPartImpProp.get<&SdPropPartImp::mAngle>();
        if( angle.isSingle() )
          angle.reset( angle.value() + 90000 );
        else
          angle.reset( 0 );
        //mLocalProp.mPartImpProp.mAngle += 90000;
        setPropertiesToComponent();
        }
      break;
    case Qt::Key_F3 :
      //Flip component to other pcb side [Перенос на другую сторону платы]
      if( mFragment.count() && getStep() == psmMove ) {
        //Save state of selected objects
        //saveStateOfSelectedObjects( QObject::tr("Component flip") );
        auto &side = mLocalProp.mPartImpProp.get<&SdPropPartImp::mSide>();
        if( side.isSingle() )
          side.reset( side.value().isTop() ? stmBottom : stmTop );
        else
          side.reset( stmTop );
        //mLocalProp.mPartImpProp.mSide = mLocalProp.mPartImpProp.mSide == stmTop ? ;
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
        //saveStateOfSelectedObjects( QObject::tr("Group rotation") );
        //Perform rotation
        QTransform map( mPrevMove.transformRotation( SdPvAngle(da90) )  );
        mFragment.forEach( dctPartImp, [map] (SdObject *obj) -> bool {
          SdGraphPartImp *imp = dynamic_cast<SdGraphPartImp*>( obj );
          if( imp ) imp->transform( map, SdPvAngle(da90) );
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
    partSelect( QStringList(mSmartName) );
    return mFirst;
    }  
  return mPrevMove;
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
      SdPtr<SdGraphPartImp> imp(obj);
      if( imp.isValid()  && (imp->stratum().isIntersect( mSideMask )) )
        imp->selectByRect( r, &mFragment );
      return true;
      });
    mPrevMove = p;
    mFragment.setOrigin( p );
    propSetToBar();
    setStep( mFragment.count() ? psmMove : psmNoSelect );
    dirtyRatNet();
    setDirtyCashe();
    update();
    mFirst = p;
    //Save state of selected objects
    saveStateOfSelectedObjects( QObject::tr("Moving components") );
    }
  }




bool SdModeCPartPlace::getInfo(SdPoint p, QString &info)
  {
  mObject->forEach( dctPartImp, [p,&info] (SdObject *obj) -> bool {
    SdPtr<SdGraphPartImp> imp(obj);
    if( imp )
      return !imp->getInfo( p, info, true );
    return true;
    });
  return false;
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
      QString str = mBehindCursorPrt->ident();
      QString temp( QObject::tr("Left button: %1;   Middle button: %2") );
      return temp.arg(str).arg(mSmartName);
      }
    }
  if( mBySheet )
    return QObject::tr("Press left button to go to sheet to select component, middle button: ") + mSmartName;
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
  return MD_MOVE_PART;
  }





void SdModeCPartPlace::unselect()
  {
  nextComponent();

  //Remove components selection
  //Убрать компоненты из выделения
  if( mFragment.count() )
    mFragment.removeAll();
  }






//Set properties to component
void SdModeCPartPlace::setPropertiesToComponent()
  {
  //saveStateOfSelectedObjects( QObject::tr("Properties changed") );

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
    mUndo->begin( undoTitle, mObject, false );
    //Сохранить состояние объектов до изменения свойств
    mFragment.forEach( dctPartImp, [this] (SdObject *obj) -> bool {
      SdGraph *graph = dynamic_cast<SdGraph*>( obj );
      if( graph != nullptr )
        graph->saveState( mUndo );
      return true;
      });
    }
  }




//Perform placing
void SdModeCPartPlace::place()
  {

  }




//Check components behind point
void SdModeCPartPlace::checkPoint(SdPoint p)
  {
  mBehindCursorTable.clear();     //Очистить таблицу компонентов под курсором
  mObject->forEach( dctPartImp, [this,p] (SdObject *obj) -> bool {
    SdGraphPartImp *imp = dynamic_cast<SdGraphPartImp*>( obj );
    if( imp && imp->behindCursor(p) && (imp->stratum().isIntersect( mSideMask )) )
      mBehindCursorTable.append( imp );
    return true;
    });
  mBehindCursorIndex = 0;
  if( mBehindCursorTable.count() )
    mBehindCursorPrt = dynamic_cast<SdGraphPartImp*>(mBehindCursorTable[mBehindCursorIndex]);
  else
    mBehindCursorPrt = nullptr;
  SdPulsar::sdPulsar->emitSetStatusMessage( getStepHelp() );
  mEditor->viewport()->setCursor( loadCursor(getCursor()) );
  }




//Prepare next component to select
void SdModeCPartPlace::nextComponent()
  {
  //Select next smart component. As base we take last
  //Расчитать следующий smart - копонент, за основу берем последний
  SdPtr<SdGraphPartImp> last( mFragment.count() ? mFragment.first() : nullptr );
  if( last )
    mSmartName = last->ident();
  else return;
//    if( mSmartName.isEmpty() )
//    mSmartName = "R1";


  if( mSmartOrNextId ) {
    //Next smart in group
    }
  else {
    //Next by name [Следующий по номеру в группе]
    //Separate on name and index
    int i;
    for( i = 0; i < mSmartName.size() && !mSmartName.at(i).isDigit(); i++ );
    QString prefix = mSmartName.left(i);
    int index = mSmartName.mid(i).toInt();
    index++;
    mSmartName = prefix + QString::number(index);
    }
//  else {
//    //Следующий ближайший не размещенный
//    if( bigCompIndex >= 0 ) {
//      int sheetIndex = info.table[bigCompIndex].sheet;
//      int target = -1;
//      DPoint from(info.table[bigCompIndex].inShem);
//      int distance = illegalRatLen;
//      for( NCount i = 0; i < info.table.GetNumber(); ++i )
//        if( info.table[i].placed || info.table[i].sheet != sheetIndex ) continue;
//        else if( from.GetDistance( info.table[i].inShem ) < distance ) {
//          distance = from.GetDistance( info.table[target = i].inShem );
//          }
//      if( target >= 0 ) smartName = info.table[target].pos;
//      else {
//        DName Name( smartName );
//        CreateNextName( Name, Name ); //В случае отсутствия варианта - следующий по порядку
//        smartName = Name;
//        }
//      }
//    else {
//      DName Name( smartName );
//      CreateNextName( Name, Name ); //В случае отсутствия варианта - следующий по порядку
//      smartName = Name;
//      }
//    }
  }




SdPItemPlate *SdModeCPartPlace::plate()
  {
  return dynamic_cast<SdPItemPlate*>(mObject);
  }




void SdModeCPartPlace::dirtyRatNet()
  {
  plate()->setDirtyRatNet();

  //Accum current unselected nets
  mNetList.clear();
  //Accum points for nets
  mObject->forEach( dctAll, [this] (SdObject *obj) -> bool {
    SdGraphTraced *traced = dynamic_cast<SdGraphTraced*>(obj);
    if( traced && !traced->isSelected() )
      traced->accumNetSegments( &mNetList );
    return true;
    });

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
