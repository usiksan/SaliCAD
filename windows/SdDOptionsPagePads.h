/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Default pads association table. Allow select association table from library.
*/
#ifndef SDDOPTIONSPAGEPADS_H
#define SDDOPTIONSPAGEPADS_H

#include <QWidget>
#include <QLineEdit>
#include <QTableWidget>

class SdDOptionsPagePads : public QWidget
  {
    Q_OBJECT

    QLineEdit    *mAssociationName; //Current association name
    QTableWidget *mPadTable;        //Association table
  public:
    explicit SdDOptionsPagePads(QWidget *parent = nullptr);

  signals:

  public slots:
    //Select and load new pad association
    void onSelect();

    void accept();

  private:
    //Fill pad table with pads associations
    void updatePinTable();
  };

#endif // SDDOPTIONSPAGEPADS_H
