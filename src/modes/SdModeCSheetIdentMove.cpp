/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for ident of component moving in the schematic sheet
*/
#include "SdModeCSheetIdentMove.h"
#include "objects/SdGraphSymImp.h"
#include "objects/SdEnvir.h"
#include "objects/SdPulsar.h"
#include "windows/SdPropBarTextual.h"
#include "windows/SdWCommand.h"
#include "windows/SdWEditorGraph.h"

SdModeCSheetIdentMove::SdModeCSheetIdentMove(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdModeCommon( editor, obj ),
  mBehindCursorIndex(0), //Индекс объекта среди объектов под курсором
  mImp(nullptr)
  {

  }


void SdModeCSheetIdentMove::drawStatic(SdContext *ctx)
  {
  //Draw all except selected implement
  mObject->forEach( dctAll, [this,ctx] (SdObject *obj) ->bool {
    SdPtr<SdGraph> graph(obj);
    if( graph.isValid() && graph.ptr() != mImp )
      graph->draw( ctx );
    return true;
    });
  }




void SdModeCSheetIdentMove::drawDynamic(SdContext *ctx)
  {
  if( getStep() && mImp ) {
    //Draw selected implement with entre color
    ctx->setOverColor( sdEnvir->getSysColor(scSelected) );
    mImp->draw( ctx );
    }
  }




int SdModeCSheetIdentMove::getPropBarId() const
  {
  return PB_TEXT;
  }




void SdModeCSheetIdentMove::propGetFromBar()
  {
  SdPropBarTextual *bar = dynamic_cast<SdPropBarTextual*>( SdWCommand::getModeBar(PB_TEXT) );
  Q_ASSERT( bar != nullptr );
  bar->getPropText( &mPropText );
  mPropSmart = mPropText;
  update();
  }




void SdModeCSheetIdentMove::propSetToBar()
  {
  SdPropBarTextual *bar = dynamic_cast<SdPropBarTextual*>( SdWCommand::getModeBar(PB_TEXT) );
  Q_ASSERT( bar != nullptr );
  bar->setPropText( &mPropText, mEditor->getPPM() );
  }





void SdModeCSheetIdentMove::enterPoint(SdPoint point)
  {
  if( getStep() ) {
    //Fix ident position
    if( mImp ) {
      //At first restore saved
      setProp( mPropSaved, mPositionSaved, nullptr );
      //At second set new values
      mUndo->begin( QObject::tr("Move ident or value"), mObject, false );
      setProp( mPropText, mPosition, mUndo );
      mImp = nullptr;
      setDirty();
      }
    setStep( sSelectComp );
    movePoint(point);
    setDirtyCashe();
    update();
    }
  else {
    if( mBehindCursorTable.count() ) {
      //There are components behind cursor [Под курсором есть компоненты]
      SdPtr<SdGraph> sym(mBehindCursorTable[mBehindCursorIndex]);
      if( sym.isValid() ) {
        //Retrive symbol impelement
        mImp = sym.ptr();
        //Store properties
        getProp();
        mPropText = mPropSaved;
        mPosition = mPositionSaved;
        mPrev = point;
        propSetToBar();
        setStep( sPlaceIdent );
        setDirtyCashe();
        update();
        }
      }
    }
  }




void SdModeCSheetIdentMove::cancelPoint(SdPoint point)
  {
  if( getStep() ) {
    //Fix ident position
    if( mImp )
      //restore saved
      setProp( mPropSaved, mPositionSaved, nullptr );
    mImp = nullptr;
    setStep(sSelectComp);
    movePoint(point);
    setDirtyCashe();
    update();
    }
  else cancelMode();
  }




void SdModeCSheetIdentMove::movePoint(SdPoint point)
  {
  if( getStep() ) {
    if( mImp ) {
      //Calculate new ident position
      mPosition = mImpMatrix.map( point );
      //Update position in imp
      setProp( mPropText, mPosition, nullptr );
      update();
      }
    }
  else {
    mBehindCursorTable.clear();     //Очистить таблицу компонентов под курсором
    mObject->forEach( getMask(), [this,point] (SdObject *obj) -> bool {
      SdPtr<SdGraph> imp( obj );
      if( imp.isValid() && imp->behindCursor(point) )
        mBehindCursorTable.append( imp.ptr() );
      return true;
      });
    mBehindCursorIndex = 0;
    SdPulsar::sdPulsar->setStatusMessage( getStepHelp() );
    }
  }




SdPoint SdModeCSheetIdentMove::enterPrev()
  {
  if( getStep() ) {
    mPropText = mPropSmart;
    propSetToBar();
    setProp( mPropText, mPosition, nullptr );
    update();
    }
  return mPrev;
  }




bool SdModeCSheetIdentMove::getInfo(SdPoint p, QString &info)
  {
  mObject->forEach( getMask(), [p,&info] (SdObject *obj) -> bool {
    SdPtr<SdGraph> imp(obj);
    if( imp )
      return !imp->getInfo( p, info, true );
    return true;
    });
  return false;
  }




QString SdModeCSheetIdentMove::getStepHelp() const
  {
  if( getStep() )
    return QObject::tr("Enter new ident or value position.");
  else {
    if( mBehindCursorTable.count() ) {
      SdPtr<SdGraphSymImp> imp(mBehindCursorTable.at(mBehindCursorIndex));
      if( imp.isValid() )
        return QObject::tr("Left button: %1; F6 - switch to next component behind cursor").arg(imp->ident());
      }
    }
  return QObject::tr("Select component to move ident or value");
  }



QString SdModeCSheetIdentMove::getModeThema() const
  {
  return QStringLiteral( MODE_HELP "ModeCSheetIdentMove.htm" );
  }




QString SdModeCSheetIdentMove::getStepThema() const
  {
  if( getStep() )
    return QStringLiteral( MODE_HELP "ModeCSheetIdentMove.htm#placeIdent" );
  return QStringLiteral( MODE_HELP "ModeCSheetIdentMove.htm#selectComp" );
  }




int SdModeCSheetIdentMove::getCursor() const
  {
  return CUR_IDENT;
  }




int SdModeCSheetIdentMove::getIndex() const
  {
  return MD_SHEET_IDENT;
  }






void SdModeCSheetIdentMove::keyDown(int key, QChar ch)
  {
  switch( key ) {
    case Qt::Key_F2 :
      //Ident rotation [Поворот]
      if( getStep() ) {
        mPropText.mDir += 90000;
        propSetToBar();
        //Update position in imp
        setProp( mPropText, mPosition, nullptr );
        update();
        }
      break;
    case Qt::Key_F6 :
      //Roll components behind cursor [Перебор компонентов под курсором]
      if( getStep() == sSelectComp && mBehindCursorTable.count() > 1 ) {
        mBehindCursorIndex++;
        mBehindCursorIndex %= mBehindCursorTable.count();
        //Show component, which will select by left button
        //Отобразить компонент, который будет выделен при нажатии левой кнопки
        SdPulsar::sdPulsar->setStatusMessage( getStepHelp() );
        }
      break;
    }
  SdModeCommon::keyDown( key, ch );
  }




void SdModeCSheetIdentMove::setProp(const SdPropText &prp, SdPoint pos, SdUndo *undo)
  {
  SdPtr<SdGraphSymImp> imp(mImp);
  if( imp.isValid() )
    imp->identSet( prp, pos, undo );
  }




void SdModeCSheetIdentMove::getProp()
  {
  SdPtr<SdGraphSymImp> imp(mImp);
  if( imp.isValid() ) {
    //Get implement matrix
    mImpMatrix     = imp->matrix().inverted();
    //Get properties
    mPropSaved     = imp->identProp();
    mPositionSaved = imp->identPosition();
    }
  }


