/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdPropBarPartPlace.h"

#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QCompleter>

SdPropBarPartPlace::SdPropBarPartPlace(const QString title) :
  SdPropBarPartImp(title)
  {
  mSideMaskButton = addAction( QIcon(QStringLiteral(":/pic/flipSide.png")), tr("Take component from both side top and bottom") );
  mSideMask = stmTop | stmBottom;
  connect( mSideMaskButton, &QAction::triggered, [=](bool) {
    if( mSideMask == (stmTop | stmBottom) )
      setSideMask( stmTop );
    else if( mSideMask == stmTop )
      setSideMask( stmBottom );
    else
      setSideMask( stmTop | stmBottom );
    emit propChanged();
    });

  mSmartMode = addAction( QIcon(QStringLiteral(":/pic/placeSmartMode.png")), tr("Smart mode on middle button") );
  mSmartMode->setCheckable(true);
  connect( mSmartMode, &QAction::triggered, [=](bool) {
    if( mSmartMode->isChecked() )
      mNextNumberMode->setChecked(false);
    else
      mNextNumberMode->setChecked(true);
    emit propChanged();
    });

  mNextNumberMode = addAction( QIcon(QStringLiteral(":/pic/placeNextId.png")), tr("Selection consistently components on middle button") );
  mNextNumberMode->setCheckable(true);
  connect( mNextNumberMode, &QAction::triggered, [=](bool) {
    if( mNextNumberMode->isChecked() )
      mSmartMode->setChecked(false);
    else
      mSmartMode->setChecked(true);
    emit propChanged();
    });

  mSheetSelection = addAction( QIcon(QStringLiteral(":/pic/placeSheet.png")), tr("Selection from schematic sheet") );
  mSheetSelection->setCheckable(true);
  connect( mSheetSelection, &QAction::triggered, [=](bool) {
    emit propChanged();
    });
  addWidget( new QLabel(tr("Sheets:")) );
  mSheetList = new QComboBox();
  mSheetList->setMinimumWidth(150);
  //Disable handle enter component ident
  mSheetList->setEditable(false);
  //on select other sheet
  connect( mSheetList, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated), [=](int) {
    emit propChanged();
    });
  addWidget( mSheetList );


  addWidget( new QLabel(tr("Component:")) );
  mComponent = new QComboBox();
  mComponent->setMinimumWidth(150);
  //Enable handle enter component ident
  mComponent->setEditable(true);
  //on complete editing
  connect( mComponent->lineEdit(), &QLineEdit::editingFinished, [=]() {
    emit partSelect( {mComponent->currentText()} );
    });
  //on select other component
  connect( mComponent, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated), [=](int) {
    emit partSelect( {mComponent->currentText()} );
    });

  addWidget( mComponent );
  }




//Set new side mask
void SdPropBarPartPlace::setSideMask(SdPvStratum mask)
  {
  mSideMask = mask;
  if( mSideMask == stmTop )
    mSideMaskButton->setIcon( QIcon(QStringLiteral(":/pic/flipSideTop.png")) );
  else if( mSideMask == stmBottom )
    mSideMaskButton->setIcon( QIcon(QStringLiteral(":/pic/flipSideBottom.png")) );
  else
    mSideMaskButton->setIcon( QIcon(QStringLiteral(":/pic/flipSide.png")) );
  }





//Set new state of smart mode
void SdPropBarPartPlace::setSmartMode(bool smartOrNextNumber)
  {
  mNextNumberMode->setChecked(!smartOrNextNumber);
  mSmartMode->setChecked(smartOrNextNumber);
  }





//Get current state of smart mode
bool SdPropBarPartPlace::isSmartMode() const
  {
  return mSmartMode->isChecked();
  }






//Set new current state of "from sheet selection" mode
void SdPropBarPartPlace::setSheetSelection(bool sheetSelection)
  {
  mSheetSelection->setChecked(sheetSelection);
  }



//Get current state of "from sheet selection" mode
bool SdPropBarPartPlace::isSheetSelection() const
  {
  return mSheetSelection->isChecked();
  }




//Set new sheet list and current sheet
void SdPropBarPartPlace::setSheetList(const QStringList sheetList, const QString curSheet)
  {
  mSheetList->clear();
  mSheetList->addItems( sheetList );
  if( curSheet.isEmpty() || mSheetList->findText(curSheet) < 0 )
    mSheetList->setCurrentIndex(0);
  else
    mSheetList->setCurrentText(curSheet);
  }



//Get current selected sheet
QString SdPropBarPartPlace::sheet()
  {
  return mSheetList->currentText();
  }




//Set new component list
void SdPropBarPartPlace::setComponentList(QStringList list)
  {
  mComponent->clear();
  mComponent->addItems(list);
  mComponent->lineEdit()->setCompleter( new QCompleter(list) );
  mComponent->setCurrentText(QString());
  }




//Get current component
QString SdPropBarPartPlace::component() const
  {
  return mComponent->currentText();
  }


