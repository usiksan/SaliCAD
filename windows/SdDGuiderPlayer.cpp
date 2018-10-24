/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdDGuiderPlayer.h"
#include "ui_SdDGuiderPlayer.h"

SdDGuiderPlayer::SdDGuiderPlayer(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::SdDGuiderPlayer)
  {
  ui->setupUi(this);
  }

SdDGuiderPlayer::~SdDGuiderPlayer()
  {
  delete ui;
  }
