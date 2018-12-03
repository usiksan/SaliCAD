/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Layers maintence dialog

  Allow switch layer visible, editing. Selecting layer color and assign name.
*/
#ifndef SDDLAYERS_H
#define SDDLAYERS_H

#include "objects/SdProject.h"

#include <QDialog>
#include <QSet>
#include <QTableWidgetItem>

class SdLayer;

namespace Ui {
  class SdDLayers;
}

class SdDLayers : public QDialog
  {
    Q_OBJECT

    SdProject    *mProject;     //Active project, on witch layers dialog called
    QStringList   mList;        //List of id's edited layers
    QSet<QString> mActualList;  //List of id's minimum layer set

    static int mShow;    //Define show all, actual or used layers
  public:
    explicit SdDLayers( SdProject *prj, QWidget *parent = nullptr);
    ~SdDLayers();

    //Stratum count. By default stratum count disabled. When set stratum count it is enabled
    void setStratumCount( int c );
    int  getStratumCount() const;

    //Load layer list (enable-disable status)
    static void loadLayerList( const QString fname );
  private:
    Ui::SdDLayers *ui;

    void fillLayerList();
    void appendLyaerToVisualList( const QString id );

  public slots:
    void cmHelp();
    void cmCreate();
    void cmDelete();
    void cmShowAll();
    void cmShowActual();
    void cmShowUsed();
    void cmHideAll();
    void cmSwitchAll();
    void cmLoad();
    void cmSave();

    void onCellClicked( int row, int column );

    void onItemChanged( QTableWidgetItem *item );

  private:
    //Return textual representation of layer state
    QString layerState( SdLayer *layer );

    //Return textual representation of layer trace
    QString layerTrace( SdLayer *layer );
  };

#endif // SDDLAYERS_H
