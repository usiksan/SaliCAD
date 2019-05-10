#include "SdWEditor3d.h"
#include "SdWView3d.h"

#include <QVBoxLayout>
#include <QDebug>

SdWEditor3d::SdWEditor3d(SdProjectItem *item, QWidget *parent) :
  SdWEditor( parent ),
  mItem(item)
  {
  QVBoxLayout *box = new QVBoxLayout();
  mView = new SdWView3d( item, this );
  box->setMargin(0);
  box->addWidget( mView );
  mView->move( 0, 0 );
//  mView->resize( 300, 300 );
  //setViewport( mView );
  setLayout( box );
  }

void SdWEditor3d::paintEvent(QPaintEvent *event)
  {
//  qDebug() << "paint event 3d";
  event->accept();
//  mView->update();
  }



SdProjectItem *SdWEditor3d::getProjectItem() const
  {
  return mItem;
  }

