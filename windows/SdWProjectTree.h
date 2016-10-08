/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#ifndef SDWPROJECTTREE_H
#define SDWPROJECTTREE_H

#include <QWidget>

class SdWProjectTree : public QWidget
  {
    Q_OBJECT
  public:
    explicit SdWProjectTree( const QString fname, QWidget *parent = 0);

  signals:

  public slots:
    virtual bool cmFileClose() { return true; }

    virtual void cmFileSave() {}
    virtual void cmFileSaveAs() {}

    virtual void cmObjectNew();
    virtual void cmObjectRename() {}
    virtual void cmObjectDelete() {}
    virtual void cmObjectCopy() {}
    virtual void cmObjectPaste() {}
    virtual void cmObjectCut() {}
    virtual void cmObjectSort() {}

    void cmClipboardChange() {}
  };

#endif // SDWPROJECTTREE_H
