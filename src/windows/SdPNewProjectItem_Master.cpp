/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Selecting master for creation item
*/
#include "SdPNewProjectItem_Master.h"
#include "SdPNewProjectItem.h"
#include "SdDGetObject.h"

#include "library/SdLibraryStorage.h"

//Master dialogs
#include "master/SdDMasterPartDoubleRect.h"
#include "master/SdDMasterPartDoubleRound.h"
#include "master/SdDMasterPartDoubleSide.h"
#include "master/SdDMasterPartDoubleSideLR.h"
#include "master/SdDMasterPartQuadSide.h"
#include "master/SdDMasterPartSingleLine.h"

#include "master/SdDMasterSymbolConnector.h"
#include "master/SdDMasterSymbolIc.h"

#include "master/SdDMasterSheetDecorator.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFile>
#include <QSettings>
#include <QMessageBox>


SdPNewProjectItem_Master::SdPNewProjectItem_Master(SdProjectItemPtr *item, SdProject *prj, QWidget *parent) :
  QWizardPage(parent),
  mItemPtr(item),
  mProject(prj)
  {

  setTitle( tr("Creation new project object") );
  setSubTitle( tr("Select master to object creation") );

  QHBoxLayout *hlay = new QHBoxLayout();
  QVBoxLayout *vlay = new QVBoxLayout();
  vlay->addWidget( new QLabel( tr("Select master type") ) );
  mMasterType = new QListWidget();
  vlay->addWidget( mMasterType );
  hlay->addLayout( vlay );

  vlay = new QVBoxLayout();
  vlay->addWidget( mImage = new QLabel() );
  hlay->addLayout( vlay );

  vlay = new QVBoxLayout();
  vlay->addWidget( new QLabel( tr("Description")) );
  vlay->addWidget( mDescription = new QTextEdit() );
  mDescription->setReadOnly(true);
  hlay->addLayout( vlay );

  setLayout( hlay );

  connect( mMasterType, &QListWidget::currentRowChanged, this, [this] ( int row ) {
    if( row >= 0 ) {
      mDescription->setText( mDescriptions.at(row) );
      mImage->setPixmap( QPixmap(mImages.at(row)) );
      }
    } );
  }




bool SdPNewProjectItem_Master::validatePage()
  {
  int index = mMasterType->currentRow();
  if( index >= 0 )
    return mFuncs[index]( *mItemPtr, this );
  return false;
  }




int SdPNewProjectItem_Master::nextId() const
  {
  return SDP_NPI_NAME;
  }




void SdPNewProjectItem_Master::initializePage()
  {
  //Fill object classes
  switch( (*mItemPtr)->getClass() ) {



    //Masters for sheet
    case dctSheet :
      addMaster( tr("Sheet decorator"), tr("Creates empty schematic sheet with inserted sheet form"),
                 QString(":/pic/sheetMasterDecorator.png"), [this] ( SdProjectItem *item, QWidget *p ) -> bool {
        Q_UNUSED(p)
        QScopedPointer<SdProject> mPastePrj( sdObjectOnly<SdProject>( SdLibraryStorage::instance()->cfObjectGet( SdDGetObject::getObjectUid( dctProject, QObject::tr("Select form to insert"), this, "form") ) )  );
        if( mPastePrj.isNull() ) return false;

        SdPItemSheet *sheet = mPastePrj->getFirstSheet();

        if( sheet == nullptr ) return false;

        SdSelector paste;
        //Select all objects in sheet
        sheet->forEach( dctAll, [&paste] (SdObject *obj) -> bool {
          SdGraph *graph = dynamic_cast<SdGraph*>(obj);
          if( graph != nullptr )
            graph->select( &paste );
          return true;
          });

        item->insertObjects( SdPoint(), &paste, nullptr, nullptr, false );

        return true;
        });
      break;



    //Masters for symbol
    case dctSymbol :
      addMaster( tr("Connectors"), tr("Creates symbol for connector"),
                 QString(":/pic/symbolMasterConnector.png"), [] ( SdProjectItem *item, QWidget *p ) -> bool {
        SdDMasterSymbolConnector dlg( item, p );
        return dlg.exec();
        });

      addMaster( tr("Integrated circuits"), tr("Creates symbol for integrated circuit"),
                 QString(":/pic/symbolMasterIc.png"), [] ( SdProjectItem *item, QWidget *p ) -> bool {
        SdDMasterSymbolIc dlg( item, p );
        return dlg.exec();
        });

      break;



    //Masters for part
    case dctPart :

      addMaster( tr("Two pins rectangle part"), tr("Creates part with exact two pins and rectangle body (resistor, capacitor and so on)"),
                 QString(":/pic/partMasterDoubleRect.png"), [] ( SdProjectItem *item, QWidget *p ) -> bool {
        SdDMasterPartDoubleRect dlg( item, p );
        return dlg.exec();
        });

      addMaster( tr("Two pins round part"), tr("Creates part with exact two pins and round body"),
                 QString(":/pic/partMasterDoubleRound.png"), [] ( SdProjectItem *item, QWidget *p ) -> bool {
        SdDMasterPartDoubleRound dlg( item, false, p );
        return dlg.exec();
        });

      addMaster( tr("Two through pins round part"), tr("Creates part with exact two pins and round body"),
                 QString(":/pic/partMasterDoubleRound2.png"), [] ( SdProjectItem *item, QWidget *p ) -> bool {
        SdDMasterPartDoubleRound dlg( item, true, p );
        return dlg.exec();
        });

      addMaster( tr("Single row pins part"), tr("Creates part with rectangle body and single row of pins at middle of part"),
                 QString(":/pic/partMasterSingleSide.png"), [] ( SdProjectItem *item, QWidget *p ) -> bool {
        SdDMasterPartSingleLine dlg( item, p );
        return dlg.exec();
        });

      addMaster( tr("Two sided pins part"), tr("Creates part with rectangle body and two pins rows by top and bottom sides"),
                 QString(":/pic/partMasterDoubleSide.png"), [] ( SdProjectItem *item, QWidget *p ) -> bool {
        SdDMasterPartDoubleSide dlg( item, p );
        return dlg.exec();
        });

      addMaster( tr("Two sided pins part left-right"), tr("Creates part with rectangle body and two pins columns by left and right sides"),
                 QString(":/pic/partMasterDoubleSideLR.png"), [] ( SdProjectItem *item, QWidget *p ) -> bool {
        SdDMasterPartDoubleSideLR dlg( item, p );
        return dlg.exec();
        });

      addMaster( tr("Four sided pins part"), tr("Creates part with rectangle body and pins on all four sides"),
                 QString(":/pic/partMasterQuadSide.png"), [] ( SdProjectItem *item, QWidget *p ) -> bool {
        SdDMasterPartQuadSide dlg( item, p );
        return dlg.exec();
        });

      break;

    }

  mMasterType->setCurrentRow( 0 );
  }




void SdPNewProjectItem_Master::addMaster(QString title, QString descr, QString img, SdMasterFun fun)
  {
  mMasterType->addItem( title );
  mDescriptions.append( descr );
  mImages.append(img);
  mFuncs.append(fun);
  }
