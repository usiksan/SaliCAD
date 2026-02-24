/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for enter bus wires
*/
#include "SdModeCBus.h"
#include "objects/SdEnvir.h"
#include "objects/SdGraphNetWire.h"
#include "objects/SdGraphNetName.h"
#include "objects/SdSnapInfo.h"
#include "windows/SdPropBarTextual.h"
#include "windows/SdPropBarWire.h"
#include "windows/SdWCommand.h"
#include "windows/SdWEditorGraph.h"
#include "windows/SdDGetBus.h"

#include <QMessageBox>

SdPointList SdModeCBus::mPattern; //Bus wire pattern
SdPoint     SdModeCBus::mNamePos;

SdModeCBus::SdModeCBus(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdModeCommon( editor, obj ),
  mPrevDirection(0),
  mIndex(0)
  {

  }


void SdModeCBus::drawDynamic(SdContext *ctx)
  {
  ctx->setOverColor( SdEnvir::instance()->getSysColor(scEnter) );
  switch( getStep() ) {
    case sNextNet :
      if( SdEnvir::instance()->mIsSmart ) smartDraw( ctx );
      //break absent specialy
      [[fallthrough]];
    case sFirstPoint :
      //Using previous pattern
      //Использование предыдущего шаблона
      if( mPattern.count() == 0 || mNetList.count() == 0 ) break;
      //break absent specialy
      [[fallthrough]];
    case sNamePlace : {
      //Enter wire name placement
      //Поиск расположения имени цепи
      drawPattern( ctx );
      SdRect r;
      ctx->text( mNamePos, r, mNetList.at(mIndex), sdGlobalProp->mWireNameProp );
      }
      break;
    case sNextPoint :
    case sSecondPoint : {
      //Поиск очередного сегмента цепи
      SdPoint middle,first;
      //Чертить шаблон цепи
      first = drawPattern( ctx );
      //Чертить последний излом
      middle = calcMiddlePoint( first, mPrevMove, sdGlobalProp->mWireEnterType );
      ctx->line( first, middle, sdGlobalProp->mWireProp );
      if( middle != mPrevMove )
        ctx->line( middle, mPrevMove, sdGlobalProp->mWireProp );
      }
      break;
    }
  ctx->resetOverColor();
  }




int SdModeCBus::getPropBarId() const
  {
  if( getStep() == sNamePlace ) return PB_TEXT;
  return PB_WIRE;
  }




void SdModeCBus::propGetFromBar()
  {
  if( getStep() == sNamePlace ) {
    auto tbar = SdWCommand::getModeToolBar<SdPropBarTextual>(PB_TEXT);
    if( tbar ) {
      tbar->getPropText( &(sdGlobalProp->mWireNameProp) );
      }
    }
  else {
    auto bar = SdWCommand::getModeToolBar<SdPropBarWire>( PB_WIRE );
    if( bar ) {
      SdPvMulty<SdPvString> wireName;
      bar->getPropWire( sdGlobalProp->mWireProp, &(sdGlobalProp->mWireEnterType), wireName );
      }
    }
  update();
  }



void SdModeCBus::propSetToBar()
  {
  if( getStep() == sNamePlace ) {
    auto tbar = SdWCommand::getModeToolBar<SdPropBarTextual>(PB_TEXT);
    if( tbar ) {
      tbar->setPropText( mObject, &(sdGlobalProp->mWireNameProp), mEditor->getPPM() );
      }
    }
  else if( mNetList.count() && mIndex < mNetList.count() ) {
    auto bar = SdWCommand::getModeToolBar<SdPropBarWire>( PB_WIRE );
    if( bar )
      bar->setPropWire( sdGlobalProp->mWireProp, mEditor->getPPM(), sdGlobalProp->mWireEnterType, SdPvMulty<SdPvString>( SdPvString(mNetList.at(mIndex)) ) );
    }
  }




void SdModeCBus::enterPoint(SdPoint enter)
  {
  switch( getStep() ) {
    case sFirstPoint :
      //Очистить шаблон
      mPattern.clear();
      //Поиск первой точки цепи
      if( testNextPoint( enter ) ) {
        //Переходим к следующему сегменту
        setStep( sSecondPoint );
        mPrevMove = enter;
        mSmartOrigin = mPrevMove;
        //Точка поставлена успешно
        }
      break;
    case sNextPoint :
      //Поиск очередной точки цепи
    case sSecondPoint :
      //Поиск второй точки цепи
      if( testNextPoint( enter ) ) {
        //Переходим к следующему сегменту
        setStep( sNextPoint );
        mPrevMove = enter;
        //Точка поставлена успешно
        }
      break;
    case sNamePlace :
      //Поиск места расположения имени
      mNamePos = enter;
      //Установить панель соответствующую имени цепи
      //SetPropBar( dpWireName, 0 );

      [[fallthrough]];
    case sNextNet :
      //Вводим цепь в базу данных
      enterNet();
      mSmartOrigin = mPattern[0];
      //Образуем новую цепь
      if( getNextNet() ) {
        //Смещаем весь шаблон
        moveAll( enter );
        setStep( sNextNet );
        }
      else {
        setStep( sLast );
        update();
        getNetList();
        }
      break;
    }
  update();
  }




void SdModeCBus::cancelPoint(SdPoint)
  {
  switch( getStep() ) {
    case sNextNet :
    case sNamePlace :
    case sFirstPoint :
      getNetList();
      break;
    case sSecondPoint :
      mPattern.clear();
      setStep( sFirstPoint );
      break;
    case sNextPoint : {
      //Setup net name properties on base pattern orientation
      SdSegment lastSegment( mPattern.last(), mPattern.at( mPattern.count() - 2 ) );
      if( lastSegment.isSectionX() ) {
        //Last segment is horizontal
        sdGlobalProp->mWireNameProp.mDir = da0;
        sdGlobalProp->mWireNameProp.mVert = dvjMiddle;
        sdGlobalProp->mWireNameProp.mHorz = lastSegment.getP1().x() < lastSegment.getP2().x() ? dhjLeft : dhjRight;
        }
      else if( lastSegment.isSectionY() ) {
        //Last segment is vertical
        sdGlobalProp->mWireNameProp.mDir = da90;
        sdGlobalProp->mWireNameProp.mVert = dvjMiddle;
        sdGlobalProp->mWireNameProp.mHorz = lastSegment.getP1().y() > lastSegment.getP2().y() ? dhjLeft : dhjRight;
        }
      setStep( sNamePlace );
      }
      break;
    }
  update();
  }



void SdModeCBus::movePoint( SdPoint p )
  {
  mPrevMove = p;
  switch( getStep() ) {
    case sNextNet :
      moveAll( mPrevMove );
      if( SdEnvir::instance()->mIsSmart && mPrevDirection != calcDirection90( mSmartOrigin, mPrevMove ) ) {
        mPrevDirection = calcDirection90( mSmartOrigin, mPrevMove );
        mSmartTable.clear();
        SdSnapInfo info;
        info.mSour     = mSmartOrigin;
        info.mExclude  = mSmartOrigin;
        info.mDir      = mPrevDirection;
        info.mSnapMask = snapNextPin | snapExcludeSour | snapExcludeExcl;
        do {
          info.scan( mObject );
          if( info.mDest != info.mSour ) mSmartTable.append( info.mDest );
          else break;
          info.mExclude = info.mSour;
          info.mSour    = info.mDest;
          info.reset(false);
          }
        while( mSmartTable.count() < mNetList.count() - mIndex );
        }
      break;
    case sNamePlace :
      mNamePos = mPrevMove;
      break;
    case sFirstPoint : {
      //Поиск начальной точки для шаблона
      SdSnapInfo info;
      info.mSour     = mPrevMove;
      info.mExclude  = mPrevMove;
      info.mSnapMask = snapNearestPin;
      info.scan( mObject );
      if( info.mDest != info.mSour ) moveAll( info.mDest );
      }
    }
  update();
  }




SdPoint SdModeCBus::enterPrev()
  {
  switch( getStep() ) {
    case sNextNet :
      if( mSmartTable.count() ) {
        //Вводим цепи циклически
        for( SdPoint t : std::as_const( mSmartTable )  ) {
          moveAll( t );
          enterNet();
          getNextNet();
          if( mIndex >= mNetList.count() ) break;
          }
        setStep( sLast );
        update();
        getNetList();
        }
      break;
    case sFirstPoint :
      enterNet();
      //Образуем новую цепь
      if( getNextNet() ) {
        mSmartOrigin = mPattern[0];
        setStep( sNextNet );
        update();
        }
      else {
        setStep( sLast );
        update();
        getNetList();
        }
      break;
    }
  return mSmartOrigin;
//  update();
  }




QString SdModeCBus::getStepHelp() const
  {
  switch ( getStep() ) {
    default:
    case sFirstPoint : return QObject::tr("Enter first point of path");
    case sSecondPoint : return QObject::tr("Enter second point of path");
    case sNextPoint :  return QObject::tr("Enter next point of path");
    case sNamePlace :  return QObject::tr("Enter wire name placement");
    case sNextNet : return QObject::tr("Enter connection point of next wire path");
    }
  }




QString SdModeCBus::getModeThema() const
  {
  return QStringLiteral( MODE_HELP "ModeCBus.htm" );
  }




QString SdModeCBus::getStepThema() const
  {
  switch ( getStep() ) {
    default:
    case sFirstPoint : return QStringLiteral( MODE_HELP "ModeCBus.htm#firstPoint" );
    case sSecondPoint : return QStringLiteral( MODE_HELP "ModeCBus.htm#secondPoint" );
    case sNextPoint :  return QStringLiteral( MODE_HELP "ModeCBus.htm#nextPoint" );
    case sNamePlace :  return QStringLiteral( MODE_HELP "ModeCBus.htm#namePlace" );
    case sNextNet : return QStringLiteral( MODE_HELP "ModeCBus.htm#nextNet" );
    }
  }




int SdModeCBus::getCursor() const
  {
  return CUR_BUS;
  }




int SdModeCBus::getIndex() const
  {
  return MD_BUS;
  }




void SdModeCBus::activate()
  {
  SdModeCommon::activate();
  if( getNetList() )
    update();
  }




bool SdModeCBus::getNextNet()
  {
  mIndex++;
  if( mIndex < mNetList.count() ) {
    //setup mode step properties bar
    SdWCommand::activateModeBar( getPropBarId(), mObject );
    //setup properties in bar
    propSetToBar();
    return true;
    }
  return false;
  }




void SdModeCBus::smartDraw(SdContext *ctx)
  {
  for( SdPoint p : std::as_const( mSmartTable )  )
    ctx->smartPoint( p, snapNextPin );
  }




bool SdModeCBus::testNextPoint(SdPoint p)
  {
  QString tmpName;
  SdPoint tp;
  if( mPattern.count() ) tp = calcMiddlePoint( mPattern.last(), p, sdGlobalProp->mWireEnterType );
  else tp = p;
  if( getSheet()->getNetFromPoint( tp, tmpName ) && tmpName != mNetList.at(mIndex) ) {
    QMessageBox::warning( mEditor, QObject::tr("Error"), QObject::tr("Bus wire vertex is on other wire. Select other point.") );
    return false;
    }
  //Ввести точку в массив точек
  mPattern.append( tp );
  return true;
  }





void SdModeCBus::enterNet()
  {
  mUndo->begin( QObject::tr("Insert bus wire"), mObject, false );
  //Попробуем найти цепь в схеме
  QString netName = mNetList.at(mIndex);

  //Ввод сегментов цепи
  for( int i = 1; i < mPattern.count(); ++i )
    if( mPattern[i-1] != mPattern[i] )
      getSheet()->insertChild( new SdGraphNetWire( mPattern[i-1], mPattern[i], netName, sdGlobalProp->mWireProp ), mUndo );
  //Ввод имени цепи
  getSheet()->insertChild( new SdGraphNetName( mNamePos, netName, sdGlobalProp->mWireNameProp ), mUndo );
  //Объявить проект как измененный
  setDirty();
  setDirtyCashe();
  }



void SdModeCBus::moveAll(SdPoint p)
  {
  if( !mPattern.count() ) return;
  //Вычислить смещение
  QTransform map( SdPoint::transformFromOffset( p - mPattern.get(0) ) );
  //Сместить шаблон
  for( int i = 0; i < mPattern.count(); ++i ) {
    mPattern[i] = map.map( mPattern.at(i) );
    }
  //Сместить имя
  mNamePos = map.map( mNamePos );
  }



SdPoint SdModeCBus::drawPattern(SdContext *ctx)
  {
  if( mPattern.count() < 1 ) return mPrevMove;
  int i = 1;
  for( ; i < mPattern.count(); ++i )
    ctx->line( mPattern[i-1], mPattern[i], sdGlobalProp->mWireProp );
  return mPattern[i-1];
  }




bool SdModeCBus::getNetList()
  {
  mSmartTable.clear();
  mPrevDirection = -1;
  SdDGetBus dbus( getSheet()->getProject(), mEditor);
  if( dbus.exec() ) {
    mIndex = -1;
    mNetList = dbus.busList();
    getNextNet();
    setStep( sFirstPoint );
    //Установить панель соответствующую цепи
    //SetPropBar( dpWire, 0 );
    update();
    return true;
    }
  update();
  cancelMode();
  return false;
  }
