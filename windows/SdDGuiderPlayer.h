/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SDDGUIDERPLAYER_H
#define SDDGUIDERPLAYER_H

#include <QDialog>

namespace Ui {
  class SdDGuiderPlayer;
}

class SdDGuiderPlayer : public QDialog
  {
    Q_OBJECT

  public:
    explicit SdDGuiderPlayer(QWidget *parent = nullptr);
    ~SdDGuiderPlayer();

  private:
    Ui::SdDGuiderPlayer *ui;
  };

#endif // SDDGUIDERPLAYER_H
