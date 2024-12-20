#include "SdDSymbolPartParam.h"
#include "SdDGetObject.h"
#include "SdDHelp.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QSplitter>

SdDSymbolPartParam::SdDSymbolPartParam(SdPItemSymbol *symbol, QWidget *parent)
  : SdDParamBase( parent )
  , mComponent(symbol)
  {
  mUndo = symbol->getUndo();

  //Retrive params
  mParam = symbol->paramTable();

  //Retrive part
  mPartUid = mComponent->partIdGet();

  //Build visual
  QVBoxLayout *vbox = new QVBoxLayout();
  //vbox->setMargin(0);
  QSplitter *split = new QSplitter(this);
  vbox->addWidget( split );
  QDialogButtonBox *box = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Help );
  vbox->addWidget( box );
  connect( box, &QDialogButtonBox::accepted, this, &SdDSymbolPartParam::accept );
  connect( box, &QDialogButtonBox::rejected, this, &SdDSymbolPartParam::reject );
  connect( box->button(QDialogButtonBox::Help), &QPushButton::clicked, this, [this] () {
    SdDHelp::help( QString("SdDSymbolPartParam.htm"), this );
    } );
  setLayout( vbox );

  //On left - part view
  QWidget *w = new QWidget(split);
  split->addWidget( w );
  vbox = new QVBoxLayout();
  vbox->setMargin(0);
  w->setLayout(vbox);
  QHBoxLayout *hbox = new QHBoxLayout();
  hbox->addWidget( new QLabel(tr("Part name (empty for none):")) );
  hbox->addWidget( mPart = new QLineEdit() );
  hbox->addWidget( mPartSelect = new QPushButton( tr("Select part...") ) );
  vbox->addLayout( hbox );
  vbox->addWidget( mPartViewer = new SdWEditorGraphView(this) );

  //On right - parametr table
  w = new QWidget(split);
  split->addWidget( w );
  QVBoxLayout *root = new QVBoxLayout();
  root->setMargin(0);
  w->setLayout(root);
    QHBoxLayout *lay = new QHBoxLayout();
      //Container param table
      vbox = new QVBoxLayout();
        vbox->addWidget( new QLabel(tr("Param table")) );
        vbox->addWidget( mParamTable = new QTableWidget() );
      lay->addLayout( vbox );

      //Buttons
      vbox = new QVBoxLayout();
        vbox->addWidget( mParamAdd = new QPushButton( tr("Add param")) );
        vbox->addWidget( mParamAddDefault = new QPushButton( tr("Add defaults")) );
        vbox->addWidget( mParamCopy = new QPushButton( tr("Copy param")) );
        vbox->addWidget( mParamDelete = new QPushButton( tr("Delete param")) );
        vbox->addWidget( mValueSelector = new QPushButton( tr("Select value...")) );
      lay->addLayout( vbox );
    root->addLayout( lay );

  //Connect signals when edit enabled
  connect( mParamAdd, &QPushButton::clicked, this, &SdDSymbolPartParam::paramAdd );
  connect( mParamAddDefault, &QPushButton::clicked, this, &SdDSymbolPartParam::paramAddDefault );
  connect( mParamDelete, &QPushButton::clicked, this, &SdDSymbolPartParam::paramDelete );
  connect( mParamCopy, &QPushButton::clicked, this, &SdDSymbolPartParam::paramCopy );
  connect( mValueSelector, &QPushButton::clicked, this, &SdDSymbolPartParam::selectValue );
  connect( mPartSelect, &QPushButton::clicked, this, &SdDSymbolPartParam::partSelect );

  fillParams();
  fillPart();
  resize( 1000, 400 );
  }


void SdDSymbolPartParam::partSelect()
  {
  //Get part uid from user
  QString uid = SdDGetObject::getObjectUid( dctPart, tr("Select part for component"), this );
  if( !uid.isEmpty() ) {
    mPartUid = uid;
    fillPart();
    }
  }



void SdDSymbolPartParam::fillPart()
  {
  mPartViewer->setItemById( mPartUid );
  mPart->setText( mPartViewer->itemTitle() );
  }



void SdDSymbolPartParam::accept()
  {
  //Setup param table and part
  if( mPart->text().isEmpty() )
    mPartUid.clear();

  mUndo->begin( tr("Param and/or part are changed"), mComponent, false );
  mComponent->paramTableSet( mParam, mUndo );
  SdPartVariant *part = mComponent->partGet();
  if( part == nullptr ) {
    if( !mPartUid.isEmpty() )
      mComponent->partIdSet( mPartUid, mUndo );
    }
  else {
    if( mPartUid.isEmpty() )
      mComponent->partRemove( mUndo );
    }

  SdDParamBase::accept();
  }
