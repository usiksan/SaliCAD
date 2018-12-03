/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SDPROPBARPARTPLACE_H
#define SDPROPBARPARTPLACE_H

#include "SdPropBarPartImp.h"

class SdPropBarPartPlace : public SdPropBarPartImp
  {
    Q_OBJECT

    //Set smart mode selection (on middle button)
    QAction   *mSmartMode;

    //Set next number mode selection (on middle button)
    QAction   *mNextNumberMode;

    //Set left button outside comp to sheet selection
    QAction   *mSheetSelection;

    //Sheet list
    QComboBox *mSheetList;

    //Direct component selection, also selection component from list
    QComboBox *mComponent;
  public:
    SdPropBarPartPlace( const QString title );

    //void    setPlaceInfo( QStringList sheetList, QStri QStringList compList, bool smartOrNextNumber, bool sheetSelection );
    //void    getPlaceInfo( QString &sheet, QString &comp, bool &sm)

    //Set new state of smart mode
    void    setSmartMode( bool smartOrNextNumber );
    //Get current state of smart mode
    bool    isSmartMode() const;

    //Set new current state of "from sheet selection" mode
    void    setSheetSelection( bool sheetSelection );
    //Get current state of "from sheet selection" mode
    bool    isSheetSelection() const;

    //Set new sheet list and current sheet
    void    setSheetList( const QStringList sheetList, const QString curSheet );
    //Get current selected sheet
    QString sheet();

    //Set new component list
    void    setComponentList( QStringList list );
    //Get current component
    QString component() const;

  signals:
    void    partSelect( QStringList list );
  };

#endif // SDPROPBARPARTPLACE_H
