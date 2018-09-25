/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Allows select layer pair. Show full layer list
*/
#ifndef SDDLAYERLIST_H
#define SDDLAYERLIST_H

#include <QDialog>
#include <QStringList>

namespace Ui {
  class SdDLayerList;
}

#define SdDLayerList_cancel 0
#define SdDLayerList_remove 1
#define SdDLayerList_assign 2

class SdDLayerList : public QDialog
  {
    Q_OBJECT

    QStringList mLayerIdList; //List of layers id's
  public:
    explicit SdDLayerList( const QString currentPair, QWidget *parent = nullptr);
    ~SdDLayerList();

    //Returned selected layer pair
    QString pair() const;

  public slots:
    void cmCancel();
    void cmRemove();
    void cmAssign();

  private:
    Ui::SdDLayerList *ui;
  };

#endif // SDDLAYERLIST_H
