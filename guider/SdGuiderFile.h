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


struct SdGuiderTime {
    QByteArray mData;
    qint32     mTime;
    qint8      mType;

    QString title() const;
  };

#define SD_GT_KEY_IMAGE 1
#define SD_GT_DIF_IMAGE 2
#define SD_GT_STOP      3
#define SD_GT_MOUSE     4

struct SdGuiderFile
  {
    QList<SdGuiderTime> mFile;
    //Playback-record status
    QImage              mBack;      //Current background
    //QImage              mCursor;    //Current cursor
    quint32             mMouseButtons;
    QPoint              mCursorPos; //Current cursor position
    QString             mTitle;     //Current title if present
    QPoint              mTitlePos;  //Current title position

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
