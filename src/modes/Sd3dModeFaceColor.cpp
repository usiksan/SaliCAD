#include "Sd3dModeFaceColor.h"
#include "SdModeIdents.h"

#include <QHBoxLayout>
#include <QColorDialog>




Sd3dModeFaceColor::Sd3dModeFaceColor(QWidget *parent, SdPItemPart *part) :
  Sd3dMode()
  {
  mPrivate = new Sd3dModeFaceColorPrivate( parent, part );
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




Sd3dModeFaceColorPrivate::Sd3dModeFaceColorPrivate(QWidget *parent, SdPItemPart *part) :
  QObject(parent),
  mView(parent),
  mModel(nullptr),
  mFaceIndex(-1),
  mActive(false)
  {
  //Retrive model from part
  part->forEach( dct3D, [this] ( SdObject *obj ) -> bool {
    SdPtr<Sd3dGraphModel> model(obj);
    if( model.isValid() ) {
      //Model is found
      mModel = model.ptr();
      //...break iterations
      return true;
      }
    return false;
    } );

  //Create table widget
  mTable = new QTableWidget(parent);
  mTable->setColumnCount(2);
  mTable->setVerticalHeaderLabels( { tr("Color code"), tr("Color") } );
  if( mModel != nullptr ) {
    mColorList = mModel->bodyColorList();
    int faceCount = mColorList.count();
    mTable->setRowCount( faceCount );

    //Fill table with colors
    for( int i = 0; i < faceCount; i++ ) {
      //Get face color
      QColor color = mColorList.at(i).mAmbient;

      //In first column - face color name
      mTable->setItem( i, 0, new QTableWidgetItem(color.name()) );

      //In second column - face color itself
      QTableWidgetItem *background = new QTableWidgetItem();
      background->setBackground( color );
      mTable->setItem( i, 1, background );
      }

    connect( mTable, &QTableWidget::cellActivated, this, &Sd3dModeFaceColorPrivate::onCellActivated );
    connect( mTable, &QTableWidget::cellClicked, this, &Sd3dModeFaceColorPrivate::onCellActivated );
    connect( mTable, &QTableWidget::cellChanged, this, &Sd3dModeFaceColorPrivate::onCellChanged );
    }

  QHBoxLayout *hbox = new QHBoxLayout();
  hbox->addStretch(4);
  hbox->addWidget( mTable, 1 );

  parent->setLayout( hbox );
  }




void Sd3dModeFaceColorPrivate::onCellActivated(int row, int column)
  {
  if( mFaceIndex >= 0 ) {
    //Restore previous color of face
    mColorList[mFaceIndex].mAmbient = mFaceColor;
    mFaceIndex = -1;
    }
  mFaceIndex = row;
  mFaceColor = mColorList[mFaceIndex].mAmbient;
  if( column == 0 ) {
    //Clicked on color name
    //We highlight this face with inverted color
    int r = 255 - mFaceColor.red();
    int g = 255 - mFaceColor.green();
    int b = 255 - mFaceColor.blue();
    mColorList[mFaceIndex].mAmbient = QColor( r, g, b );
    }
  else {
    //Call dialog to select color
    QColor color = QColorDialog::getColor( QColor( mTable->item( row, 0 )->text() ), mView );
    if( color.isValid() ) {
      mFaceColor = color;
      mColorList[mFaceIndex].mAmbient = mFaceColor;
      mActive = true;
      mTable->item(row,0)->setText( color.name() );
      mTable->item(row,1)->setBackground( color );
      mActive = false;
      }
    }
  mModel->bodyColorListSet( mColorList );
  mView->update();
  }



void Sd3dModeFaceColorPrivate::onCellChanged(int row, int column)
  {
  if( !mActive && column == 0 ) {
    mActive = true;
    if( mFaceIndex >= 0 ) {
      //Restore previous color of face
      mColorList[mFaceIndex].mAmbient = mFaceColor;
      mFaceIndex = -1;
      }
    mFaceIndex = row;
    mFaceColor = QColor( mTable->item( row, 0 )->text() );
    mColorList[mFaceIndex].mAmbient = mFaceColor;
    mTable->item(row,1)->setBackground( mFaceColor );
    mFaceIndex = -1;
    mActive = false;
    mModel->bodyColorListSet( mColorList );
    mView->update();
    }
  }
