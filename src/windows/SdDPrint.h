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

    SdRect          mOver;    //!< Over rect for print area
    SdRect          mSel;     //!< Over rect for selected elements
    SdRect          mWin;     //!< Over rect for selected window
    double          mPPM;     //!< PPM is how much phys in one logical
    QPrinter       *mPrinter; //!< Printer, page print to
  public:
    explicit SdDPrint(SdRect over, SdRect sel, SdRect wnd, double ppm, QPrinter *printer, QWidget *parent = nullptr);
    ~SdDPrint();

    SdRect getWindow();

    //Selected print area code
    int    getPrintArea();

    //Return current scale factor
    double getScaleFactor();

    //Return color print flag
    bool   isColor() const;

    //Return zero width line conversion width
    int    getZeroWidth() const;

    //!
    //! \brief isMirrorHorz Return current state of mirrored print
    //! \return             When true must be mirrored on horizontal axiz
    //!
    bool   isMirrorHorz() const;
  private:
    Ui::SdDPrint *ui;

    void   updateOrientation( bool horizontal );
    void   updatePrintArea( int area, SdRect ovr );
    void   updateScaleFactor();
  };

#endif // SDDPRINT_H
