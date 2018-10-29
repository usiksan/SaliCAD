/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Some common params:
    - interface language
*/
#ifndef SDDOPTIONSPAGECOMMON_H
#define SDDOPTIONSPAGECOMMON_H

#include <QWidget>
#include <QComboBox>

class SdDOptionsPageCommon : public QWidget
  {
    Q_OBJECT

    QComboBox *mLanguage;
  public:
    explicit SdDOptionsPageCommon(QWidget *parent = nullptr);

  signals:

  public slots:
    void accept();
  };

#endif // SDDOPTIONSPAGECOMMON_H
