#include "SdGraphScriptRef.h"
#include "SdContainer.h"
#include "SdContext.h"
#include "SdGraphSymImp.h"

SdGraphScriptRef::SdGraphScriptRef() :
  mRefOwner(nullptr),
  mRef(nullptr),
  mCalculated(false)
  {

  }



SdGraphScriptRef::SdGraphScriptRef(const QString &nm) :
  mRefOwner(nullptr),
  mRef(nullptr),
  mName(nm),
  mCalculated(false)
  {

  }



QString SdGraphScriptRef::refName() const
  {
  if( checkRef() ) {
    SdPtr<SdGraphSymImp> sym(mRef);
    if( sym.isValid() )
      return sym->ident() + QStringLiteral(".") + mParam;
    return mParam;
    }
  return mName;
  }





//!
//! \brief refOffset Returns relative offset of referenced object relative org point
//! \param org       Origin point
//! \return          Relative offset of referenced object relative org point
//!
QPoint SdGraphScriptRef::refOffset(QPoint org) const
  {
  if( checkRef() ) {
    SdPtrConst<SdGraph> graph(mRef);
    if( graph.isValid() ) {
      return graph->getOverRect().center() - org;
      }
    }
  return QPoint{};
  }











void SdGraphScriptRef::draw(SdPoint p, const SdPropText &prop, SdContext *dc, bool drawValue)
  {
  //Build name part
  QString namePart = (mCalculated ? QStringLiteral("*") : QLatin1String("")) + refName() + QStringLiteral(" = ");
  //Draw name part
  dc->text( p, mOverName, namePart, prop );
  if( drawValue ) {
    //Draw value part
    //Calc value position at right of name part
    p.rx() = mOverName.right();
    mOriginValue = p;
    QString valuePart = valueGet();
    if( valuePart.simplified().isEmpty() )
      valuePart = QStringLiteral("none");
    dc->text( p, mOverValue, valuePart, prop );
    }
  }




//!
//! \brief drawLink Draws line to link object
//! \param p        Origin of script
//! \param dc       Drawing context
//!
void SdGraphScriptRef::drawLink( SdPoint org, SdContext *dc) const
  {
  //If no reference then nothing draw
  if( !mRefOffset.isNull() ) {
    SdPoint src = mOverName.center();
    org += mRefOffset;

    dc->line( src, org );
    dc->circle( org, 50 );
    }
  }



QString SdGraphScriptRef::valueGet() const
  {
  if( checkRef() )
    return mRef->paramGet( mParam );
  return mValue;
  }



void SdGraphScriptRef::valueSet(const QString &v)
  {
  mCalculated = true;
  if( checkRef() )
    mRef->paramSet( mParam, v );
  else
    mValue = v;
  }



void SdGraphScriptRef::refAssign(SdContainer *refOwner, SdGraphParam *ref, QString param)
  {
  mRefOwner = refOwner;
  mRef = ref;
  if( !param.isEmpty() )
    mParam = param;
  checkRef();
  }



void SdGraphScriptRef::jsonWrite(SdJsonWriter &js) const
  {
  SdObject::writePtr( mRefOwner, QStringLiteral("owner"), js.ref() );
  SdObject::writePtr( mRef, QStringLiteral("ref"), js.ref() );
  js.jsonString( "name", mName );
  js.jsonString( "param", mParam );
  js.jsonString( "value", mValue );
  }




void SdGraphScriptRef::jsonRead(SdJsonReader &js)
  {
  mRefOwner = SdObject::readPtrClass<SdContainer>( QStringLiteral("owner"), js.property(), js.object() );
  mRef = SdObject::readPtrClass<SdGraphParam>( QStringLiteral("ref"), js.property(), js.object() );
  js.jsonString( "name", mName );
  js.jsonString( "param", mParam );
  js.jsonString( "value", mValue );
  }




bool SdGraphScriptRef::checkRef() const
  {
  if( mRef == nullptr )
    return false;
  if( mRefOwner->isChild(mRef) )
    return true;
  mRefOwner = nullptr;
  mRef = nullptr;
  return false;
  }
