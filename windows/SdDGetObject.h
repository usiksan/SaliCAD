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

#include <QDialog>
#include <QSqlQueryModel>

class SdProjectItem;
class SdPItemSymbol;
class SdWEditorGraphView;

namespace Ui {
class SdDGetObject;
}

class SdDGetObject : public QDialog
  {
    Q_OBJECT

    SdWEditorGraphView    *mSymbolView;   //Widget for schematic preview
    SdWEditorGraphView    *mPartView;     //Widget for part preview
    SdPItemSymbol         *mComponent;    //Component if selected

    QString                mObjName;      //Object name
    QString                mObjAuthor;    //Object author
    QString                mObjId;        //Unical object id
    int                    mSectionIndex; //Section index

    quint64                mSort;         //Object select sort (class)

    static QSqlQueryModel *mModel;
  public:
    explicit SdDGetObject( quint64 sort, const QString title, QWidget *parent = 0);
    ~SdDGetObject();

    QString getObjName() const { return mObjName; }
    QString getObjAuthor() const { return mObjAuthor; }
    QString getObjId() const { return mObjId; }
    int     getSectionIndex() const { return mSectionIndex; }

  public slots:
    //Find button pressed
    void find();

    //On change selection item in find item table
    void onSelectItem( QModelIndex index );

    //On change segment selection
    void onCurrentSegment( int row );

    //Selected new category, apply filtr
    void onTagPath( const QString path );

  protected:
    void changeEvent(QEvent *e);

  private:
    Ui::SdDGetObject *ui;
  public:
    static bool           getObjectName( QString *name, QString *author, quint64 sort, const QString title, QWidget *parent );
    static SdProjectItem *getObject( quint64 sort, const QString title, QWidget *parent);
    static SdProjectItem *getComponent( int *logSectionPtr, quint64 sort, const QString title, QWidget *parent );

    // QDialog interface
  public slots:
    virtual void accept() override;
  };

#endif // SDDGETOBJECT_H
