/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdGraphNet.h"
#include "SdJsonIO.h"

SdGraphNet::SdGraphNet() :
  SdGraph(),
  mNetName()
  {

  }

SdGraphNet::SdGraphNet(const QString netName) :
  SdGraph(),
  mNetName(netName)
  {

  }




void SdGraphNet::setNetName(const QString netName, SdUndo *undo)
  {
  //Store previous net name
  if( undo )
    undo->string2( &mNetName, nullptr );
  mNetName = netName;
  }





//!
//! \brief cloneFrom Overrided function. We copy object from source
//! \param src       Source of object from which copy must be made
//! \param copyMap   Structure for mapping copying substitutes
//! \param next      Make simple or next copy. Next copy available not for all objects.
//!                  For example: pin name A23 with next copy return A24
//!
void SdGraphNet::cloneFrom(const SdObject *src, SdCopyMap &copyMap, bool next)
  {
  SdGraph::cloneFrom( src, copyMap, next );
  SdPtrConst<SdGraphNet> net(src);
  Q_ASSERT( net.isValid() );
  mNetName = net->mNetName;
  }





//!
//! \brief json Overloaded function to write object content into json writer
//!             Overrided function
//! \param js   Json writer
//!
void SdGraphNet::json(SdJsonWriter &js) const
  {
  js.jsonString( QStringLiteral("netName"), mNetName );
  SdGraph::json( js );
  }





//!
//! \brief json Overloaded function to read object content from json reader
//!             Overrided function
//! \param js   Json reader
//!
void SdGraphNet::json(const SdJsonReader &js)
  {
  js.jsonString( QStringLiteral("netName"), mNetName );
  SdGraph::json( js );
  }









//Set new properties to net
void SdGraphNet::setProp(SdPropSelected &prop)
  {
  prop.mWireName.assignTo( mNetName );
  }



//Get properties from net
void SdGraphNet::getProp(SdPropSelected &prop)
  {
  prop.mWireName.append( mNetName );
  }



void SdGraphNet::saveState(SdUndo *undo)
  {
  undo->string2( &mNetName, nullptr );
  }
