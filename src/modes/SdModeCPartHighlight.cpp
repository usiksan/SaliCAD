#include "SdModeCPartHighlight.h"
#include "objects/SdGraphPartImp.h"
#include "objects/SdPulsar.h"

#include "windows/SdWEditorGraph.h"

SdModeCPartHighlight::SdModeCPartHighlight(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdModeCommon( editor, obj ),
  mBehindCursorPrt(nullptr)   //Компонент, подлежащий выделению при нажатии левой кнопки мыши
  {

  }




void SdModeCPartHighlight::drawStatic(SdContext *ctx)
  {
  //Draw all object
  if( mHighlightComponentBom.isEmpty() ) {
    //Draw objects as usualy
    mObject->forEach( dctAll, [ctx] (SdObject *obj) -> bool {
      SdPtr<SdGraph> graph( obj );
      if( graph.isValid() )
        graph->draw( ctx );
      return true;
      });
    }
  else {
    //Draw all objects as downlighted but highlighted components - as usualy
    mObject->forEach( dctAll, [this, ctx] (SdObject *obj) -> bool {
      SdPtr<SdGraph> graph( obj );
      if( graph.isValid() ) {
        SdPtr<SdGraphPartImp> partImp( obj );
        if( partImp.isValid() ) {
          //If part is match to highlighted component then draw it as usualy
          if( partImp->getBomItemLine() == mHighlightComponentBom )
            partImp->draw( ctx );
          else {
            //Not matched components draw some opacity
            ctx->setOpacity( 0.1 );
            partImp->draw( ctx );
            ctx->setOpacity( 1.0 );
            }
          }
        else graph->draw( ctx );
        }
      return true;
      });
    }
  }





void SdModeCPartHighlight::enterPoint(SdPoint)
  {
  if( mBehindCursorTable.count() ) {
    //There are components behind cursor [Под курсором есть компоненты]
    SdGraphPartImp *part = dynamic_cast<SdGraphPartImp*>(mBehindCursorTable[mBehindCursorIndex]);
    if( part != nullptr ) {
      mHighlightComponentBom = part->getBomItemLine();
      setDirtyCashe();
      update();
      }
    }
  }


void SdModeCPartHighlight::cancelPoint(SdPoint)
  {
  if( mHighlightComponentBom.isEmpty() )
    cancelMode();
  else mHighlightComponentBom.clear();
  setDirtyCashe();
  update();
  }



void SdModeCPartHighlight::movePoint(SdPoint p)
  {
  mBehindCursorTable.clear();     //Очистить таблицу компонентов под курсором
  mObject->forEach( dctPartImp, [this,p] (SdObject *obj) -> bool {
    SdGraphPartImp *imp = dynamic_cast<SdGraphPartImp*>( obj );
    if( imp && imp->behindCursor(p)  )
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




void SdModeCPartHighlight::keyDown(int key, QChar ch)
  {
  switch( key ) {
    case Qt::Key_F6 :
      //Roll components behind cursor [Перебор компонентов под курсором]
      if( mBehindCursorTable.count() > 1 ) {
        mBehindCursorIndex++;
        mBehindCursorIndex %= mBehindCursorTable.count();
        //Show component, which will select by left button
        //Отобразить компонент, который будет выделен при нажатии левой кнопки
        mBehindCursorPrt = dynamic_cast<SdGraphPartImp*>(mBehindCursorTable[mBehindCursorIndex]);
        SdPulsar::sdPulsar->emitSetStatusMessage( getStepHelp() );
        }
      break;
    }
  SdModeCommon::keyDown( key, ch );
  }




QString SdModeCPartHighlight::getStepHelp() const
  {
  if( mBehindCursorPrt == nullptr )
    return QObject::tr("Click on component to highlight");

  QString str = mBehindCursorPrt->ident();
  QString temp( QObject::tr("Left button: %1 (%2);   F6 to roll components behind cursor") );
  return temp.arg( str, mBehindCursorPrt->getBomItemLine() );
  }




QString SdModeCPartHighlight::getModeThema() const
  {
  return QStringLiteral( MODE_HELP "ModeCPartHighlight.htm" );
  }




QString SdModeCPartHighlight::getStepThema() const
  {
  return getModeThema();
  }




int SdModeCPartHighlight::getCursor() const
  {
  return mBehindCursorTable.count() ? CUR_TAKE : CUR_ARROW;
  }




int SdModeCPartHighlight::getIndex() const
  {
  return MD_PART_HIGHLIGHT;
  }


