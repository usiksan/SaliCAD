/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Widget to display help
*/
#ifndef SDWHELP_H
#define SDWHELP_H

#include <QTextBrowser>

class SdWMain;

class SdWHelp : public QTextBrowser
  {
    Q_OBJECT

    SdWMain *mMain; //Main window pointer
  public:
    //Common constructor for help widgets
    SdWHelp();
    //Constructor for intro help page widget
    SdWHelp( SdWMain *main );


    //Path where resides help system files
    static QString helpPath();

    //Conversion page name with fragment point in page to URL with current language
    //Преобразование названия страницы в фактический URL в соответствии с выбранным языком
    static QUrl    pageConvert( const QString &page , const QString &fragment);

    //Return page with reference error
    //Вернуть страницу помощи с ошибкой ссылки
    static QUrl    pageError();

  signals:

  public slots:
    //Show help contents
    void contents();

    //Show help topic
    void helpTopic( const QString topic );

  private:

    //Show intro topic
    void helpIntro();
  };

#endif // SDWHELP_H
