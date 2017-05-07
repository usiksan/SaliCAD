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
#include "objects/SdProject.h"
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>

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
    psym->addWidget( mSymbolViewer = new SdWEditorGraphSymbol(0,symbolViewer) );
   QGroupBox *sectionBox = new QGroupBox( tr("Sections"), sym );
   box->addWidget( sectionBox );
    box = new QHBoxLayout();
    sectionBox->setLayout( box );
    mSectionsTab = new QTabWidget(sectionBox);
    box->addWidget( mSectionsTab );
    QVBoxLayout *buts = new QVBoxLayout();
    box->addLayout( buts );
     buts->addWidget( mSectionAdd = new QPushButton( tr("Add section") )  );
     buts->addWidget( mSectionSelect = new QPushButton( tr("Select symbol")) );
     buts->addWidget( mSectionDelete = new QPushButton( tr("Delete section")) );

     connect( mSectionAdd, &QPushButton::clicked, this, &SdWEditorComponent::sectionAdd );
     connect( mSectionSelect, &QPushButton::clicked, this, &SdWEditorComponent::sectionSelect );
     connect( mSectionDelete, &QPushButton::clicked, this, &SdWEditorComponent::sectionDelete );




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
    mPartTable = new QWidget(partBox);
    box->addWidget( mPartTable );
    buts = new QVBoxLayout();
    box->addLayout( buts );
     buts->addWidget( mPartAdd = new QPushButton( tr("Add part")) );
     buts->addWidget( mPartSelect = new QPushButton( tr("Select part")) );
     buts->addWidget( mPartDelete = new QPushButton( tr("Delete part")) );

  //Params
  QGroupBox *param = new QGroupBox( tr("Params"), splitter );
  splitter->addWidget( param );
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
  SdSection *section = new SdSection();
  SdWSection *ws = new SdWSection( section, mSectionsTab );
  mSectionsTab->addTab( ws, tr("Section %1").arg(mSectionsTab->count()+1) );
  mComponent->insertChild( section, mUndo );
  }

void SdWEditorComponent::sectionSelect()
  {

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

