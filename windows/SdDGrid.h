/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Grid options dialog
*/
#ifndef SDDGRID_H
#define SDDGRID_H

#include <QDialog>

namespace Ui {
  class SdDGrid;
}

class SdDGrid : public QDialog
  {
    Q_OBJECT

    QPointF mGrid; //Current grid
  public:
    explicit SdDGrid( QPointF curGrid, QWidget *parent = 0);
    ~SdDGrid();

    QPointF getGrid() const { return mGrid; }

  private:
    Ui::SdDGrid *ui;

    //Fill X and Y fields
    void printGrid( QPointF g );

    // QDialog interface
  public slots:
    virtual void accept() override;
  };

#endif // SDDGRID_H
