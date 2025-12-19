#ifndef SDDOFFSET_H
#define SDDOFFSET_H

#include <QDialog>
#include <QComboBox>
#include <QPointF>

class SdDOffset : public QDialog
  {
    Q_OBJECT

    QComboBox *mBoxOffsetX;
    QComboBox *mBoxOffsetY;
    QPointF    mOffset;
  public:
    SdDOffset( const QString &key, QWidget *parent );


    QPointF offset() const { return mOffset; }

    // QDialog interface
  public slots:
    virtual void accept() override;

  private:
    void replaceBoxX( const QString &str );
    void replaceBoxY( const QString &str );
  };

#endif // SDDOFFSET_H
