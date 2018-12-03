/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Dialog for selection from list of string
*/
#ifndef SDDSTRINGFROMLIST_H
#define SDDSTRINGFROMLIST_H

#include <QDialog>
#include <QStringList>
#include <functional>

namespace Ui {
  class SdDStringFromList;
}


class SdDStringFromList : public QDialog
  {
    Q_OBJECT

    std::function<QString( QString key )> mGetDescription;
  public:
    explicit SdDStringFromList( const QStringList &lst, std::function<QString( QString key )> fun, QWidget *parent = nullptr);
    ~SdDStringFromList();

    //Result of selection
    QString result() const;
  private:
    Ui::SdDStringFromList *ui;
  };

#endif // SDDSTRINGFROMLIST_H
