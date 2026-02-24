/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Symbol implementation properties bar for component insertion and edit in sheet editor
*/
#ifndef SDPROPBARSYMIMP_H
#define SDPROPBARSYMIMP_H

#include "objects/SdPropSymImp.h"
#include "SdPropBar.h"

struct SdPropSymImp;

class SdPropBarSymImp : public SdPropBar
  {
    Q_OBJECT

    //Symbol direction
    QAction   *mDir0;
    QAction   *mDir90;
    QAction   *mDir180;
    QAction   *mDir270;

    //Mirror
    QAction   *mMirror;
  public:
    SdPropBarSymImp( const QString title );

    void setPropSymImp( const SdPropSymImp &propSymImp );
    void getPropSymImp( SdPropSymImp &propSymImp );

    void setPropSymImp( const SdPropComposerSymImp &propSymImp );
    void getPropSymImp( SdPropComposerSymImp &propSymImp );

  private:
    void setDirection( int dir );
  };

#endif // SDPROPBARSYMIMP_H
