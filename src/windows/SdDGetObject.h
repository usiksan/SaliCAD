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
#include <QSplitter>

class SdObject;
class SdProjectItem;
class SdPItemVariant;
class SdWEditorGraphView;
class SdWEditor3d;
class SdProject;
class QDialogButtonBox;
class QTableWidget;
class QLineEdit;
class QListWidget;


class SdDGetObject : public QDialog
  {
    Q_OBJECT

    QSplitter             *mSplitFinder;    //!< Find table vertical splitter
    QSplitter             *mSplitView;      //!< Preview windows splitter
    QDialogButtonBox      *mDialogButtons;  //!< Standard dialog buttons

    SdWEditorGraphView    *mSymbolView;     //!< Widget for schematic preview
    SdWEditorGraphView    *mPartView;       //!< Widget for part preview
    SdWEditor3d           *m3dView;         //!< 3d part view window
    SdPItemVariant        *mComponent;      //!< Component if selected
    SdProject             *mProject;        //!< Project if selected
    QLineEdit             *mNameFilter;     //!< Name filter edit line
    QTableWidget          *mTable;          //!< Table with find results
    QListWidget           *mSections;       //!< Sections list

    static QString                mObjName;       //!< Object name
    static QString                mObjUid;        //!< Unical object id
    static QString                mItemName;      //!< Item name inside project
    static QString                mCompUid;       //!< Unical object id for components
    static int                    mSectionIndex;  //!< Section index
    static SdStringMap            mParam;         //!< Component or instance params

    static quint64                mSort;          //!< Object select sort (class)
    static SdLibraryHeaderList    mHeaderList;    //!< Header list for filtered objects
    static bool                   mExpandVariant; //!< Flag for find only in titles
    static bool                   mWithPartOnly;  //!< Check box with flag which filter only components with part
  public:
    explicit SdDGetObject(quint64 sort, const QString title, const QString &defFilter = QString{}, QWidget *parent = nullptr);

  public slots:
    //Find button pressed
    void find();

    //On change selection item in find item table
    void onSelectItem( int row, int column );

    //On change component section selection
    void onCurrentSection( int row );

  protected:
    void changeEvent(QEvent *e) override;

  private:
    void clearComponent();
    void clearProject();

    //Fill visual table with mHeaderList contents
    void fillTable();

  public:
    static SdObject         *getObject(quint64 sort, const QString title, QWidget *parent, const QString defFiltr = QString{} );
    static QString           getObjectUid( quint64 sort, const QString title, QWidget *parent, const QString defFiltr = QString() );
    static SdPItemVariant   *getComponent( int *logSectionPtr, SdStringMap *param, const QString title, QWidget *parent );
    static SdProject        *getProject( QString &itemName, const QString title, QWidget *parent, const QString defFiltr = QString{} );

    // QDialog interface
  public slots:
    virtual void accept() override;
  };

#endif // SDDGETOBJECT_H
