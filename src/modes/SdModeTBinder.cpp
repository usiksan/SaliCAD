#include "SdModeTBinder.h"
#include "objects/SdEnvir.h"
#include "objects/SdSnapInfo.h"
#include "windows/SdDOffset.h"
#include "windows/SdWEditorGraph.h"


//! \brief SdModeTBinder Constructor for temporary binder mode
//! \param editor        Pointer to the graphical editor
//! \param obj           Pointer to the project item being edited
SdModeTBinder::SdModeTBinder(SdWEditorGraph *editor, SdProjectItem *obj)
  : SdModeTemp( editor, obj )
  {

  }




//! \brief drawDynamic Draw dynamic (interactive) part of the scene during editing
//! \param ctx         Drawing context for rendering
void SdModeTBinder::drawDynamic(SdContext *ctx)
  {
  if( mSmartType )
    ctx->smartPoint( mSmartPoint, mSmartType );
  }



//! \brief enterPoint Process point input (left mouse button click)
//! \param p          Input point coordinates
void SdModeTBinder::enterPoint(SdPoint p)
  {
  //We fixed smart point as base point and query offset
  SdDOffset offset( QString{}, mEditor );
  if( offset.exec() ) {
    SdPoint of;
    of.setX( SdUtil::phys2log( offset.offset().x(), getPPM() ) );
    of.setY( SdUtil::phys2log( offset.offset().y(), getPPM() ) );
    mSmartPoint += of;
    mEditor->modePopWithEnter( mSmartPoint );
    }
  else cancelPoint(p);
  }



void SdModeTBinder::cancelPoint(SdPoint)
  {
  mEditor->modeCancel();
  }



void SdModeTBinder::movePoint(SdPoint p)
  {
  //Вычислить предполагаемую точку вывода
  SdSnapInfo snap;
  snap.mSour     = p;
  snap.mSnapMask = snapAll;
//  snap.mExclude  = mFirst;
  snap.scan( mObject );
  mSmartType  = snap.mDestMask;
  mSmartPoint = snap.mDest;

  update();
  }




void SdModeTBinder::keyDown(int key, QChar ch)
  {
  Q_UNUSED(key)
  if( !ch.isNull() ) {
    SdDOffset offset( QString(ch), mEditor );
    if( offset.exec() ) {
      SdPoint of;
      of.setX( SdUtil::phys2log( offset.offset().x(), getPPM() ) );
      of.setY( SdUtil::phys2log( offset.offset().y(), getPPM() ) );
      of += mEditor->prevEnterPosition();
      mEditor->modePopWithEnter( of );
      }
    else mEditor->modeCancel();
    }
  }



//! \brief getStepHelp Get brief help text for current editing step
//! \return            Help text string
QString SdModeTBinder::getStepHelp() const
  {
  return QObject::tr("Enter binding point or enter offset from previous point");
  }




//! \brief getModeThema Get help page address for the mode
//! \return             Help page URL or identifier
QString SdModeTBinder::getModeThema() const
  {
  return QStringLiteral( MODE_HELP "ModeTBinder.htm" );
  }




//! \brief getStepThema Get help page address and link for current editing step
//! \return             Help page URL or identifier for current step
QString SdModeTBinder::getStepThema() const
  {
  return QStringLiteral( MODE_HELP "ModeTBinder.htm" );
  }




//! \brief getCursor Get cursor number/type for this mode
//! \return          Cursor identifier
int SdModeTBinder::getCursor() const
  {
  return CUR_ARROW;
  }




//! \brief getIndex Get mode index in the mode list
//! \return         Mode index number
int SdModeTBinder::getIndex() const
  {
  return MD_BINDER;
  }
