/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SDGUIDERPLAYER_H
#define SDGUIDERPLAYER_H

#include <QWidget>
#include <QTimer>
#include <QImage>

class SdGuiderPlayer : public QWidget
  {
    Q_OBJECT

    QTimer  mTimer;     //Timer for interval event generating
    qint32  mTime;      //Current play moment
  public:
    explicit SdGuiderPlayer(QWidget *parent = nullptr);


  signals:

  public slots:
    void onTime();

    void play( const QString &fname );

    // QWidget interface
  protected:
    virtual void paintEvent(QPaintEvent *event) override;
  };

#endif // SDGUIDERPLAYER_H
