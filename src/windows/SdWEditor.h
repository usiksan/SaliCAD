/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Common base class for any editors.
*/

#ifndef SDWEDITOR_H
#define SDWEDITOR_H

#include "SdConfig.h"
#include "objects/SdPoint.h"
#include "objects/SdRect.h"
#include "objects/SdScaler.h"
#include <QAbstractScrollArea>
#include <QString>

class SdProjectItem;
class SdProject;

class SdWEditor : public QAbstractScrollArea
  {
    Q_OBJECT

    int mRecentlyIndex; //Index for release last resently used mechanism
  public:
    explicit SdWEditor( QWidget *parent = nullptr);
    virtual ~SdWEditor() {}

    //Return item which edited by this editor
    virtual SdProjectItem* getProjectItem() const = 0;

    //Return project to which referenced item edited by this editor
    virtual SdProject*     getProject();

    void                   dirtyProject();

    virtual QString        getTitle();

    virtual QString        getToolTip();

    virtual QString        getIconName();

    int                    getRecentlyIndex() const { return mRecentlyIndex; }

    void                   updateRecentlyIndex( bool dec ) { mRecentlyIndex = dec ? qMax(0,mRecentlyIndex-1) : SD_MAX_EDITORS; }

    //Update status of undo and redo
    void                   updateUndoRedoStatus();

    //On activate editor window
    virtual void onActivateEditor();

    //Command on rections [Реакции на команды]
    virtual void cmFilePrint() {}

    virtual void cmFileImport() {}
    virtual void cmFileExport() {}

    virtual void cmObjectEditEnable();
    virtual void cmObjectEditDisable();

    virtual void cmEditUndo() {}
    virtual void cmEditRedo() {}
    virtual void cmEditCut() {}
    virtual void cmEditCopy() {}
    virtual void cmEditPaste() {}
    virtual void cmEditDelete() {}
    virtual void cmEditSelectAll() {}
    virtual void cmEditUnSelect() {}
    virtual void cmEditFind() {}
    virtual void cmEditReplace() {}
    virtual void cmEditProperties() {}

    virtual void cmViewProject() {}
    virtual void cmViewFit() {}
    virtual void cmViewNets() {}
    virtual void cmViewGrid() {}
    virtual void cmViewLayers() {}
    virtual void cmViewZoomIn() {}
    virtual void cmViewZoomOut() {}
    virtual void cmViewWindow() {}
    virtual void cmViewMeasurement() {}

    virtual void cmModeSelect() {}

    virtual void cmModeLine() {}
    virtual void cmModeRect() {}
    virtual void cmModeFilledRect() {}
    virtual void cmModeRegion() {}
    virtual void cmModeFilledRegion() {}
    virtual void cmModeCircle() {}
    virtual void cmModeFilledCircle() {}
    virtual void cmModeArc() {}
    virtual void cmModeText() {}
    virtual void cmModeField() {}

    //3d
    virtual void cmBall() {}
    virtual void cmPinWired() {}
    virtual void cmPinFlat() {}
    virtual void cmBodyCylinder() {}
    virtual void cmBodyBrick() {}
    virtual void cmBodyContur() {}

    //Symbol Part
    virtual void cmModePin() {}
    virtual void cmModeReference() {}
    virtual void cmModeOrigin() {}
    virtual void cmModeValue() {}

    //Sheet and pcb
    virtual void cmRenumeration() {}
    virtual void cmModeReferenceMove() {}
    virtual void cmModeValueMove() {}
    virtual void cmModeComponent() {}

    //Sheet
    virtual void cmModeFragment() {}
    virtual void cmModeNet() {}
    virtual void cmNetSetup() {}
    virtual void cmModeBus() {}
    virtual void cmModeDisconnect() {}
    virtual void cmModePcbArea() {}
    virtual void cmModeLink() {}
    virtual void cmModeNetName() {}
    virtual void cmModeNetList() {}
    virtual void cmModePack() {}
    virtual void cmModeBrowse( SdProjectItem* ) {}

    //Pcb
    virtual void cmPads() {}
    virtual void cmModeLineSize() {}
    virtual void cmModeRadiusSize() {}
    virtual void cmModeMovePart() {}
    virtual void cmModePartSelect( QStringList ) {}

    virtual void cmModeEditWire() {}
    virtual void cmModeRoadEnter() {}
    virtual void cmModePolygon() {}
    virtual void cmModeDeleteWire() {}
    virtual void cmModePad() {}
    virtual void cmModeViaEnter() {}

    //Pcb rules
    virtual void cmRulesEdit() {}
    virtual void cmRulesCheck() {}
    virtual void cmRulesErrorNext() {}

    //All
    virtual void cmClipboardChange() {}

    virtual void cmPropChanged() {}

    virtual void cmEnterPosition() {}

  signals:

  public slots:

//  protected:
//    //Show dialog with error msg
//    QMessageBox::StandardButton msgError( QString msg, QMessageBox::StandardButton buttons = QMessageBox::Ok );
//    //Show dialog with warning msg
//    QMessageBox::StandardButton msgWarning( QString msg, QMessageBox::StandardButton buttons = QMessageBox::Ok );
//    //Show dialog with info msg
//    QMessageBox::StandardButton msgInfo( QString msg, QMessageBox::StandardButton buttons = QMessageBox::Ok );
  };

#endif // SDWEDITOR_H