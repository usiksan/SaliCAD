/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Visual Editor for component
*/
#include "SdWEditorComponent.h"
#include "SdWEditorGraphSymbol.h"
#include "SdWEditorGraphPart.h"
#include "SdWSection.h"
#include "SdDGetObject.h"
#include "objects/SdProject.h"
#include "objects/SdPartVariant.h"
#include "objects/SdObjectFactory.h"
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QDebug>

SdWEditorComponent::SdWEditorComponent(SdPItemComponent *comp, QWidget *parent) :
  SdWEditor( parent ),
  mComponent(comp)
  {
  mUndo = mComponent->getUndo();

  mAnotherAuthor = mComponent->isAnotherAuthor();

  QSplitter *splitter = new QSplitter( Qt::Vertical, this );
  QVBoxLayout *lay = new QVBoxLayout();
  lay->addWidget( splitter );
  setLayout( lay );

  //Symbol
  QGroupBox *sym = new QGroupBox( tr("Symbol"), splitter );
  splitter->addWidget( sym );
  QHBoxLayout *box = new QHBoxLayout();
  sym->setLayout( box );
   QGroupBox *symbolViewer = new QGroupBox( tr("Preview"), sym );
   box->addWidget( symbolViewer );
    QHBoxLayout *psym = new QHBoxLayout();
    symbolViewer->setLayout( psym );
    psym->addWidget( mSymbolViewer = new SdWEditorGraphSymbol(0,symbolViewer) );
   QGroupBox *sectionBox = new QGroupBox( tr("Sections"), sym );
   box->addWidget( sectionBox );
    box = new QHBoxLayout();
    sectionBox->setLayout( box );
    mSectionsTab = new QTabWidget(sectionBox);
    box->addWidget( mSectionsTab );
    connect( mSectionsTab, &QTabWidget::currentChanged, this, &SdWEditorComponent::onCurrentSection );
    QVBoxLayout *buts = new QVBoxLayout();
    box->addLayout( buts );
     buts->addWidget( mSectionAdd = new QPushButton( tr("Add section") )  );
     buts->addWidget( mSectionDubl = new QPushButton( tr("Dubl section") ) );
     buts->addWidget( mSectionSelect = new QPushButton( tr("Select symbol") ) );
     buts->addWidget( mSectionDelete = new QPushButton( tr("Delete section") ) );

     if( anotherAuthor ) {

       }
     else {
       connect( mSectionAdd, &QPushButton::clicked, this, &SdWEditorComponent::sectionAdd );
       connect( mSectionDubl, &QPushButton::clicked, this, &SdWEditorComponent::sectionDubl );
       connect( mSectionSelect, &QPushButton::clicked, this, &SdWEditorComponent::sectionSelect );
       connect( mSectionDelete, &QPushButton::clicked, this, &SdWEditorComponent::sectionDelete );
       }




  //Part
  QGroupBox *part = new QGroupBox( tr("Part"), splitter );
  splitter->addWidget( part );
  box = new QHBoxLayout();
  part->setLayout( box );
   QGroupBox *partViewer = new QGroupBox( tr("Preview"), part );
   box->addWidget( partViewer );
    QHBoxLayout *ppart = new QHBoxLayout();
    partViewer->setLayout( ppart );
    ppart->addWidget( mPartViewer = new SdWEditorGraphPart(0,part) );
   QGroupBox *partBox = new QGroupBox( tr("Parts"), part );
   box->addWidget( partBox );
    box = new QHBoxLayout();
    partBox->setLayout( box );
    mPartTable = new QListWidget(partBox);
    box->addWidget( mPartTable );
    connect( mPartTable, &QListWidget::currentRowChanged, this, &SdWEditorComponent::onCurrentPart );
    buts = new QVBoxLayout();
    box->addLayout( buts );
     buts->addWidget( mPartAdd = new QPushButton( tr("Add part")) );
     buts->addWidget( mPartSelect = new QPushButton( tr("Select part")) );
     buts->addWidget( mPartDelete = new QPushButton( tr("Delete part")) );
     buts->addWidget( mPartDefault = new QPushButton( tr("Set def part")) );

     connect( mPartAdd, &QPushButton::clicked, this, &SdWEditorComponent::partAdd );
     connect( mPartSelect, &QPushButton::clicked, this, &SdWEditorComponent::partSelect );
     connect( mPartDelete, &QPushButton::clicked, this, &SdWEditorComponent::partDelete );
     connect( mPartDefault, &QPushButton::clicked, this, &SdWEditorComponent::partDefault );

  //Params
  QGroupBox *param = new QGroupBox( tr("Params"), splitter );
  splitter->addWidget( param );

  //Update info from component
  fillSections();
  fillParts();
  }




SdProjectItem *SdWEditorComponent::getProjectItem()
  {
  return mComponent;
  }




void SdWEditorComponent::onActivateEditor()
  {
  }



void SdWEditorComponent::sectionAdd()
  {
  SdPItemSymbol *sym = dynamic_cast<SdPItemSymbol*>( SdDGetObject::getObject( dctSymbol, tr("Select symbol for section"), this ) );
  if( sym ) {
    SdSection *section = new SdSection();
    section->updateFromSymbol( sym );
    delete sym;
    SdWSection *ws = new SdWSection( section, mSectionsTab );
    mSectionsTab->addTab( ws, tr("Section %1").arg(mSectionsTab->count()+1) );
    mSectionsTab->setCurrentWidget( ws );
    mComponent->insertChild( section, mUndo );
    }
  }




