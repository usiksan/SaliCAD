/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Pads dialog. Show and allow edit pin-to-pad association with pad creation master.
*/
#ifndef SDDPADS_H
#define SDDPADS_H

#include "objects/SdPadAssociation.h"
#include "objects/SdPItemPlate.h"

#include <QDialog>

namespace Ui {
  class SdDPads;
  }

class SdDPads : public QDialog
  {
    Q_OBJECT

    SdPadMap           mMap;    //Edited pin-to-pad association
    SdPItemPlate      *mPlate;  //Plate, owned pin-to-pad association
    QString            mUid;
  public:
    explicit SdDPads( SdPItemPlate *plate, const QString associationName, SdPadMap map, QWidget *parent = nullptr);
    ~SdDPads();

    QString  getAssociationUid() const { return mUid; }

    QString  getAssociationName() const;

    //Retrive current selected pin from association table
    QString  getCurrentPin() const;

    SdPadMap getPadMap() const { return mMap; }

    static QString selectPinType( QWidget *parent );
  protected:
    void changeEvent(QEvent *e);

  private:
    Ui::SdDPads *ui;

    void updatePinTable();

  public slots:
    //Append to association table full list of used in plate pins
    void cmAccumUsedPins();
    //Append single empty pin to visual table. Appending to mMap will after pinType enter
    void cmAppendPin();
    //Delete single pin from visual table and mMap
    void cmDeletePin();
    //Delete all pins (clear pin-to-pad association table)
    void cmDeleteAllPins();


    //Replace all association in table with empty pad (default)
    void cmAssociationClear();
    //Save pin-to-pad association table in local library
    void cmAssociationSave();
    //Assign associations from loaded table
    void cmAssociationUse();
    //Replace pin-to-pad association table with loaded table
    void cmAssociationLoad();

    //Complete edit pin-to-pad association
    void cmCellEditComplete( int row, int column );
    void cmCellClicked( int row, int column );
  };

#endif // SDDPADS_H
