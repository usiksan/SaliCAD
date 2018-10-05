/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Widget to display and navigate on category hierarchy
*/
#ifndef SDWCATEGORYLIST_H
#define SDWCATEGORYLIST_H

#include <QListWidget>

class SdWCategoryList : public QListWidget
  {
    Q_OBJECT
  public:
    SdWCategoryList( QWidget *parent );

  private:
    //Fill list with contens of current category path
    void fill();
  };

#endif // SDWCATEGORYLIST_H
