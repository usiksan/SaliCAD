/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SDDLAYERCREATE_H
#define SDDLAYERCREATE_H

#include <QDialog>

namespace Ui {
  class SdDLayerCreate;
}

class SdDLayerCreate : public QDialog
  {
    Q_OBJECT

  public:
    explicit SdDLayerCreate(QWidget *parent = nullptr);
    ~SdDLayerCreate() override;

    QString   layerId() const;

    int       layerStratum() const;

  public slots:
    //On layer constructed
    void onLayerIdConstructed( int );

  private:
    Ui::SdDLayerCreate *ui;

    // QDialog interface
  public slots:
    virtual void accept() override;
  };

#endif // SDDLAYERCREATE_H
