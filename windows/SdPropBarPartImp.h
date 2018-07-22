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

#include <QToolBar>
#include <QComboBox>
#include <QList>

struct SdPropPartImp;

class SdPropBarPartImp : public QToolBar
  {
    Q_OBJECT

    //Part direction
    QComboBox *mDirection;

    //Mirror
    QAction   *mMirror;

    //Bottom side
    QAction   *mBottom;
  public:
    SdPropBarPartImp( const QString title );

    void setPropPartImp( SdPropPartImp *propPartImp );
    void getPropPartImp( SdPropPartImp *propPartImp );

  signals:
    //On property changed by user
    void propChanged();

  private:
    void reorderDirection();
  };

#endif // SDPROPBARPARTIMP_H
