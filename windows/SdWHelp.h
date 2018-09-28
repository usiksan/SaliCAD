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
    SdWHelp( QWidget *parent = nullptr );


    //Path where resides help system files
    static QString helpPath();
    static void    setHelpPath( const QString pth );

    //Conversion page name with fragment point in page to URL with current language
    //Преобразование названия страницы в фактический URL в соответствии с выбранным языком
    static QUrl    pageConvert( const QString &page , const QString &fragment);

    //Return page with reference error
    //Вернуть страницу помощи с ошибкой ссылки
    static QUrl    pageError();

  signals:

  public slots:
    //Show help contens
    void contens();

    //Show help topic
    void helpTopic( const QString topic );

    //Show intro topic
    void helpIntro( SdWMain *main );

  private:

    //Path where resides help system files
    static QString mHelpPath;
  };

#endif // SDWHELP_H
