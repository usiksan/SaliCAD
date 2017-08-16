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

#include <QDialog>
#include <QtPrintSupport/QPrinter>

namespace Ui {
    class SdDPrint;
  }

class SdDPrint : public QDialog
  {
    Q_OBJECT

    QPrinter *mPrinter; //Printer, page print to
  public:
    explicit SdDPrint( QPrinter *printer, QWidget *parent = 0);
    ~SdDPrint();

  private:
    Ui::SdDPrint *ui;

    void updateOrientation( bool horizontal );
  };

#endif // SDDPRINT_H
