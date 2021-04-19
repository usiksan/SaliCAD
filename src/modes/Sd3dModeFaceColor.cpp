#include "Sd3dModeFaceColor.h"
#include "SdModeIdents.h"

#include <QHBoxLayout>




Sd3dModeFaceColor::Sd3dModeFaceColor(QWidget *parent, SdPItemPart *part) :
  Sd3dMode()
  {
  mPrivate = new Sd3dModeFaceColorPrivate( parent );
  }

Sd3dModeFaceColor::~Sd3dModeFaceColor()
  {
  mPrivate->deleteLater();
  }





bool Sd3dModeFaceColor::draw3d(QOpenGLFunctions_2_0 *f)
  {
  Q_UNUSED(f)
  return false;
  }




int Sd3dModeFaceColor::modeId() const
  {
  return MD_3D_FACE_COLOR;
  }




QString Sd3dModeFaceColor::getStepHelp() const
  {
  return QObject::tr("Click face color name to view selected face and click color cell to select new face color");
  }



QString Sd3dModeFaceColor::getModeThema() const
  {
  return  QString( MODE_HELP "3dModeFaceColor.htm" );
  }




Sd3dModeFaceColorPrivate::Sd3dModeFaceColorPrivate(QWidget *parent)
  {
  //Create table widget
  mTable = new QTableWidget(parent);
  mTable->setColumnCount(2);

  QHBoxLayout *hbox = new QHBoxLayout();
  hbox->addStretch(4);
  hbox->addWidget( mTable, 1 );

  parent->setLayout( hbox );
  }
