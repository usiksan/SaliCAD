/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Dialog with help page. Used to display help to dialogs.
*/
#ifndef SDDHELP_H
#define SDDHELP_H

#include <QDialog>

class SdDHelp : public QDialog
  {
    Q_OBJECT

  public:
    SdDHelp( const QString &helpPage, QWidget *parent );

    static void help( const QString &helpPage, QWidget *parent );
  };

#endif // SDDHELP_H
