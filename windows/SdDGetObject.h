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

class SdProjectItem;

namespace Ui {
class SdDGetObject;
}

class SdDGetObject : public QDialog
  {
    Q_OBJECT

    QString mObjName;
    QString mObjAuthor;
    QString mObjId;

    quint64 mSort;       //Object select sort (class)
  public:
    explicit SdDGetObject( quint64 sort, const QString title, QWidget *parent = 0);
    ~SdDGetObject();

  protected:
    void changeEvent(QEvent *e);

  private:
    Ui::SdDGetObject *ui;
  public:
    static bool           getObjectName( QString *name, QString *author, quint64 sort, const QString title, QWidget *parent );
    static SdProjectItem *getObject( quint64 sort, const QString title, QWidget *parent);
  };

#endif // SDDGETOBJECT_H
