/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Part implementation properties bar for component insertion and edit in pcb editor
*/
#ifndef SDPROPBARPARTIMP_H
#define SDPROPBARPARTIMP_H

#include "objects/SdPropPartImp.h"
#include "SdPropBar.h"

#include <QComboBox>
#include <QList>


class SdPropBarPartImp : public SdPropBar
  {
    Q_OBJECT

  protected:
    //Part direction
    QComboBox *mDirection;

    //Mirror
    QAction   *mMirror;

    //Bottom side
    QAction   *mBottom;

    //Top side
    QAction   *mTop;

    QAction   *mAlignToGrid;
  public:
    SdPropBarPartImp( const QString title );

    void setPropPartImp( const SdPropPartImp &propPartImp );
    void getPropPartImp( SdPropPartImp &propPartImp );

    void setPropPartImp( const SdPropComposerPartImp &propPartImp );
    void getPropPartImp( SdPropComposerPartImp &propPartImp );

  private:
    void reorderDirection();
  };

#endif // SDPROPBARPARTIMP_H
