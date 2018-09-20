/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SDGUIDERFILE_H
#define SDGUIDERFILE_H

#include <QImage>
#include <QList>
#include <QMap>
#include <QByteArray>
#include <functional>



struct SdGuiderTime {
    QByteArray mData;
    qint32     mTime;
    qint8      mType;

    QString title() const;
  };


struct SdGuiderTiter {
    QPoint                mPos;
    QMap<QString,QString> mContens;

    QByteArray write() const;
    void       read( const QByteArray &src );
  };


#define SD_GT_KEY_IMAGE  1
#define SD_GT_DIF_IMAGE  2
#define SD_GT_STOP       3
#define SD_GT_MOUSE      4
#define SD_GT_TITER      5
#define SD_GT_TITER_HIDE 6


typedef std::function<void( const QString &str)> SdSayFunc;

struct SdGuiderFile
  {
    QList<SdGuiderTime> mFile;
    //Playback-record status
    QImage              mBack;      //Current background
    //QImage              mCursor;    //Current cursor
    quint32             mMouseButtons;
    QPoint              mCursorPos; //Current cursor position
    QString             mTitle;     //Current title if present
    QString             mLanguage;  //Language id
    SdSayFunc           mSay;       //Say func

    SdGuiderFile();

    void    addImage( QImage img, int time );

    void    addCursor( quint32 buttons, QPoint cursorPos, int time );

    void    addStop( int time );

    void    play( int index );

    QPixmap build();

    void    save( const QString fname );

    void    load( const QString fname );
  };

#endif // SDGUIDERFILE_H
