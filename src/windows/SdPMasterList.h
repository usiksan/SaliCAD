/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Listing of masters for export and import objects.

  Show list masters which can export or import object.
*/
#ifndef SDPMASTERLIST_H
#define SDPMASTERLIST_H

#include <QWizardPage>
#include <QListWidget>
#include <QTextEdit>
#include <QLabel>
#include <QList>

struct SdMasterInfo {
    QString mMasterName;        //Name of master
    QString mMasterDescription; //Short description of master
    int     mMasterStep;        //Id of master start
    QString mImageSrc;          //Source image for master icon
  };



class SdPMasterList : public QWizardPage
  {
    Q_OBJECT

    QListWidget        *mMasterType;     //Visual list with master types
    QLabel             *mImage;
    QTextEdit          *mDescription;    //Visual description of selected master
    QList<SdMasterInfo> mInfoList;
  public:
    SdPMasterList(const QString atitle, const QString asubTitle, QWidget *parent = nullptr );

    //Add new master to list
    void addMaster( const QString name, const QString descr, int stp, const QString img );

    // QWizardPage interface
  public:
    virtual void initializePage() override;
    virtual bool validatePage() override;
    virtual int  nextId() const override;
  };

#endif // SDPMASTERLIST_H
