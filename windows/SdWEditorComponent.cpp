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
#include <QMessageBox>
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

     if( mAnotherAuthor ) {
       //For another author editing is prohibited
       mSectionAdd->setEnabled(false);
       mSectionDubl->setEnabled(false);
       mSectionSelect->setEnabled(false);
       mSectionDelete->setEnabled(false);
       mSectionDeleteAll->setEnabled(false);
       }
     else {
       connect( mSectionAdd, &QPushButton::clicked, this, &SdWEditorComponent::sectionAdd );
       connect( mSectionDubl, &QPushButton::clicked, this, &SdWEditorComponent::sectionDubl );
       connect( mSectionSelect, &QPushButton::clicked, this, &SdWEditorComponent::sectionSelect );
       connect( mSectionDelete, &QPushButton::clicked, this, &SdWEditorComponent::sectionDelete );
       connect( mSectionDeleteAll, &QPushButton::clicked, this, &SdWEditorComponent::sectionDeleteAll );
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

     if( mAnotherAuthor ) {
       //mPartAdd->setEnabled(false);
       mPartSelect->setEnabled(false);
       mPartDelete->setEnabled(false);
       }
     else {
       connect( mPartSelect, &QPushButton::clicked, this, &SdWEditorComponent::partSelect );
       connect( mPartDelete, &QPushButton::clicked, this, &SdWEditorComponent::partDelete );
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
  //Update info from component
  fillSections();
  fillParts();
  }



void SdWEditorComponent::sectionAdd()
  {
  SdPItemSymbol *sym = dynamic_cast<SdPItemSymbol*>( SdDGetObject::getObject( dctSymbol, tr("Select symbol for section"), this ) );
  if( sym ) {
    SdSection *section = new SdSection();
    section->updateFromSymbol( sym );
    delete sym;
    SdWSection *ws = new SdWSection( mAnotherAuthor, section, mSectionsTab );
    mSectionsTab->addTab( ws, tr("Section %1").arg(mSectionsTab->count()+1) );
    mSectionsTab->setCurrentWidget( ws );
    mComponent->insertChild( section, mUndo );
    mComponent->updateCreationTime();

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
    SdWSection *ws = new SdWSection( mAnotherAuthor, section, mSectionsTab );
    mSectionsTab->addTab( ws, tr("Section %1").arg(mSectionsTab->count()+1) );
    mSectionsTab->setCurrentWidget( ws );
    mComponent->insertChild( section, mUndo );
    mComponent->updateCreationTime();
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
      SdWSection *ws = new SdWSection( mAnotherAuthor, section, mSectionsTab );
      mSectionsTab->insertTab( index, ws, tr("Section %1").arg(index+1) );
      mSectionsTab->setCurrentWidget( ws );
      mComponent->updateCreationTime();
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
      mComponent->updateCreationTime();

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
    SdObject *obj = SdObjectFactory::extractObject( sect->getSymbolTitle(), sect->getSymbolAuthor(), true, this );
    if( obj ) {
      SdPItemSymbol *sym = dynamic_cast<SdPItemSymbol*>( obj );
      if( sym ) {
        mSymbolViewer->setItem( sym, true );
        return;
        }
      delete obj;
      }
    }
  mSymbolViewer->setItem( nullptr, true );
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
    mComponent->updateCreationTime();

    mPartDefault->setEnabled( mPartTable->count() );
    mPartSelect->setEnabled( mPartTable->count() );
    mPartDelete->setEnabled( mPartTable->count() );
    mPartDeleteAll->setEnabled( mPartTable->count() );
    }
  }



void SdWEditorComponent::partSelect()
  {
  int cur = mPartTable->currentRow();
  qDebug() << Q_FUNC_INFO << cur;
  if( cur >= 0 ) {
    SdPartVariant *var = getPartVariant(cur);
    if( var ) {
      SdPItemPart *part = dynamic_cast<SdPItemPart*>( SdDGetObject::getObject( dctPart, tr("Select part for component"), this ) );
      var->updateFromPart( part );
      mComponent->updateCreationTime();
      fillParts();
      }
    }
  }




void SdWEditorComponent::partDelete()
  {
  int cur = mPartTable->currentRow();
  qDebug() << Q_FUNC_INFO << cur;
  if( cur >= 0 ) {
    SdPartVariant *var = getPartVariant(cur);
    if( var ) {
      if( QMessageBox::question( this, tr("Attention!"), tr("You attemption to delete part %1").arg(var->getTitle())) == QMessageBox::Yes ) {
        mComponent->deleteChild( var, mUndo );
        mComponent->updateCreationTime();
        fillParts();
        }
      }
    }
  }





void SdWEditorComponent::partDeleteAll()
  {
  if( QMessageBox::question( this, tr("Attention!"), tr("Are You sure delete All parts?") ) == QMessageBox::Yes ) {
    //TODO D016 add delete all parts
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
        mPartViewer->setItem( part, true );
        return;
        }
      delete obj;
      }
    }
  mPartViewer->setItem( nullptr, true );
  }




void SdWEditorComponent::fillSections()
  {
  //Remove previous tabs
  mSectionsTab->clear();

  //Fill with actual tabs
  mComponent->forEach( dctSection, [this] (SdObject *obj) -> bool {
    SdSection *section = dynamic_cast<SdSection*>(obj);
    if( section ) {
      SdWSection *ws = new SdWSection( mAnotherAuthor, section, mSectionsTab );
      mSectionsTab->addTab( ws, tr("Section %1").arg(mSectionsTab->count()+1) );
      }
    return true;
    });

  //Enable-disable buttons on editing enable
  if( !mAnotherAuthor ) {
    mSectionDelete->setEnabled( mSectionsTab->count() );
    mSectionDeleteAll->setEnabled( mSectionsTab->count() );
    mSectionSelect->setEnabled( mSectionsTab->count() );
    }
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
  //Remove previous list
  mPartTable->clear();

  mPartTable->setSortingEnabled(false);

  //Fill new list with default detecting
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
  if( !mAnotherAuthor ) {
    mPartDefault->setEnabled( mPartTable->count() );
    mPartSelect->setEnabled( mPartTable->count() );
    mPartDelete->setEnabled( mPartTable->count() );
    mPartDeleteAll->setEnabled( mPartTable->count() );
    }
  }




SdPartVariant *SdWEditorComponent::getPartVariant()
  {
  //And find SdPartVariant with this index
  SdPartVariant *prt = mComponent->getPart();
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





