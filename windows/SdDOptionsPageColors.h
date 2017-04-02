/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#ifndef SDDOPTIONSPAGECOLORS_H
#define SDDOPTIONSPAGECOLORS_H

#include <QWidget>
#include <QTableWidget>

class SdDOptionsPageColors : public QWidget
  {
    Q_OBJECT

    QTableWidget *mTable; //Table with colors
  public:
    explicit SdDOptionsPageColors(QWidget *parent = 0);

  signals:

  public slots:

  private:
    void addColor( const QString descr, const QString key );
  };

#endif // SDDOPTIONSPAGECOLORS_H
