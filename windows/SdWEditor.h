/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#ifndef SDWEDITOR_H
#define SDWEDITOR_H


#include <QWidget>
#include <QString>

class SdProjectItem;

class SdWEditor : public QWidget
  {
    Q_OBJECT
  public:
    explicit SdWEditor(QWidget *parent = 0);

    virtual SdProjectItem* getProjectItem() = 0;

    QString                getTitle();

    QString                getIconName();

  signals:

  public slots:
  };

#endif // SDWEDITOR_H
