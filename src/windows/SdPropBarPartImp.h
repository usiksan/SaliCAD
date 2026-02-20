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

#include <QToolBar>
#include <QComboBox>
#include <QList>


class SdPropBarPartImp : public QToolBar
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
  signals:
    //On property changed by user
    void propChanged();

  private:
    void reorderDirection();
  };

#endif // SDPROPBARPARTIMP_H
