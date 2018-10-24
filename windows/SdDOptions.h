/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Dialog with tab pages of options
*/

#ifndef SDDOPTIONS_H
#define SDDOPTIONS_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QTabWidget>

class SdDOptions : public QDialog
  {
    Q_OBJECT

    QTabWidget       *mTabWidget;
    QDialogButtonBox *mButtons;
  public:
    explicit SdDOptions( QWidget *parent = nullptr );
    ~SdDOptions();

  };

#endif // SDDOPTIONS_H
