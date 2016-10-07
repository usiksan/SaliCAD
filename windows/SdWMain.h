/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  SdWMain - Main application window
*/

#ifndef SBWMAIN_H
#define SBWMAIN_H

#include "SdConfig.h"
#include "SdWProjectList.h"
#include "SdWEditor.h"
#include <QMainWindow>
#include <QSplitter>
#include <QTabWidget>
#include <QStringList>

class SdProjectItem;

class SdWMain : public QMainWindow
  {
    Q_OBJECT

    QSplitter      *mWSplitter;     //Central part of programm windows - is splitter with projects and redactors
    SdWProjectList *mWProjectList;  //Project list
    QTabWidget     *mWEditors;      //Actived visual editors
  public:
    explicit SdWMain( QStringList args, QWidget *parent = 0 );

  signals:

  public slots:
    void activateProjectName( const QString name, bool dirty );

    void activateProjectItem( SdProjectItem *item );

    // QWidget interface
  protected:
    virtual void closeEvent(QCloseEvent *ev) override;

  private:
    SdWEditor* getEditor( int index );
    SdWEditor* getCurrent();
  };

#endif // SBWMAIN_H
