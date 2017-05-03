#include "SdGraphSymPin.h"
#include "SdSelector.h"
#include <QStringRef>

SdGraphSymPin::SdGraphSymPin() :
  SdGraph(),
  //Different pin part selection
  mPinSelect(false),  //Pin selected
  mNumSelect(false),  //Pin number selected
  mNamSelect(false)  //Pin name selected
  {

  }




SdObject *SdGraphSymPin::copyNext()
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
  undo->propTextAndText( &mNumberProp, &mNumberPos, &mNumberRect, 0 );
  undo->propTextAndText( &mNameProp, &mNamePos, &mNameRect, &mName );
  }



void SdGraphSymPin::move(SdPoint offset)
  {
  if( mPinSelect ) mOrigin.move(offset);
  if( mNamSelect ) mNamePos.move(offset);
  if( mNumSelect ) mNumberPos.move(offset);
  }




void SdGraphSymPin::rotate(SdPoint center, SdAngle angle)
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




void SdGraphSymPin::setProp(SdProp &prop)
  {
  if( mPinSelect ) mPinProp = prop.mSymPinProp;
  if( mNamSelect ) mNameProp = prop.mPinNameProp;
  if( mNumSelect ) mNumberProp = prop.mPinNumberProp;
  }




void SdGraphSymPin::getProp(SdProp &prop)
  {
  if( mPinSelect ) prop.mSymPinProp.append( mPinProp );
  if( mNamSelect ) prop.mPinNameProp.append( mNameProp );
  if( mNumSelect ) prop.mPinNumberProp.append( mNumberProp );
  }




void SdGraphSymPin::setText(int index, QString sour, SdPropText &prop)
  {
  Q_UNUSED(index)
  mName     = sour;
  mNameProp = prop;
  }




void SdGraphSymPin::selectByPoint(const SdPoint p, SdSelector *selector)
  {
  //Test pin point
  if( mPinProp.mLayer.isEdited() && mOrigin == p ) {
    if( !getSelector() ) mNumSelect = mNamSelect = false;
    mPinSelect = true;
    selector->
    DoSelect( selector );
    }
  //Тестирование имени вывода
  if( selector.IsLayerAble( info.nameProp.layer ) ) {
    DRect rect;
    info.nameProp.CalcOverRect(info.name, &rect);
    if( rect.IsPointInside(p) ) {
      if( !GetSelect() ) pinSelect = numSelect = false;
      nameSelect = true;
      DoSelect( selector );
      }
    }
  //Тестирование номера вывода
  if( selector.IsLayerAble( info.numberProp.layer ) ) {
    DRect rect;
    info.numberProp.CalcOverRect(szDublSpace, &rect);
    if( rect.IsPointInside(p) ) {
      if( !GetSelect() ) pinSelect = nameSelect = false;
      numSelect = true;
      DoSelect( selector );
      }
    }
  }

void SdGraphSymPin::selectByRect(const SdRect &r, SdSelector *selector)
  {
  }

void SdGraphSymPin::select(SdSelector *selector)
  {
  }

bool SdGraphSymPin::isVisible()
  {
  }

SdRect SdGraphSymPin::getOverRect() const
  {
  }

void SdGraphSymPin::draw(SdContext *dc)
  {
  }

int SdGraphSymPin::behindCursor(SdPoint p)
  {
  }

int SdGraphSymPin::behindText(SdPoint p, SdPoint &org, QString &dest, SdPropText &prop)
  {

  }

bool SdGraphSymPin::getInfo(SdPoint p, QString &info, bool extInfo)
  {
  }

bool SdGraphSymPin::snapPoint(SdSnapInfo *snap)
  {
  }
