/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SDWSECTION_H
#define SDWSECTION_H

#include "objects/SdSection.h"
#include <QWidget>
#include <QLabel>
#include <QTableWidget>

class SdWSection : public QWidget
  {
    Q_OBJECT

    SdSection    *mSection;  //Symbol section
    QLabel       *mTitle;    //Section symbol name
    QTableWidget *mPinTable; //Section pin assotiation table
  public:
    explicit SdWSection( bool editEnabled, SdSection *s, QWidget *parent = 0);

    SdSection *getSection() const { return mSection; }
  signals:

  public slots:
    //Update visual title as in section
    void updateTitle();

    //Update visual pin assotiation table
    void updatePinTable(bool editEnabled );

    //On pin edit finish
    void onPinEditFinish( int row, int column );
  };

#endif // SDWSECTION_H
