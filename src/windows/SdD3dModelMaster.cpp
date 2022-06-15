/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model master.

  It allow build new 3d and 2d projection model of part
*/
#include "SdD3dModelMaster.h"
#include "SdDPadMaster.h"
#include "script/SdScriptParser3d.h"
#include "script/SdScriptProgramm.h"
#include "objects/SdObjectFactory.h"
#include "objects/Sd3dGraphModel.h"
#include "SdDHelp.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSplitter>
#include <QDialogButtonBox>
#include <QFont>
#include <QMessageBox>
#include <QColorDialog>
#include <QGuiApplication>
#include <QScreen>


//!
//! \brief SdD3dModelMaster Dialog constructor. It creates dialog for build new 3d and 2d projection model of part
//! \param script           Script of 3d model
//! \param part             Pointer to created part object
//! \param parent           Parent widget
//!
SdD3dModelMaster::SdD3dModelMaster(const QString script, SdPItemPart *part, QWidget *parent) :
  QDialog(parent),
  mPart(part),
  mProgramm(nullptr),
  mActive(false)
  {
  //Setup editor size
  resize( qMin( QGuiApplication::primaryScreen()->size().width(), 1600), 700 );

  //Main layout of dialog is vertical
  QVBoxLayout *vlay = new QVBoxLayout();

  //Central part of dialog is splitter
  QSplitter *splitter = new QSplitter();
  vlay->addWidget( splitter );

   //Param table widget
   splitter->addWidget( mParamWidget = new QTableWidget() );
   connect( mParamWidget, &QTableWidget::cellChanged, this, &SdD3dModelMaster::rebuild );
   connect( mParamWidget, &QTableWidget::cellClicked, this, &SdD3dModelMaster::onCellClicked );

   //Part preview widget
   splitter->addWidget( mPreview = new SdWView3d( mPart, this ) );

   //Stretch factor
   splitter->setStretchFactor( 0, 1 );
   splitter->setStretchFactor( 1, 3 );

  //Buttons at dialog bottom
  QDialogButtonBox *dialogButtonBox = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Help );
  vlay->addWidget( dialogButtonBox );
  connect( dialogButtonBox, &QDialogButtonBox::accepted, this, &SdD3dModelMaster::accept );
  connect( dialogButtonBox, &QDialogButtonBox::rejected, this, &SdD3dModelMaster::reject );
  //Help system
  connect( dialogButtonBox, &QDialogButtonBox::helpRequested, this, [this] () { SdDHelp::help( "SdD3dModelMaster.htm", this ); });

  setLayout( vlay );

  //Extract programm source
  mSourceScript = script;
  mParamWidget->clear();
  mParamWidget->setColumnCount(2);
  mParamWidget->setRowCount(0);
  mParamWidget->setHorizontalHeaderLabels( {tr("Parametr name"), tr("Parametr value") } );
  SdScriptParser3d parser( mParamWidget, &mModel );

  mProgramm = parser.parse3d( script, mPart, &mModel );
  rebuild();
  mPreview->fitItem();
  mPreview->update();
  }





SdD3dModelMaster::~SdD3dModelMaster()
  {
  }




//!
//! \brief scriptRelease Return source script modified with input values
//! \return              Modified script
//!
QString SdD3dModelMaster::scriptRelease() const
  {
  //We perform line-by-line scan and replace
  QStringList lines = mSourceScript.split( QChar('\n') );
  for( int i = 0; i < lines.count(); ) {
    QString line = lines.at(i);
    if( line.startsWith( QStringLiteral("#-") ) ) {
      //Remove this and next line
      lines.removeAt(i+1);
      lines.removeAt(i);
      continue;
      }

    if( line.startsWith( QStringLiteral("#f?") ) || line.startsWith( QStringLiteral("#c?") ) || line.startsWith( QStringLiteral("#p?") )  ) {
      //Remove next line and current line replace with name equation
      lines.removeAt(i+1);
      QString variableName = line.mid( 3 );
      SdScriptValueVariablePtr ptr = mProgramm->variable( variableName );
      if( ptr != nullptr ) {
        if( line.startsWith( QStringLiteral("#f?") ) )
          line = QStringLiteral("%1 = %2").arg( variableName ).arg( ptr->toFloat() );
        else if( line.startsWith( QStringLiteral("#c?") ) )
          line = QStringLiteral("%1 = selectColor( \"%2\" )").arg( variableName, ptr->toColor().name() );
        else
          line = QStringLiteral("%1 = selectPad( \"%2\" )").arg( variableName, ptr->toString() );
        lines[i] = line;
        }
      else {
        lines.removeAt(i);
        continue;
        }
      }
    i++;
    }
  return lines.join( QChar('\n') );
  }





void SdD3dModelMaster::rebuild()
  {
  if( mProgramm && !mActive ) {
    mActive = true;
    //Clear previously builded part
    mPart->clear();
    mModel.clear();

    //Build new part
    mProgramm->execute();
    //mPart->insertChild( new Sd3dGraphModel(mModel), nullptr );
    mPart->model()->scriptSet( scriptRelease(), nullptr );

    //Update preview
    mPreview->update();
    mActive = false;
    }
  }




void SdD3dModelMaster::onCellClicked(int row, int column)
  {
  if( column == 1 ) {
    QTableWidgetItem *item = mParamWidget->item( row, column );
    if( item != nullptr ) {
      if( item->type() == SDM3D_INPUT_COLOR ) {
        //Show color selection dialog
        QColor color = QColorDialog::getColor( QColor(item->text()), this, mParamWidget->item(row,0)->text() );
        if( color.isValid() )
          item->setText( color.name() );
        return;
        }
      else if( item->type() == SDM3D_INPUT_PAD ) {
        //Show pad master dialog
        item->setText( SdDPadMaster::build( item->text(), this ) );
        return;
        }
      }
    }
  }
