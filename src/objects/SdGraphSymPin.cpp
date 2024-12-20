/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical object Symbol pin.
*/
#include "SdGraphSymPin.h"
#include "SdContainer.h"
#include "SdEnvir.h"
#include "SdSelector.h"
#include "SdContext.h"
#include "SdJsonIO.h"

#include <QStringRef>
#include <QMessageBox>

SdGraphSymPin::SdGraphSymPin() :
  SdGraph(),
  //Different pin part selection
  mPinSelect(false),  //Pin selected
  mNumSelect(false),  //Pin number selected
  mNamSelect(false)  //Pin name selected
  {

  }



SdGraphSymPin::SdGraphSymPin(SdPoint org, const SdPropSymPin &pinProp, SdPoint numberPos, const SdPropText &numberProp, SdPoint namePos, const SdPropText &nameProp, const QString name, const QString number) :
  SdGraph(),
  //Different pin parts selection
  mPinSelect(false),  //Pin selected
  mNumSelect(false),  //Pin number selected
  mNamSelect(false)  //Pin name selected
  {
  mOrigin     = org;
  mPinProp    = pinProp;
  mNumberPos  = numberPos;
  mNumberProp = numberProp;
  mNumber     = number;
  mNamePos    = namePos;
  mNameProp   = nameProp;
  mName       = name;
  }




//Draw pin in symbol implementation
void SdGraphSymPin::drawImp(SdContext *dc, const QString pinNumber, bool com)
  {
  //Pin it self
  if( !com )
    dc->symPin( mOrigin, mPinProp.mLayer.layer() );

  //Pin name
  dc->text( mNamePos, mNameRect, mName, mNameProp );

  //Pin number
  SdRect r;
  dc->text( mNumberPos, r, pinNumber.isEmpty() ? mNumber : pinNumber, mNumberProp );
  }





