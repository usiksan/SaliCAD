/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical object with script. Script used for in schematic calculations.
  It represents math script language like python or subset of C

  Example of script:
  #This is remark. All symbols after # sing - are ignored
  R1 = ref( "Om", "" )
  R2 = ref( "Om", "" )
  Rcom = ref( "Om", "" )
  Rcom = R1 + R2

  This script calculate common resistance of continuosly connected resistors.
  ref function used to declare visibility and editing possibility of variable.
  In schematic such script will be displayed as:

  R1 = 10kOm
  R2 = 10kOm
  Rcom = 20kOm

  Values 10kOm are editable, i.e. they can be edited by text mode. When they changed
  script automatically recalculated and display correct result.
*/
#include "SdGraphScript.h"
#include "SdContext.h"
#include "SdSelector.h"
#include "SdPItemSheet.h"
#include "script/SdScriptParserCalculator.h"

#include <QDebug>

SdGraphScript::SdGraphScript() :
  mNeedLink(false)
  {

  }



SdGraphScript::SdGraphScript(const QString &script, SdPoint org, const SdPropText &prp) :
  SdGraphParam(),
  mScript(script),
  mOrigin(org),
  mProp(prp),
  mNeedLink(false)
  {
  ajustProp();
  parse();
  }



void SdGraphScript::scriptSet(const QString &scr, SdUndo *undo)
  {
  if( undo != nullptr )
    undo->string2( &mScript, nullptr );

  mScript = scr;
  parse();
  }



void SdGraphScript::parse()
  {
  SdScriptParserCalculator parser( &mRefMap, nullptr );
  mProgramm = parser.parse( mScript );
  mRefMap.parseEnd();
  mError = parser.error();
  }




void SdGraphScript::calculate()
  {
  if( mNeedLink ) {
    mRefMap.updateLinks( mOrigin, getParent() );
    mNeedLink = false;
    }
  if( mProgramm ) {
    mRefMap.clearCalculated();
    mProgramm->execute();
    }
  }

int SdGraphScript::tryLink(SdPoint p, SdRect &overRect) const
  {
  return mRefMap.tryLink( p, overRect );
  }



void SdGraphScript::link(int index, SdContainer *owner, SdGraphParam *ref, const QString paramName)
  {
  mRefMap.link( index, owner, ref, paramName );
  calculate();
  }



QString SdGraphScript::getType() const
  {
  return QStringLiteral(SD_TYPE_SCRIPT);
  }




SdClass SdGraphScript::getClass() const
  {
  return dctGraphScript;
  }






//!
//! \brief cloneFrom Overrided function. We copy object from source
//! \param src       Source of object from which copy must be made
//! \param copyMap   Structure for mapping copying substitutes
//! \param next      Make simple or next copy. Next copy available not for all objects.
//!                  For example: pin name A23 with next copy return A24
//!
void SdGraphScript::cloneFrom(const SdObject *src, SdCopyMap &copyMap, bool next)
  {
  SdGraphParam::cloneFrom( src, copyMap, next );
  SdPtrConst<SdGraphScript> ps(src);
  Q_ASSERT_X( ps.isValid(), "SdGraphScript::cloneForm", "Cloned not script" );
  mScript = ps->mScript;
  mOrigin = ps->mOrigin;
  mProp   = ps->mProp;
  parse();
  //Prepare offsets
  mRefMap.cloneLinks( mOrigin, ps->mRefMap );
  mNeedLink = true;
  //qDebug() << "cloned";
  }






void SdGraphScript::writeObject(QJsonObject &obj) const
  {
  SdGraphParam::writeObject( obj );
  SdJsonWriter js( obj );
  js.jsonValue<SdPropText>( "prop", mProp );
  js.jsonString( "script", mScript );
  js.jsonObject<SdGraphScriptRefMap>( "ref", mRefMap );
  js.jsonPoint( "origin", mOrigin );
  js.jsonObject<SdRect>( "over", mOverRect );
  }




