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
class SdPItemComponent;
class SdWEditorGraphView;
class SdWEditor3d;
class SdWCategoryList;
class SdProject;
class QDialogButtonBox;
class QTableWidget;
class QLineEdit;
class QListWidget;



class SdDGetObject : public QDialog
  {
    Q_OBJECT

    QSplitter             *mSplitCentral;  //!< Central splitter
    QSplitter             *mSplitFinder;   //!< Find table vertical splitter
    QSplitter             *mSplitView;     //!< Preview windows splitter
    QDialogButtonBox      *mDialogButtons; //!< Standard dialog buttons

    SdWEditorGraphView    *mSymbolView;   //Widget for schematic preview
    SdWEditorGraphView    *mPartView;     //Widget for part preview
    SdWEditor3d           *m3dView;         //!< 3d part view window
    SdPItemComponent      *mComponent;    //Component if selected
    SdProject             *mProject;      //Project if selected
    SdWCategoryList       *mCategoryList; //Hierarchy presentation of category tree
    QTableWidget          *mFieldsBox;      //!< Filter fields table
    QLineEdit             *mNameFilter;     //!< Name filter edit line
    QTableWidget          *mTable;          //!< Table with find results
    QListWidget           *mSections;       //!< Sections list

    static QString                mObjName;      //Object name
    static QString                mObjUid;       //Unical object id
    static QString                mCompUid;      //Unical object id for components
    static int                    mSectionIndex; //Section index
    static SdStringMap            mParam;        //Component or instance params

    static quint64                mSort;         //Object select sort (class)
    static SdLibraryHeaderList    mHeaderList;   //Header list for filtered objects
    static bool                   mExpandVariant;    //Flag for find only in titles
  public:
    explicit SdDGetObject(quint64 sort, const QString title, QWidget *parent = nullptr);
    ~SdDGetObject() override;

  public slots:
    //Find button pressed
    void find();

    //On change selection item in find item table
    void onSelectItem( int row, int column );

    //On change component section selection
    void onCurrentSection( int row );

    //When changed field filtr
    void onFieldChanged( int, int );

    //Clear all fields filtr
    void onClearFieldFiltr();

    //On category selected
    void onCategory( const QString str );

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
    static SdPItemComponent *getComponent( int *logSectionPtr, SdStringMap *param, const QString title, QWidget *parent );

    // QDialog interface
  public slots:
    virtual void accept() override;
  };

#endif // SDDGETOBJECT_H
