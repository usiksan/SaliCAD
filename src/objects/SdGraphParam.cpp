/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Any param in sheet
*/
#include "SdGraphParam.h"
#include "SdContext.h"
#include "SdSelector.h"
#include "SvJsonIO.h"

SdGraphParam::SdGraphParam() :
  SdGraphText()
  {

  }

SdGraphParam::SdGraphParam(SdPoint org, const QString &str, SdRect r, const SdPropText &p) :
  SdGraphText( org, str, r, p )
  {
  paramParse(str);
  }




QString SdGraphParam::paramName() const
  {
  QStringList lst( mString.split( QChar('=') ) );

  if( lst.count() == 2 )
    return lst.at(0).simplified();

  return mString;
  }




QString SdGraphParam::paramValueGet() const
  {
  QStringList lst( mString.split( QChar('=') ) );

  if( lst.count() == 2 )
    return lst.at(1).simplified();

  return QString{};
  }









void SdGraphParam::paramValueSet( const QString &value )
  {
  paramParse( paramName() + QStringLiteral(" = ") + value );
  }






void SdGraphParam::paramParse( const QString &param )
  {
  mString = param;

  if( mString.isEmpty() )
    mString = QStringLiteral("none = 0");
  }





void SdGraphParam::setText(int index, QString sour, SdPropText &prop, QWidget *parent)
  {
  Q_UNUSED(index)
  Q_UNUSED(parent)
  paramParse( sour );
  mProp   = prop;
  }