void SdGraphScript::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdGraph::readObject( map, obj );
  mProp.read( QStringLiteral("prop"), obj );
  SdJsonReader js( obj, map );
  js.jsonValue<SdPropText>( "prop", mProp );
  js.jsonString( "script", mScript );
  js.jsonObject<SdGraphScriptRefMap>( "ref", mRefMap );
  js.jsonPoint( "origin", mOrigin );
  js.jsonObject<SdRect>( "over", mOverRect );
  parse();
  }



void SdGraphScript::saveState(SdUndo *undo)
  {
  if( undo )
    undo->propTextAndText( &mProp, &mOrigin, &mOverRect, &mScript );
  }




void SdGraphScript::move(SdPoint offset)
  {
  mOrigin.move( offset );
  mOverRect.move( offset );
  }




void SdGraphScript::setProp(SdPropSelected &prop)
  {
  mProp = prop.mTextProp;
  ajustProp();
  }



void SdGraphScript::getProp(SdPropSelected &prop)
  {
  prop.mTextProp.append( mProp );
  prop.mFilledPropMask |= spsTextProp;
  }




void SdGraphScript::setText(int index, QString sour, SdPropText &prop, QWidget *parent)
  {
  Q_UNUSED(parent)
  mProp = prop;
  ajustProp();
  mRefMap.setText( index - 1, sour );
  calculate();
  }



void SdGraphScript::selectByPoint(const SdPoint p, SdSelector *selector)
  {
  if( mProp.mLayer.isEdited() ) {
    if( !getSelector() && mOverRect.isPointInside(p) ) {
      select( selector );
      }
    }
  }





void SdGraphScript::selectByRect(const SdRect &r, SdSelector *selector)
  {
  if( mProp.mLayer.isEdited() ) {
    if( !getSelector() && r.isAccross( mOverRect ) ) {
      select( selector );
      }
    }
  }



void SdGraphScript::select(SdSelector *selector)
  {
  SdGraphParam::select( selector );
  if( !mNeedLink )
    mRefMap.fillLinks( mOrigin );
  }






void SdGraphScript::setLayerUsage()
  {
  mProp.mLayer.setLayerUsage();
  }



bool SdGraphScript::isVisible()
  {
  return mProp.mLayer.isVisible();
  }



SdRect SdGraphScript::getOverRect() const
  {
  return mOverRect;
  }



void SdGraphScript::draw(SdContext *dc)
  {
  if( mError.isEmpty() ) {
    if( getSelector() == nullptr )
      calculate();
    mRefMap.drawExcept( mOrigin, mProp, dc, -1 );
    mOverRect = mRefMap.getOverRect();
    if( getSelector() != nullptr ) {
      //Set property
      SdPropLine prop;
      prop.mType = dltDashed;
      prop.mLayer = mProp.mLayer;
      dc->setProp( prop );
      //Draw all links
      mRefMap.drawLinks( mOrigin, dc );
      }
    }
  else
    dc->text( mOrigin, mOverRect, QStringLiteral("Error: ") + mError, mProp );
  }



void SdGraphScript::drawExceptText(SdContext *dc, int textId)
  {
  mRefMap.drawExcept( mOrigin, mProp, dc, textId - 1 );
  }



int SdGraphScript::behindCursor(SdPoint p)
  {
  if( mProp.mLayer.isEdited() ) {
    if( mOverRect.isPointInside(p) )
      return getSelector() ? ELEM_SEL : ELEM_UNSEL;
    }
  return 0;
  }



int SdGraphScript::behindText(SdPoint p, SdPoint &org, QString &dest, SdPropText &prop)
  {
  if( mProp.mLayer.isEdited() && mOverRect.isPointInside(p) ) {
    prop = mProp;
    return mRefMap.behindText( p, org, dest ) + 1;
    }
  return 0;
  }




void SdGraphScript::ajustProp()
  {
  mProp.mDir    = da0;
  mProp.mMirror = 0;
  mProp.mHorz   = dhjLeft;
  }

