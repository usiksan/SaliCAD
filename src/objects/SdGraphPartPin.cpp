/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical object Part pin.
*/
#include "SdGraphPartPin.h"
#include "SdEnvir.h"
#include "SdSelector.h"
#include "SdContext.h"
#include "SdContainer.h"
#include "SdPItemPlate.h"
#include "SdJsonIO.h"

#include <QMessageBox>

SdGraphPartPin::SdGraphPartPin() :
  SdGraph(),
  mPinSelect(false),  //Pin selected
  mNumSelect(false),  //Pin number selected
  mNamSelect(false)   //Pin name selected
  {

  }




SdGraphPartPin::SdGraphPartPin(SdPoint org, const SdPropPartPin &pinProp, SdPoint numberPos, const SdPropText &numberProp, SdPoint namePos, const SdPropText &nameProp, const QString number) :
  SdGraph(),
  mPinSelect(false),  //Pin selected
  mNumSelect(false),  //Pin number selected
  mNamSelect(false)   //Pin name selected
  {
  mOrigin     = org;        //Pin origin
  mPinProp    = pinProp;    //Pin properties
  mNamePos    = namePos;    //Pin name position
  mNameProp   = nameProp;   //Pin name properties
  mNumberPos  = numberPos;  //Pin number position
  mNumberProp = numberProp; //Pin number properties
  mNumber     = number;     //Pin number
  }




int SdGraphPartPin::getPinStratum(bool otherSide) const
  {
  return mPinProp.mSide.stratum( !otherSide );
  }




void SdGraphPartPin::drawImp(SdContext *dc, const QString pinName, bool com)
  {
  //Pin it self
  if( !com )
    dc->partPin( mOrigin, mPinProp.mLayer.layer() );

  //Pin name
  dc->text( mNamePos, mNameRect, pinName, mNameProp );

  //Pin number as over rectangle.
  dc->text( mNumberPos, mNumberRect, mNumber, mNumberProp );
  }




//!
//! \brief cloneFrom Overrided function. We copy object from source
//! \param src       Source of object from which copy must be made
//! \param copyMap   Structure for mapping copying substitutes
//! \param next      Make simple or next copy. Next copy available not for all objects.
//!                  For example: pin name A23 with next copy return A24
//!
void SdGraphPartPin::cloneFrom(const SdObject *src, SdCopyMap &copyMap, bool next)
  {
  SdGraph::cloneFrom( src, copyMap, next );
  SdPtrConst<SdGraphPartPin> pin(src);
  if( pin.isValid() ) {
    mOrigin     = pin->mOrigin;     //Pin origin
    mPinProp    = pin->mPinProp;    //Pin properties
    mNumberPos  = pin->mNumberPos;  //Pin number position
    mNumberRect = pin->mNumberRect; //Pin number over rect
    mNumberProp = pin->mNumberProp; //Pin number properties
    mNamePos    = pin->mNamePos;    //Pin name position
    mNameRect   = pin->mNameRect;   //Pin name over rect
    mNameProp   = pin->mNameProp;   //Pin name properties
    mNumber     = next ? nextText( pin->mNumber ) : pin->mNumber;     //Pin number

    //Different pin part selection
    mPinSelect = false;  //Pin selected
    mNumSelect = false;  //Pin number selected
    mNamSelect = false;  //Pin name selected
    }
  }




//!
//! \brief json Overloaded function to write object content into json writer
//!             Overrided function
//! \param js   Json writer
//!
void SdGraphPartPin::json(SdJsonWriter &js) const
  {
  js.jsonPoint( QStringLiteral("Origin"), mOrigin );
  mPinProp.json( js );
  js.jsonPoint( QStringLiteral("NumberPos"), mNumberPos );
  mNumberProp.json( QStringLiteral("Number"), js );
  js.jsonPoint( QStringLiteral("NamePos"), mNamePos );
  mNameProp.json( QStringLiteral("Name"), js );
  js.jsonString( QStringLiteral("Number"), mNumber );
  SdGraph::json( js );
  }





