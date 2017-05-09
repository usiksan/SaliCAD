/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#ifndef SDDGETOBJECT_H
#define SDDGETOBJECT_H

#include <QDialog>
#include <QSqlQueryModel>

class SdProjectItem;
class SdWEditorGraphSymbol;
class SdWEditorGraphPart;

namespace Ui {
class SdDGetObject;
}

class SdDGetObject : public QDialog
  {
    Q_OBJECT

    SdWEditorGraphSymbol *mSymbolView;
    SdWEditorGraphPart   *mPartView;

    QString mObjName;
    QString mObjAuthor;
    QString mObjId;

    quint64 mSort;       //Object select sort (class)

    static QSqlQueryModel *mModel;
  public:
    explicit SdDGetObject( quint64 sort, const QString title, QWidget *parent = 0);
    ~SdDGetObject();

    QString getObjName() const { return mObjName; }
    QString getObjAuthor() const { return mObjAuthor; }
    QString getObjId() const { return mObjId; }

  public slots:
    void find();
    void onSelectItem( QModelIndex index );

  protected:
    void changeEvent(QEvent *e);

  private:
    Ui::SdDGetObject *ui;
  public:
    static bool           getObjectName( QString *name, QString *author, quint64 sort, const QString title, QWidget *parent );
    static SdProjectItem *getObject( quint64 sort, const QString title, QWidget *parent);

    // QDialog interface
  public slots:
    virtual void accept() override;
  };

#endif // SDDGETOBJECT_H
