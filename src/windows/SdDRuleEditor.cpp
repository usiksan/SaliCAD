/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Rule editor
*/
#include "SdDRuleEditor.h"
#include "ui_SdDRuleEditor.h"

#include "objects/SdPItemPlate.h"
#include "objects/SdProject.h"
#include "objects/SdEnvir.h"
#include "SdDHelp.h"

#include <QPushButton>

SdDRuleEditor::SdDRuleEditor(SdPItemPlate *plate, QWidget *parent) :
  QDialog(parent),
  mPlate(plate),
  ui(new Ui::SdDRuleEditor)
  {
  ui->setupUi(this);

  mPcb = mPlate->rulePcbGet();
  mNet = mPlate->ruleMapGet();

  //Fill pcb block
  ui->mPcbRoadWidth->setText( SdEnvir::instance()->toPhisPcb(mPcb.mRules[ruleRoadWidth]) );
  ui->mPcbPad2Pad->setText( SdEnvir::instance()->toPhisPcb(mPcb.mRules[rulePadPad]) );
  ui->mPcbRoad2Pad->setText( SdEnvir::instance()->toPhisPcb(mPcb.mRules[ruleRoadPad]) );
  ui->mPcbRoad2Road->setText( SdEnvir::instance()->toPhisPcb(mPcb.mRules[ruleRoadRoad]) );

  //Fill net list
  QStringList netList = mPlate->getProject()->netList();
  ui->mNetList->addItems( netList );

  connect( ui->mNetList, &QListWidget::itemSelectionChanged, this, [this] () {
    //Get list of selected nets
    auto items = ui->mNetList->selectedItems();
    //Update previous selected block
    getNetBlock();
    //Create new selected net list
    mNetList.clear();
    for( QListWidgetItem *item : items )
      mNetList.append( item->text() );
    //Show rules for selected nets
    setNetBlock();
    });

  connect( ui->mNetClearAll, &QPushButton::clicked, this, [this] () {
    //Setup all net rules to inherited
    QString str("-");
    ui->mNetRoadWidth->setText(str);
    ui->mNetPad2Pad->setText(str);
    ui->mNetRoad2Pad->setText(str);
    ui->mNetRoad2Road->setText(str);
    });

  connect( ui->mNetApplyAllGaps, &QPushButton::clicked, this, [this] () {
    QString str = ui->mNetAllGaps->text();
    ui->mNetPad2Pad->setText(str);
    ui->mNetRoad2Pad->setText(str);
    ui->mNetRoad2Road->setText(str);
    });

  connect( ui->mPcbApplyAllGaps, &QPushButton::clicked, this, [this] () {
    QString str = ui->mPcbAllGaps->text();
    ui->mPcbPad2Pad->setText(str);
    ui->mPcbRoad2Pad->setText(str);
    ui->mPcbRoad2Road->setText(str);
    });

  connect( ui->buttonBox->button(QDialogButtonBox::Help), &QPushButton::clicked, this, [this] () {
    SdDHelp::help( QString("SdDRuleEditor.htm"), this );
    } );
  }





SdDRuleEditor::~SdDRuleEditor()
  {
  delete ui;
  }




//Setup visual fields for net block from all selected nets
void SdDRuleEditor::setNetBlock()
  {
  //Block for accumulate net rules
  //-2 some various values
  SdRuleBlock blk;
  if( mNetList.count() ) {
    blk = mNet.value( mNetList.at(0) );
    for( int i = 1; i < mNetList.count(); i++ ) {
      SdRuleBlock nb = mNet.value( mNetList.at(i) );
      for( int k = 0; k < ruleLast; k++ )
        if( blk.mRules[k] != nb.mRules[k] )
          blk.mRules[k] = -2;
      }
    }
  else blk.setAllRule(-2);

  writeToLineEdit( blk.mRules[ruleRoadWidth], ui->mNetRoadWidth );
  writeToLineEdit( blk.mRules[rulePadPad], ui->mNetPad2Pad );
  writeToLineEdit( blk.mRules[ruleRoadPad], ui->mNetRoad2Pad );
  writeToLineEdit( blk.mRules[ruleRoadRoad], ui->mNetRoad2Road );
  }






//Store from visual fields to all selected nets
void SdDRuleEditor::getNetBlock()
  {
  SdRuleBlock blk;
  bool isEmpty = true;
  blk.mRules[ruleRoadWidth] = readFromLineEdit( ui->mNetRoadWidth, &isEmpty );
  blk.mRules[rulePadPad] = readFromLineEdit( ui->mNetPad2Pad, &isEmpty );
  blk.mRules[ruleRoadPad] = readFromLineEdit( ui->mNetRoad2Pad, &isEmpty );
  blk.mRules[ruleRoadRoad] = readFromLineEdit( ui->mNetRoad2Road, &isEmpty );

  if( isEmpty ) {
    //Block is empty. Remove all selected blocks
    for( const QString &str : mNetList )
      mNet.remove(str);
    }
  else {
    //Update all blocks
    for( const QString &str : mNetList ) {
      //Get existing block. If block not exist then return empty block
      SdRuleBlock src = mNet.value( str );
      //Update only changed rules
      for( int i = 0; i < ruleLast; i++ )
        if( blk.mRules[i] != -2 )
          src.mRules[i] = blk.mRules[i];
      //Put new block
      mNet.insert( str, src );
      }
    }
  }




void SdDRuleEditor::writeToLineEdit(int v, QLineEdit *edit)
  {
  if( v < -1 )
    edit->clear();
  else if( v < 0 )
    edit->setText( QString("-") );
  else
    edit->setText( SdEnvir::instance()->toPhisPcb(v) );
  }




//Read value from visual editor
int SdDRuleEditor::readFromLineEdit(QLineEdit *edit, bool *isEmpty)
  {
  QString str = edit->text();
  if( str.startsWith( QChar('-') ) )
    return -1;
  *isEmpty = false;
  if( str.isEmpty() )
    return -2;
  return SdEnvir::instance()->fromPhisPcb( str );
  }


void SdDRuleEditor::accept()
  {
  //Apply last selection changes
  getNetBlock();

  //Get pcb values
  bool isEmpty;
  mPcb.mRules[ruleRoadWidth] = readFromLineEdit( ui->mPcbRoadWidth, &isEmpty );
  mPcb.mRules[rulePadPad]    = readFromLineEdit( ui->mPcbPad2Pad, &isEmpty );
  mPcb.mRules[ruleRoadPad]   = readFromLineEdit( ui->mPcbRoad2Pad, &isEmpty );
  mPcb.mRules[ruleRoadRoad]  = readFromLineEdit( ui->mPcbRoad2Road, &isEmpty );

  //Transfer to plate
  SdUndo *undo = mPlate->getUndo();
  if( undo )
    undo->begin( tr("Rule setup"), mPlate, false );
  mPlate->ruleSet( mPcb, mNet, undo );

  //Finish dialog
  QDialog::accept();
  }