//!
//! \brief json Overloaded function to read object content from json reader
//!             Overrided function
//! \param js   Json reader
//!
void SdGraphPartPin::json(const SdJsonReader &js)
  {
  js.jsonPoint( QStringLiteral("Origin"), mOrigin );
  mPinProp.json( js );
  js.jsonPoint( QStringLiteral("NumberPos"), mNumberPos );
  mNumberProp.json( QStringLiteral("Number"), js );
  js.jsonPoint( QStringLiteral("NamePos"), mNamePos );
  mNameProp.json( QStringLiteral("Name"), js );
  js.jsonString( QStringLiteral("Number"), mNumber );
  SdGraph::json( js );
  }











void SdGraphPartPin::saveState(SdUndo *undo)
  {
  undo->propPartPin( &mPinProp, &mOrigin );
  undo->propTextAndText( &mNumberProp, &mNumberPos, &mNumberRect, &mNumber );
  undo->propTextAndText( &mNameProp, &mNamePos, &mNameRect, nullptr );
  }




void SdGraphPartPin::move(SdPoint offset)
  {
  if( mPinSelect ) mOrigin.move(offset);
  if( mNamSelect ) mNamePos.move(offset);
  if( mNumSelect ) mNumberPos.move(offset);
  }




void SdGraphPartPin::rotate(SdPoint center, SdPropAngle angle)
  {
  if( mPinSelect ) mOrigin.rotate( center, angle );
  if( mNamSelect ) mNamePos.rotate( center, angle );
  if( mNumSelect ) mNumberPos.rotate( center, angle );
  }




void SdGraphPartPin::mirror(SdPoint a, SdPoint b)
  {
  if( mPinSelect ) mOrigin.mirror( a, b );
  if( mNamSelect ) mNamePos.mirror( a, b );
  if( mNumSelect ) mNumberPos.mirror( a, b );
  }




void SdGraphPartPin::setProp(SdPropSelected &prop)
  {
  if( mPinSelect ) mPinProp = prop.mPartPinProp;
  if( mNamSelect ) mNameProp = prop.mTextProp;
  if( mNumSelect ) mNumberProp = prop.mTextProp;
  }




void SdGraphPartPin::getProp(SdPropSelected &prop)
  {
  if( mPinSelect ) {
    prop.mPartPinProp.append( mPinProp );
    prop.mFilledPropMask |= spsPartPinProp;
    }
  if( mNamSelect ) {
    prop.mTextProp.append( mNameProp );
    prop.mFilledPropMask |= spsTextProp;
    }
  if( mNumSelect ) {
    prop.mTextProp.append( mNumberProp );
    prop.mFilledPropMask |= spsTextProp;
    }
  }




void SdGraphPartPin::setText(int index, QString sour, SdPropText &prop, QWidget *parent)
  {
  Q_UNUSED(index)
  //Test if pin name unical
  if( getParent() ) {
    bool unical = true;
    getParent()->forEach( dctPartPin, [&unical, sour, this] (SdObject *obj) -> bool {
      SdGraphPartPin *pin = dynamic_cast<SdGraphPartPin*>( obj );
      if( pin && pin != this && pin->getPinNumber() == sour ) unical = false;
      return unical;
      });
    if( !unical ) {
      QMessageBox::warning( parent, QObject::tr("Error"), QObject::tr("Pin with number '%1' already present. Number must be unical").arg(sour) );
      return;
      }
    }
  mNumber     = sour;
  mNumberProp = prop;
  }




void SdGraphPartPin::selectByPoint(const SdPoint p, SdSelector *selector)
  {
  //Test pin point
  if( mPinProp.mLayer.isEdited() && mOrigin == p ) {
    if( !getSelector() ) mNumSelect = mNamSelect = false;
    mPinSelect = true;
    selector->insert( this );
    }
  //Test pin name
  if( mNameProp.mLayer.isEdited() ) {
    if( mNameRect.isPointInside(p) ) {
      if( !getSelector() ) mPinSelect = mNumSelect = false;
      mNamSelect = true;
      selector->insert( this );
      }
    }
  //Test pin number
  if( mNumberProp.mLayer.isEdited() ) {
    if( mNumberRect.isPointInside(p) ) {
      if( !getSelector() ) mPinSelect = mNamSelect = false;
      mNumSelect = true;
      selector->insert( this );
      }
    }
  }




