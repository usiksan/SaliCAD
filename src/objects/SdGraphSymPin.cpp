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



SdGraphSymPin::SdGraphSymPin(SdPoint org, const SdPropSymPin &pinProp, SdPoint numberPos, const SdPropText &numberProp, SdPoint namePos, const SdPropText &nameProp, const QString name) :
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
  dc->text( mNumberPos, r, pinNumber, mNumberProp );
  }




SdObject *SdGraphSymPin::copyNext() const
  {
  //Copy pin with next pinName
  //At first - clone pin
  SdGraphSymPin *pin = new SdGraphSymPin();
  pin->cloneFrom( this );
  //Change name on nextName
  pin->mName = nextText( pin->mName );
  //Return resulting pin
  return pin;
  }




void SdGraphSymPin::cloneFrom(const SdObject *src)
  {
  SdGraph::cloneFrom( src );
  const SdGraphSymPin *pin = dynamic_cast<const SdGraphSymPin*>(src);
  if( pin ) {
    mOrigin     = pin->mOrigin;     //Pin origin
    mPinProp    = pin->mPinProp;    //Pin properties
    mNumberPos  = pin->mNumberPos;  //Pin number position
    mNumberRect = pin->mNumberRect; //Pin number over rect
    mNumberProp = pin->mNumberProp; //Pin number properties
    mNamePos    = pin->mNamePos;    //Pin name position
    mNameRect   = pin->mNameRect;   //Pin name over rect
    mNameProp   = pin->mNameProp;   //Pin name properties
    mName       = pin->mName;       //Pin name

    //Different pin part selection
    mPinSelect = false;  //Pin selected
    mNumSelect = false;  //Pin number selected
    mNamSelect = false;  //Pin name selected
    }
  }




void SdGraphSymPin::writeObject(QJsonObject &obj) const
  {
  SdGraph::writeObject( obj );
  mOrigin.write( QString("Origin"), obj );
  mPinProp.write( obj );
  mNumberPos.write( QString("NumberPos"), obj );
  mNumberProp.write( QString("Number"), obj );
  mNamePos.write( QString("NamePos"), obj );
  mNameProp.write( QString("Name"), obj );
  obj.insert( QString("Name"), mName );
  }




void SdGraphSymPin::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdGraph::readObject( map, obj );
  mOrigin.read( QString("Origin"), obj );
  mPinProp.read( obj );
  mNumberPos.read( QString("NumberPos"), obj );
  mNumberProp.read( QString("Number"), obj );
  mNamePos.read( QString("NamePos"), obj );
  mNameProp.read( QString("Name"), obj );
  mName = obj.value( QString("Name") ).toString();
  }




void SdGraphSymPin::saveState(SdUndo *undo)
  {
  undo->propSymPin( &mPinProp, &mOrigin );
  undo->propTextAndText( &mNumberProp, &mNumberPos, &mNumberRect, nullptr );
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
  Q_UNUSED(index)
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
  dc->text( mNumberPos, mNumberRect, QString("  "), mNumberProp );
  //At second draw rectangle
  if( mNumberProp.mLayer.layer()->isVisible() ) {
    dc->setPen( 0, mNumberProp.mLayer.layer(), dltDashed );
    dc->rect( mNumberRect );
    }
  }




int SdGraphSymPin::behindCursor(SdPoint p)
  {
  //Test pin point
  if( mPinProp.mLayer.isVisible() && mOrigin == p ) return getSelector() && mPinSelect ? SEL_ELEM : UNSEL_ELEM;
  //Test pin name
  if( mNameProp.mLayer.isVisible() && mNameRect.isPointInside(p) ) return getSelector() && mNamSelect ? SEL_ELEM : UNSEL_ELEM;
  //Test pin number
  if( mNumberProp.mLayer.isVisible() && mNumberRect.isPointInside(p) ) return getSelector() && mNumSelect ? SEL_ELEM : UNSEL_ELEM;
  return 0;
  }




int SdGraphSymPin::behindText(SdPoint p, SdPoint &org, QString &dest, SdPropText &prop)
  {
  //Тестирование имени вывода
  if( mNameProp.mLayer.isVisible() && mNameRect.isPointInside(p) ) {
    org  = mNamePos;
    dest = mName;
    prop = mNameProp;
    return 1;
    }
  return 0;
  }





//Find snap point on object
void SdGraphSymPin::snapPoint(SdSnapInfo *snap)
  {
  if( snap->match(snapNearestPin) )
    snap->test( this, mOrigin, snapNearestPin );
  }

