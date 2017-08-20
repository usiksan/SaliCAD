/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Print dialog
*/
#ifndef SDDPRINT_H
#define SDDPRINT_H

#include "objects/SdRect.h"

#include <QDialog>
#include <QtPrintSupport/QPrinter>

namespace Ui {
    class SdDPrint;
  }

//Print areas
#define SDPA_FULL_OBJECT 0 //Print whole object with fit to page
#define SDPA_SELECTION   1 //Print selected object with fit to page
#define SDPA_WINDOW      2 //Print window with fit it to page


class SdDPrint : public QDialog
  {
    Q_OBJECT

    double    mPPM;     //PPM is how much phys in one logical
    QPrinter *mPrinter; //Printer, page print to
  public:
    explicit SdDPrint(SdRect over, SdRect sel, SdRect wnd, double ppm, QPrinter *printer, QWidget *parent = 0);
    ~SdDPrint();

    SdRect getWindow();

    int    getPrintArea();
  private:
    Ui::SdDPrint *ui;

    void   updateOrientation( bool horizontal );
    void   updatePrintArea( int area, SdRect ovr );
    double log2phys( int log );
    QString log2physStr( int log );
    int    phys2log( QString phys );
  };

#endif // SDDPRINT_H