void SdGraphPartPin::selectByRect(const SdRect &r, SdSelector *selector)
  {
  //Test pin point
  if( mPinProp.mLayer.isEdited() && r.isPointInside( mOrigin ) ) {
    if( !getSelector() ) mNumSelect = mNamSelect = false;
    mPinSelect = true;
    selector->insert( this );
    }
  //Test pin name
  if( mNameProp.mLayer.isEdited() ) {
    if( r.isAccross( mNameRect ) ) {
      if( !getSelector() ) mPinSelect = mNumSelect = false;
      mNamSelect = true;
      selector->insert( this );
      }
    }
  //Test pin number
  if( mNumberProp.mLayer.isEdited() ) {
    if( r.isAccross( mNumberRect ) ) {
      if( !getSelector() ) mPinSelect = mNamSelect = false;
      mNumSelect = true;
      selector->insert( this );
      }
    }
  }




void SdGraphPartPin::select(SdSelector *selector)
  {
  mPinSelect = mNamSelect = mNumSelect = true;
  if( selector != nullptr )
    selector->insert( this );
  }




bool SdGraphPartPin::isVisible()
  {
  return mPinProp.mLayer.isVisible() || mNameProp.mLayer.isVisible() || mNumberProp.mLayer.isVisible();
  }




SdRect SdGraphPartPin::getOverRect() const
  {
  SdRect over( mOrigin, mOrigin );
  over.grow( mNameRect );
  over.grow( mNumberRect );
  return over;
  }




void SdGraphPartPin::draw(SdContext *dc)
  {
  //Pin it self
  dc->partPin( mOrigin, mPinProp.mLayer.layer() );
  if( sdEnvir->mShowPads )
    sdEnvir->getPad( mPinProp.mPinType.str() ).draw( dc, mOrigin, stmThrough );

  //Pin name
  //At first calc over rectangle
  dc->text( mNamePos, mNameRect, QStringLiteral("  "), mNameProp );
  //At second draw rectangle
  if( mNameProp.mLayer.layer()->isVisible() ) {
    dc->setPen( 0, mNameProp.mLayer.layer(), dltDashed );
    dc->rect( mNameRect );
    }

  //Pin number as over rectangle.
  dc->text( mNumberPos, mNumberRect, mNumber, mNumberProp );
  }




//!
//! \brief draw3d Draws object in 3d space
//! \param f      3d draw functions with predefined 3d context
//!
void SdGraphPartPin::draw3d(QOpenGLFunctions_2_0 *f) const
  {
  //Draw pad only
  sdEnvir->getPad( mPinProp.mPinType.str() ).draw3d( f, mOrigin );
  }




int SdGraphPartPin::behindCursor(SdPoint p)
  {
  //Test pin point
  if( mPinProp.mLayer.isVisible() && mOrigin == p ) return getSelector() && mPinSelect ? ELEM_SEL : ELEM_UNSEL;
  //Test pin name
  if( mNameProp.mLayer.isVisible() && mNameRect.isPointInside(p) ) return getSelector() && mNamSelect ? ELEM_SEL : ELEM_UNSEL;
  //Test pin number
  if( mNumberProp.mLayer.isVisible() && mNumberRect.isPointInside(p) ) return getSelector() && mNumSelect ? ELEM_SEL : ELEM_UNSEL;
  return 0;
  }




int SdGraphPartPin::behindText(SdPoint p, SdPoint &org, QString &dest, SdPropText &prop)
  {
  //Тестирование имени вывода
  if( mNumberProp.mLayer.isVisible() && mNumberRect.isPointInside(p) ) {
    org  = mNumberPos;
    dest = mNumber;
    prop = mNumberProp;
    return 1;
    }
  return 0;
  }




//Find snap point on object
void SdGraphPartPin::snapPoint(SdSnapInfo *snap)
  {
  if( snap->match(snapNearestPin) )
    snap->test( this, mOrigin, snapNearestPin );
  }