//!
//! \brief cloneFrom Overrided function. We copy object from source
//! \param src       Source of object from which copy must be made
//! \param copyMap   Structure for mapping copying substitutes
//! \param next      Make simple or next copy. Next copy available not for all objects.
//!                  For example: pin name A23 with next copy return A24
//!
void SdGraphSymPin::cloneFrom(const SdObject *src, SdCopyMap &copyMap, bool next)
  {
  SdGraph::cloneFrom( src, copyMap, next );
  SdPtrConst<SdGraphSymPin> pin(src);
  if( pin.isValid() ) {
    mOrigin     = pin->mOrigin;     //Pin origin
    mPinProp    = pin->mPinProp;    //Pin properties
    mNumberPos  = pin->mNumberPos;  //Pin number position
    mNumberRect = pin->mNumberRect; //Pin number over rect
    mNumberProp = pin->mNumberProp; //Pin number properties
    mNumber     = next ? nextText( pin->mNumber ) : pin->mNumber; //Pin number
    mNamePos    = pin->mNamePos;    //Pin name position
    mNameRect   = pin->mNameRect;   //Pin name over rect
    mNameProp   = pin->mNameProp;   //Pin name properties
    mName       = next ? nextText( pin->mName ) : pin->mName;       //Pin name

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
void SdGraphSymPin::json(SdJsonWriter &js) const
  {
  js.jsonPoint( QStringLiteral("Origin"), mOrigin );
  mPinProp.json( js );
  js.jsonPoint( QStringLiteral("NumberPos"), mNumberPos );
  mNumberProp.json( QStringLiteral("Number"), js );
  js.jsonString( QStringLiteral("Number"), mNumber );
  js.jsonPoint( QStringLiteral("NamePos"), mNamePos );
  mNameProp.json( QStringLiteral("Name"), js );
  js.jsonString( QStringLiteral("Name"), mName );
  SdGraph::json( js );
  }




//!
//! \brief json Overloaded function to read object content from json reader
//!             Overrided function
//! \param js   Json reader
//!
void SdGraphSymPin::json(const SdJsonReader &js)
  {
  js.jsonPoint( QStringLiteral("Origin"), mOrigin );
  mPinProp.json( js );
  js.jsonPoint( QStringLiteral("NumberPos"), mNumberPos );
  mNumberProp.json( QStringLiteral("Number"), js );
  js.jsonString( QStringLiteral("Number"), mNumber );
  js.jsonPoint( QStringLiteral("NamePos"), mNamePos );
  mNameProp.json( QStringLiteral("Name"), js );
  js.jsonString( QStringLiteral("Name"), mName );
  SdGraph::json( js );
  }







void SdGraphSymPin::saveState(SdUndo *undo)
  {
  undo->propSymPin( &mPinProp, &mOrigin );
  undo->propTextAndText( &mNumberProp, &mNumberPos, &mNumberRect, &mNumber );
  undo->propTextAndText( &mNameProp, &mNamePos, &mNameRect, &mName );
  }



void SdGraphSymPin::move(SdPoint offset)
  {
  if( mPinSelect ) mOrigin.move(offset);
  if( mNamSelect ) mNamePos.move(offset);
  if( mNumSelect ) mNumberPos.move(offset);
  }




void SdGraphSymPin::rotate(SdPoint center, SdPropAngle angle)
  {
  if( mPinSelect ) mOrigin.rotate( center, angle );
  if( mNamSelect ) mNamePos.rotate( center, angle );
  if( mNumSelect ) mNumberPos.rotate( center, angle );
  }



void SdGraphSymPin::mirror(SdPoint a, SdPoint b)
  {
  if( mPinSelect ) mOrigin.mirror( a, b );
  if( mNamSelect ) mNamePos.mirror( a, b );
  if( mNumSelect ) mNumberPos.mirror( a, b );
  }




void SdGraphSymPin::setProp(SdPropSelected &prop)
  {
  if( mPinSelect ) mPinProp = prop.mSymPinProp;
  if( mNamSelect ) mNameProp = prop.mTextProp;
  if( mNumSelect ) mNumberProp = prop.mTextProp;
  }




void SdGraphSymPin::getProp(SdPropSelected &prop)
  {
  if( mPinSelect ) {
    prop.mSymPinProp.append( mPinProp );
    prop.mFilledPropMask |= spsSymPinProp;
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




void SdGraphSymPin::setText(int index, QString sour, SdPropText &prop, QWidget *parent)
  {
  if( index == 1 ) {
    //Set pin name
    //Test if pin name unical
    if( getParent() ) {
      bool unical = true;
      getParent()->forEach( dctSymPin, [&unical, sour, this] (SdObject *obj) -> bool {
        SdGraphSymPin *pin = dynamic_cast<SdGraphSymPin*>( obj );
        if( pin && pin != this && pin->getPinName() == sour ) unical = false;
        return unical;
        });
      if( !unical ) {
        QMessageBox::warning( parent, QObject::tr("Error"), QObject::tr("Pin with name '%1' already present. Name must be unical").arg(sour) );
        return;
        }
      }
    mName     = sour;
    mNameProp = prop;
    }
  else if( index == 2 ) {
    //Set pin number
    mNumber     = sour;
    mNumberProp = prop;
    }
  }




void SdGraphSymPin::selectByPoint(const SdPoint p, SdSelector *selector)
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




void SdGraphSymPin::selectByRect(const SdRect &r, SdSelector *selector)
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




void SdGraphSymPin::select(SdSelector *selector)
  {
  mPinSelect = mNamSelect = mNumSelect = true;
  if( selector != nullptr )
    selector->insert( this );
  }




bool SdGraphSymPin::isVisible()
  {
  return mPinProp.mLayer.isVisible() || mNameProp.mLayer.isVisible() || mNumberProp.mLayer.isVisible();
  }




SdRect SdGraphSymPin::getOverRect() const
  {
  SdRect over( mOrigin, mOrigin );
  over.grow( mNameRect );
  over.grow( mNumberRect );
  return over;
  }



void SdGraphSymPin::draw(SdContext *dc)
  {
  //Pin it self
  dc->symPin( mOrigin, mPinProp.mLayer.layer() );

  //Pin name
  dc->text( mNamePos, mNameRect, mName, mNameProp );

  //Pin number as over rectangle.
  //At first calc over rectangle
  dc->text( mNumberPos, mNumberRect, mNumber.isEmpty() ? QString("  ") : mNumber, mNumberProp );
  //At second draw rectangle if no pin number assigned
  if( mNumber.isEmpty() && mNumberProp.mLayer.layer()->isVisible() ) {
    dc->setPen( 0, mNumberProp.mLayer.layer(), dltDashed );
    dc->rect( mNumberRect );
    }
  }




int SdGraphSymPin::behindCursor(SdPoint p)
  {
  //Test pin point
  if( mPinProp.mLayer.isVisible() && mOrigin == p ) return getSelector() && mPinSelect ? ELEM_SEL : ELEM_UNSEL;
  //Test pin name
  if( mNameProp.mLayer.isVisible() && mNameRect.isPointInside(p) ) return getSelector() && mNamSelect ? ELEM_SEL : ELEM_UNSEL;
  //Test pin number
  if( mNumberProp.mLayer.isVisible() && mNumberRect.isPointInside(p) ) return getSelector() && mNumSelect ? ELEM_SEL : ELEM_UNSEL;
  return 0;
  }




int SdGraphSymPin::behindText(SdPoint p, SdPoint &org, QString &dest, SdPropText &prop)
  {
  //Check if cursor inside pin name
  if( mNameProp.mLayer.isVisible() && mNameRect.isPointInside(p) ) {
    org  = mNamePos;
    dest = mName;
    prop = mNameProp;
    return 1;
    }
  if( mNumberProp.mLayer.isVisible() && mNumberRect.isPointInside(p) ) {
    org  = mNumberPos;
    dest = mNumber;
    prop = mNumberProp;
    return 2;
    }
  return 0;
  }





//Find snap point on object
void SdGraphSymPin::snapPoint(SdSnapInfo *snap)
  {
  if( snap->match(snapNearestPin) )
    snap->test( this, mOrigin, snapNearestPin );
  }