void SdWEditorComponent::sectionDubl()
  {

  }





void SdWEditorComponent::sectionSelect()
  {
  SdDGetObject::getObjectName( 0, 0, dctSymbol, tr("Select symbol for section"), this );
  }




void SdWEditorComponent::sectionDelete()
  {
  SdWSection *sw = dynamic_cast<SdWSection*>( mSectionsTab->currentWidget() );
  if( sw ) {
    //Remove from component
    mComponent->deleteChild( sw->getSection(), mUndo );
    mSectionsTab->removeTab( mSectionsTab->currentIndex() );
    //Rename tabs
    renameSymbolTabs();
    }
  }




void SdWEditorComponent::onCurrentSection(int index)
  {
  SdWSection *sw = dynamic_cast<SdWSection*>( mSectionsTab->widget(index) );
  if( sw ) {
    SdSection *sect = sw->getSection();
    SdObject *obj = SdObjectFactory::extractObject( sect->getSymbolTitle(), sect->getSymbolAuthor(), true, this );
    if( obj ) {
      SdPItemSymbol *sym = dynamic_cast<SdPItemSymbol*>( obj );
      if( sym ) {
        mSymbolViewer->setSymbol( sym );
        return;
        }
      delete obj;
      }
    }
  mSymbolViewer->setSymbol( nullptr );
  }




void SdWEditorComponent::partAdd()
  {
  SdPItemPart *part = dynamic_cast<SdPItemPart*>( SdDGetObject::getObject( dctPart, tr("Select part for component"), this ) );
  if( part ) {
    SdPartVariant *pvar = new SdPartVariant();
    pvar->updateFromPart( part );
    delete part;
    mPartTable->addItem( pvar->getTitle() );
    mPartTable->setCurrentRow( mPartTable->count() - 1 );
    onCurrentPart( mPartTable->count() - 1 );
    mComponent->insertChild( pvar, mUndo );
    }
  }



void SdWEditorComponent::partSelect()
  {

  }

void SdWEditorComponent::partDelete()
  {

  }



void SdWEditorComponent::partDefault()
  {
  int cur = mPartTable->currentRow();
  qDebug() << Q_FUNC_INFO << cur;
  if( cur >= 0 ) {
//    mComponent->setDefaultPart( nullptr );
    mComponent->setDefaultPart( getPartVariant(cur) );
    fillParts();
    }
  }




void SdWEditorComponent::onCurrentPart(int index)
  {
  SdPartVariant *prt = getPartVariant( index );
  if( prt ) {
    SdObject *obj = SdObjectFactory::extractObject( prt->getPartTitle(), prt->getPartAuthor(), true, this );
    if( obj ) {
      SdPItemPart *part = dynamic_cast<SdPItemPart*>( obj );
      if( part ) {
        mPartViewer->setPart( part );
        return;
        }
      delete obj;
      }
    }
  mPartViewer->setPart( nullptr );
  }




void SdWEditorComponent::fillSections()
  {
  mComponent->forEach( dctSection, [this] (SdObject *obj) -> bool {
    SdSection *section = dynamic_cast<SdSection*>(obj);
    if( section ) {
      SdWSection *ws = new SdWSection( section, mSectionsTab );
      mSectionsTab->addTab( ws, tr("Section %1").arg(mSectionsTab->count()+1) );
      }
    return true;
    });

  mSectionDelete->setEnabled( mSectionsTab->count() );
  mSectionSelect->setEnabled( mSectionsTab->count() );
  if( mSectionsTab->count() ) {
    //Sections present, select first one
    mSectionsTab->setCurrentIndex(0);
    }
  }



void SdWEditorComponent::renameSymbolTabs()
  {
  for( int i = 0; i < mSectionsTab->count(); i++ )
    mSectionsTab->setTabText( i, tr("Section %1").arg(i+1) );
  }




void SdWEditorComponent::fillParts()
  {
  mPartTable->clear();
  mPartTable->setSortingEnabled(false);
  int def = -1;
  mComponent->forEach( dctPartVariant, [this,&def] (SdObject *obj) -> bool {
    SdPartVariant *prt = dynamic_cast<SdPartVariant*>(obj);
    if( prt ) {
      prt->mVisualIndex = mPartTable->count();
      if( prt->isDefault() ) {
        mPartTable->addItem( tr("[def] %1").arg(prt->getTitle()) );
        def = prt->mVisualIndex;
        }
      else
        mPartTable->addItem( prt->getTitle() );
      }
    return true;
    });

  if( def >= 0 ) {
    mPartTable->setCurrentRow( def );
    onCurrentPart( def );
    }
  else if( mPartTable->count() ) {
    mPartTable->setCurrentRow( 0 );
    onCurrentPart( 0 );
    }
  }




SdPartVariant *SdWEditorComponent::getPartVariant(int index)
  {
  //And find SdPartVariant with this index
  SdPartVariant *prt = nullptr;
  mComponent->forEach( dctPartVariant, [&prt, index] (SdObject *obj) -> bool {
    prt = dynamic_cast<SdPartVariant*>(obj);
    qDebug() << prt->mVisualIndex << prt->getPartTitle();
    if( prt && prt->mVisualIndex == index )
      //SdPartVariant is found. Break iterations
      return false;
    //Continue iterations
    prt = nullptr;
    return true;
    });
  return prt;
  }

