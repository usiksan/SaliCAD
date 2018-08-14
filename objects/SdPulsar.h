/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Object to send messages to visual elements
*/

#ifndef SDPULSAR_H
#define SDPULSAR_H

#include <QObject>

class SdProject;
class SdProjectItem;

class SdPulsar : public QObject
  {
    Q_OBJECT
  public:
    explicit SdPulsar(QObject *parent = nullptr);

    //Send signals
    //BEFORE destroy project
    void emitCloseProject( SdProject *project );
    //AFTER rename project
    void emitRenameProject( SdProject *project );
    //AFTER rename item
    void emitRenameItem( SdProjectItem *item );
    //AFTER insert item
    void emitInsertItem( SdProjectItem *item );
    //BEFORE remove item
    void emitRemoveItem( SdProjectItem *item );
    //Activate item (bring item on top) with edit status
    void emitActivateItem(SdProjectItem *item);
    //Need close item edit (view)
    void emitCloseEditView( SdProjectItem *item );

    //Status bar
    void emitSetStatusLabels( const QString xlabel, const QString ylabel );
    void emitSetStatusPositions( const QString x, const QString y );
    void emitSetStatusMessage( const QString msg );

    //Prop bar
    //Emit signal on viewed layers changed. It emited when layer dialog completed
    void emitViewedLayers();

    static SdPulsar *sdPulsar;
  signals:
    //BEFORE destroy project
    void closeProject( SdProject *project );
    //AFTER rename project
    void renameProject( SdProject *project );
    //AFTER rename item
    void renameItem( SdProjectItem *item );
    //AFTER insert item
    void insertItem( SdProjectItem *item );
    //BEFORE remove item
    void removeItem( SdProjectItem *item );
    //Activate item (bring item on top)
    void activateItem( SdProjectItem *item );
    //Need close item edit (view)
    void closeEditView( SdProjectItem *item );

    //Status bar
    void setStatusLabels( const QString xlabel, const QString ylabel );
    void setStatusPositions( const QString x, const QString y );
    void setStatusMessage( const QString msg );

    //Prop bar
    //Emit signal on viewed layers changed. It emited when layer dialog completed
    void viewedLayers();

  public slots:
  };

#endif // SDPULSAR_H
