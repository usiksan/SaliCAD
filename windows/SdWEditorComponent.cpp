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
#include "SdWCommand.h"
#include "objects/SdProject.h"
#include "objects/SdPartVariant.h"
#include "objects/SdObjectFactory.h"
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QDebug>

SdWEditorComponent::SdWEditorComponent(SdPItemComponent *comp, QWidget *parent) :
  SdWEditor( parent ),
  mComponent(comp)
  {
  mUndo = mComponent->getUndo();

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
    psym->addWidget( mSymbolViewer = new SdWEditorGraphView( nullptr, symbolViewer ) );
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
     buts->addWidget( mSectionDeleteAll = new QPushButton( tr("Delete all sections") ) );

     if( mComponent->isEditEnable() ) {
       //Connect signals when edit enabled
       connect( mSectionAdd, &QPushButton::clicked, this, &SdWEditorComponent::sectionAdd );
       connect( mSectionDubl, &QPushButton::clicked, this, &SdWEditorComponent::sectionDubl );
       connect( mSectionSelect, &QPushButton::clicked, this, &SdWEditorComponent::sectionSelect );
       connect( mSectionDelete, &QPushButton::clicked, this, &SdWEditorComponent::sectionDelete );
       connect( mSectionDeleteAll, &QPushButton::clicked, this, &SdWEditorComponent::sectionDeleteAll );
       }
     else {
       //Disable buttons when edit disabled
       mSectionAdd->setEnabled(false);
       mSectionDubl->setEnabled(false);
       mSectionSelect->setEnabled(false);
       mSectionDelete->setEnabled(false);
       mSectionDeleteAll->setEnabled(false);
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
    ppart->addWidget( mPartViewer = new SdWEditorGraphView( nullptr, part ) );
   QGroupBox *partBox = new QGroupBox( tr("Part"), part );
   box->addWidget( partBox );
    box = new QHBoxLayout();
    partBox->setLayout( box );
    mPart = new QLineEdit(partBox);
    mPart->setReadOnly( true );
    box->addWidget( mPart );
    buts = new QVBoxLayout();
    box->addLayout( buts );
     buts->addWidget( mPartSelect = new QPushButton( tr("Select part")) );
     buts->addWidget( mPartDelete = new QPushButton( tr("Delete part")) );

     if( mComponent->isEditEnable() ) {
       //Connect signals when edit enabled
       connect( mPartSelect, &QPushButton::clicked, this, &SdWEditorComponent::partSelect );
       connect( mPartDelete, &QPushButton::clicked, this, &SdWEditorComponent::partDelete );
       }
     else {
       //Disable buttons when edit disabled
       mPartSelect->setEnabled(false);
       mPartDelete->setEnabled(false);
       }

  //Params
  QGroupBox *param = new QGroupBox( tr("Params"), splitter );
  splitter->addWidget( param );

  }




SdProjectItem *SdWEditorComponent::getProjectItem() const
  {
  return mComponent;
  }




void SdWEditorComponent::onActivateEditor()
  {
  SdWEditor::onActivateEditor();

  //Activate menu
  SdWCommand::cmObjectEditEnable->setVisible( !mComponent->isEditEnable() );
  SdWCommand::cmObjectEditDisable->setVisible( mComponent->isEditEnable() );

  //Update info from component
  fillSections();
  fillPart();
  }



void SdWEditorComponent::sectionAdd()
  {
  SdPItemSymbol *sym = dynamic_cast<SdPItemSymbol*>( SdDGetObject::getObject( dctSymbol, tr("Select symbol for section"), this ) );
  if( sym ) {
    SdSection *section = new SdSection();
    section->updateFromSymbol( sym );
    delete sym;
    SdWSection *ws = new SdWSection( mComponent->isEditEnable(), section, mSectionsTab );
    mSectionsTab->addTab( ws, tr("Section %1").arg(mSectionsTab->count()+1) );
    mSectionsTab->setCurrentWidget( ws );
    mComponent->insertChild( section, mUndo );

    mSectionDelete->setEnabled( mSectionsTab->count() != 0 );
    mSectionDeleteAll->setEnabled( mSectionsTab->count() != 0 );
    mSectionDubl->setEnabled( mSectionsTab->count() != 0 );
    mSectionSelect->setEnabled( mSectionsTab->count() != 0 );
    }
  }




void SdWEditorComponent::sectionDubl()
  {
  SdWSection *sw = dynamic_cast<SdWSection*>( mSectionsTab->currentWidget() );
  if( sw ) {
    SdSection *src = sw->getSection();
    SdSection *section = new SdSection();
    section->cloneFrom( src );
    SdWSection *ws = new SdWSection( mComponent->isEditEnable(), section, mSectionsTab );
    mSectionsTab->addTab( ws, tr("Section %1").arg(mSectionsTab->count()+1) );
    mSectionsTab->setCurrentWidget( ws );
    mComponent->insertChild( section, mUndo );
    }
  }





void SdWEditorComponent::sectionSelect()
  {
  SdWSection *sw = dynamic_cast<SdWSection*>( mSectionsTab->currentWidget() );
  if( sw ) {
    SdSection *section = sw->getSection();
    SdPItemSymbol *sym = dynamic_cast<SdPItemSymbol*>( SdDGetObject::getObject( dctSymbol, tr("Select symbol for section"), this ) );
    if( sym ) {
      section->updateFromSymbol( sym );
      delete sym;
      int index = mSectionsTab->currentIndex();
      mSectionsTab->removeTab( index );
      SdWSection *ws = new SdWSection( mComponent->isEditEnable(), section, mSectionsTab );
      mSectionsTab->insertTab( index, ws, tr("Section %1").arg(index+1) );
      mSectionsTab->setCurrentWidget( ws );
      }
    }
  }




void SdWEditorComponent::sectionDelete()
  {
  if( QMessageBox::question( this, tr("Attention!"), tr("You attempting to delete section %1").arg(mSectionsTab->currentIndex()) ) == QMessageBox::Yes ) {
    SdWSection *sw = dynamic_cast<SdWSection*>( mSectionsTab->currentWidget() );
    if( sw ) {
      //Remove from component
      mComponent->deleteChild( sw->getSection(), mUndo );
      mSectionsTab->removeTab( mSectionsTab->currentIndex() );
      //Rename tabs
      renameSymbolTabs();

      mSectionDelete->setEnabled( mSectionsTab->count() != 0 );
      mSectionDeleteAll->setEnabled( mSectionsTab->count() != 0 );
      mSectionDubl->setEnabled( mSectionsTab->count() != 0 );
      mSectionSelect->setEnabled( mSectionsTab->count() != 0 );
      }
    }
  }




void SdWEditorComponent::sectionDeleteAll()
  {
  if( QMessageBox::question( this, tr("Attention!"), tr("Are You sure delete All sections?") ) == QMessageBox::Yes ) {
    //TODO D015 add delete all sections
    }
  }




void SdWEditorComponent::onCurrentSection(int index)
  {
  SdWSection *sw = dynamic_cast<SdWSection*>( mSectionsTab->widget(index) );
  if( sw ) {
    SdSection *sect = sw->getSection();
    mSymbolViewer->setItemByNameAndAuthor( sect->getSymbolTitle(), sect->getSymbolAuthor() );
    }
  else
    mSymbolViewer->setItem( nullptr, true );
  }




void SdWEditorComponent::partSelect()
  {
  SdPartVariant *part = mComponent->getPart();
  if( part == nullptr ) {
    part = new SdPartVariant();
    mComponent->insertChild( part, mUndo );
    }
  SdPItemPart *prt = dynamic_cast<SdPItemPart*>( SdDGetObject::getObject( dctPart, tr("Select part for component"), this ) );
  part->updateFromPart( prt );
  fillPart();
  }




void SdWEditorComponent::partDelete()
  {
  SdPartVariant *part = mComponent->getPart();
  if( part != nullptr ) {
    //Remove object
    part->deleteObject( mUndo );
    //Remove visual element
    mPart->clear();
    mPartViewer->setItem( nullptr, true );
    }
  }






void SdWEditorComponent::fillSections()
  {
  //Remove previous tabs
  mSectionsTab->clear();

  //Fill with actual tabs
  mComponent->forEach( dctSection, [this] (SdObject *obj) -> bool {
    SdSection *section = dynamic_cast<SdSection*>(obj);
    if( section ) {
      SdWSection *ws = new SdWSection( mComponent->isEditEnable(), section, mSectionsTab );
      mSectionsTab->addTab( ws, tr("Section %1").arg(mSectionsTab->count()+1) );
      }
    return true;
    });

  //Enable-disable buttons on editing enable
  if( mComponent->isEditEnable() ) {
    mSectionDelete->setEnabled( mSectionsTab->count() );
    mSectionDeleteAll->setEnabled( mSectionsTab->count() );
    mSectionSelect->setEnabled( mSectionsTab->count() );
    }
  if( mSectionsTab->count() ) {
    //Sections present, select first one
    mSectionsTab->setCurrentIndex(0);
    onCurrentSection(0);
    }
  }



void SdWEditorComponent::renameSymbolTabs()
  {
  for( int i = 0; i < mSectionsTab->count(); i++ )
    mSectionsTab->setTabText( i, tr("Section %1").arg(i+1) );
  }




void SdWEditorComponent::fillPart()
  {
  SdPartVariant *part = mComponent->getPart();
  if( part == nullptr ) {
    mPart->clear();
    mPartViewer->setItem( nullptr, true );
    }
  else {
    mPart->setText( part->getTitle() );
    mPartViewer->setItemByNameAndAuthor( part->getPartTitle(), part->getPartAuthor() );
    }

  if( mComponent->isEditEnable() ) {
    mPartSelect->setEnabled( true );
    mPartDelete->setEnabled( part != nullptr );
    }
  }




