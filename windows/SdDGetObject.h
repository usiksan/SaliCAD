/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Dialog for retrive object from dataBase
*/

#ifndef SDDGETOBJECT_H
#define SDDGETOBJECT_H

#include "library/SdLibraryHeader.h"

#include <QDialog>

class SdObject;
class SdProjectItem;
class SdPItemComponent;
class SdWEditorGraphView;

namespace Ui {
class SdDGetObject;
}



class SdDGetObject : public QDialog
  {
    Q_OBJECT

    SdWEditorGraphView    *mSymbolView;   //Widget for schematic preview
    SdWEditorGraphView    *mPartView;     //Widget for part preview
    SdPItemComponent      *mComponent;    //Component if selected

    QString                mObjName;      //Object name
    QString                mObjAuthor;    //Object author
    QString                mObjUid;       //Unical object id
    int                    mSectionIndex; //Section index
    SdStringMap            mParam;        //Component or instance params

    quint64                mSort;         //Object select sort (class)
    SdLibraryHeaderList    mHeaderList;
  public:
    explicit SdDGetObject( quint64 sort, const QString title, QWidget *parent = nullptr);
    ~SdDGetObject() override;

    QString     getObjName() const { return mObjName; }
    QString     getObjAuthor() const { return mObjAuthor; }
    QString     getObjUid() const { return mObjUid; }
    int         getSectionIndex() const { return mSectionIndex; }
    SdStringMap getParams() const { return mParam; }

  public slots:
    //Find button pressed
    void find();

    //On change selection item in find item table
    void onSelectItem( int row, int column );

    //On change component section selection
    void onCurrentSection( int row );

    //Selected new category, apply filtr
    void onTagPath( const QString path );

    //Pressed button "Load from central repository"
    void onLoadFromCentral();

  protected:
    void changeEvent(QEvent *e) override;

  private:
    Ui::SdDGetObject *ui;
    void clearComponent();

    //Fill visual table with mHeaderList contens
    void fillTable();

  public:
    static bool              getObjectName( QString *name, QString *author, quint64 sort, const QString title, QWidget *parent );
    static SdObject         *getObject( quint64 sort, const QString title, QWidget *parent);
    static QString           getObjectUid( quint64 sort, const QString title, QWidget *parent );
    static SdPItemComponent *getComponent( int *logSectionPtr, SdStringMap *param, const QString title, QWidget *parent );

    // QDialog interface
  public slots:
    virtual void accept() override;
  };

#endif // SDDGETOBJECT_H
