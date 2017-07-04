/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  SdWLabel - Replacement for QLabel, because it minimizing main window
             it being inserted in QStatusBar
*/
#include "SdWLabel.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QResizeEvent>

SdWLabel::SdWLabel(const QString text, int minWidth, QWidget *parent) :
  QWidget(parent)
  {
  mLabel = new QLabel( this );
  mLabel->setText( text );
  mLabel->setMinimumWidth( minWidth );
  setMinimumWidth( minWidth );
  }

void SdWLabel::setText(const QString text)
  {
  mLabel->setText( text );
  }




void SdWLabel::resizeEvent(QResizeEvent *event)
  {
  mLabel->resize( event->size() );
  QWidget::resizeEvent( event );
  }



void SdWLabel::mousePressEvent(QMouseEvent *event)
  {
  QWidget::mousePressEvent(event);
  emit trigger();
  }
