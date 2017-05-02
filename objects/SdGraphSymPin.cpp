#include "SdGraphSymPin.h"
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
    mNumberProp = pin->mNumberProp; //Pin number properties
    mNamePos    = pin->mNamePos;    //Pin name position
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
  undo->pr
  }

void SdGraphSymPin::move(SdPoint offset)
  {
  }

void SdGraphSymPin::rotate(SdPoint center, SdAngle angle)
  {
  }

void SdGraphSymPin::mirror(SdPoint a, SdPoint b)
  {
  }

void SdGraphSymPin::setProp(SdProp &prop)
  {
  }

void SdGraphSymPin::getProp(SdProp &prop)
  {
  }

void SdGraphSymPin::setText(int index, QString sour, SdPropText &prop)
  {
  }

void SdGraphSymPin::selectByPoint(const SdPoint p, SdSelector *selector)
  {
  }

void SdGraphSymPin::selectByRect(const SdRect &r, SdSelector *selector)
  {
  }

void SdGraphSymPin::select(SdSelector *selector)
  {
  }

bool SdGraphSymPin::isAble()
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
