/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Creation files for smt automat.
*/
#include "SdConfig.h"
#include "SdPExportPlate_Assembler.h"
#include "objects/SdEnvir.h"
#include "objects/SdGraphPartImp.h"

#include <QPushButton>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QTextStream>
#include <QMessageBox>
#include <QList>
#include <QMap>
#include <QLabel>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QHeaderView>


SdPExportPlate_Assembler::SdPExportPlate_Assembler(SdWEditorGraphPlate *editor, SdPItemPlate *plate, int step, SdPMasterList *list, QWidget *parent) :
  QWizardPage( parent ),
  mEditor(editor),
  mPlate(plate)
  {
  //Refistration of master in master list
  setMinimumWidth(800);
  list->addMaster( tr("SMT assemble"), tr("Creates smt assemblion automat files for plate"), step, QString(":/pic/exportSmdAssembler.png") );

  QVBoxLayout *vbox = new QVBoxLayout();
  //Title
  vbox->addWidget( new QLabel( tr("Options for SMT assemble file") ) );

  vbox->addWidget( mReference = new QCheckBox( tr("Use referenced positions by plate origin, else global") ) );

  vbox->addWidget( mComps = new QCheckBox( tr("Export components, else no components") ) );

  vbox->addWidget( mBottom = new QCheckBox( tr("Perform for bottom side") ) );

  vbox->addWidget( mSolder = new QCheckBox( tr("Perform dispence code") ) );

  vbox->addWidget( new QLabel( tr("Columt table delimiter for file result") ) );

  vbox->addWidget( mDelimiter = new QLineEdit("!") );

  setLayout( vbox );
  }


bool SdPExportPlate_Assembler::validatePage()
  {
  //Perform export
  QString fname;
  fname = QFileDialog::getSaveFileName( this, tr("Select file name for save assemble file") );
  if( !fname.isEmpty() ) {
    if( !fname.endsWith( SD_ASSEMBLER_EXTENSION ) )
      fname.append( SD_ASSEMBLER_EXTENSION );
    QFile file(fname);
    if( file.open( QIODevice::WriteOnly ) ) {
      QString os( ";-------------------------------------------------\n" );
      //Title
      os.append( tr(";SaliCAD V%1.%2 smt assemble export file\n").arg(SD_VERSION_MAJOR).arg(SD_VERSION_MINOR) );
      os.append( ";-------------------------------------------------\n\n\n" );
      SdPoint origin;
      bool doRef    = mReference->isChecked();
      if( doRef )
        origin = mPlate->getOrigin();
      bool doComp   = mComps->isChecked();
      bool doBottom = mBottom->isChecked();
      bool doSolder = mSolder->isChecked();
      QString delim = mDelimiter->text();
      //Perform comp
      if( doComp ) {
        os.append( tr(";Components list:\n") );
        mPlate->forEach( dctPartImp, [&] ( SdObject *obj ) -> bool {
          SdPtr<SdGraphPartImp> part(obj);
          if( part.isValid() && part->stratum().isBottom() == doBottom ) {
            //Comp ident
            os.append( part->ident() ).append( delim );
            //Comp name
            os.append( part->getBomItemLine() ).append( delim );
            //Part name
            os.append( part->partTitle() ).append( delim );
            //Angle
            SdPvAngle angle = part->angle();
            if( doBottom ) angle += 180000;
            os.append( QString("%1%2").arg( angle ).arg(delim) );
            //Position
            SdPoint pos = part->getOverRect().center();
            int x;
            if( doBottom )
              x = origin.x() - pos.x();
            else
              x = pos.x() - origin.x();
            int y = pos.y() - origin.y();
            os.append( QString("%1%2%3\n").arg(x).arg(delim).arg(y) );
            }
          return true;
          });
        }
      //Perform solder paste
      if( doSolder ) {
        os.append( tr("\n\n\n;Solder paste dispence points:\n") );
        mPlate->forEach( dctPartImp, [&] ( SdObject *obj ) -> bool {
          SdPtr<SdGraphPartImp> part(obj);
          if( part.isValid() && part->stratum().isBottom() == doBottom ) {
            //QTransform t = part->matrix();
            part->forEachPin( [&] (const SdPartImpPin &pin) {
              //One injection for pin at its center
              //TODO D077 change injection count to according pad form and area
              //Position
              SdPoint pos = pin.mPosition;
              int x;
              if( doBottom )
                x = origin.x() - pos.x();
              else
                x = pos.x() - origin.x();
              int y = pos.y() - origin.y();
              os.append( QString("%1%2%3\n").arg(x).arg(delim).arg(y) );
              });
            }
          return true;
          });
        }
      //Write to file
      file.write( os.toUtf8() );
      file.close();
      }
    }
  return true;
  }

