/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
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
    explicit SdPulsar(QObject *parent = 0);

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
    //Activate item (bring item on top)
    void emitActivateItem( SdProjectItem *item );
    //Item contens changed
    void emitItemContentChanged( SdProjectItem *item );

    static SdPulsar *pulsar;
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
    //Item contens changed
    void itemContentChanged( SdProjectItem *item );

  public slots:
  };

#endif // SDPULSAR_H
