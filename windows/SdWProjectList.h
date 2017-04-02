/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#ifndef SDWPROJECTLIST_H
#define SDWPROJECTLIST_H

#include "objects/SdProject.h"
#include <QWidget>
#include <QList>
#include <QStackedWidget>
#include <QComboBox>
#include <QToolButton>

class SdWProjectTree;

class SdWProjectList : public QWidget
  {
    Q_OBJECT

    QStackedWidget   *mWProjectStack; //Изображение деревьев открытых проектов
    QComboBox        *mProjectTitles; //Список открытых проектов
    QToolButton      *mCloseProject;  //Кнопка закрытия активного проекта
  public:
    explicit SdWProjectList(QWidget *parent = 0);

    bool isEmpty() { return mWProjectStack->count() == 0; }

    SdWProjectTree* activeProject();
    SdWProjectTree* project( int index );

  signals:
    //Name of active project changed or other project selected
    void projectNameChanged( const QString name, bool dirty );

  public slots:
    //Close all projects
    void fileCloseAll();

    //File project open
    void fileOpen( const QString fname );

    //Active file close
    bool cmFileClose();

    //On project index activated
    void onProjectActivated( int index );

    //On rename project
    void onRenameProject( SdProject *prj );
  };

#endif // SDWPROJECTLIST_H
