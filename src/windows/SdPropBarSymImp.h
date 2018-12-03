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

#include <QToolBar>

struct SdPropSymImp;

class SdPropBarSymImp : public QToolBar
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

    void setPropSymImp( SdPropSymImp *propSymImp );
    void getPropSymImp( SdPropSymImp *propSymImp );

  signals:
    //On property changed by user
    void propChanged();

  private:
    void setDirection( int dir );
  };

#endif // SDPROPBARSYMIMP_H
