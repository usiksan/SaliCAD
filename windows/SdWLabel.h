/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  SdWLabel - Replacement for QLabel, because it minimizing main window
             it being inserted in QStatusBar
*/
#ifndef SDWLABEL_H
#define SDWLABEL_H

#include <QWidget>
#include <QLabel>

class SdWLabel : public QWidget
  {
    Q_OBJECT

    QLabel *mLabel;
  public:
    explicit SdWLabel( const QString text, int minWidth, QWidget *parent = 0);

    void setText( const QString text );

    // QWidget interface
  protected:
    virtual void resizeEvent(QResizeEvent *event) override;
  };

#endif // SDWLABEL_H
