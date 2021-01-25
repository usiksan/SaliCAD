#ifndef SDSCANERMULTYLINE_H
#define SDSCANERMULTYLINE_H

#include "SdScaner.h"

#include <QStringList>
#include <QIODevice>

class SdScanerMultyline : public SdScaner
  {
    QStringList mLines;
    int         mLineIndex;
  public:
    SdScanerMultyline();

    void sourceSetStringList( QStringList lines );

    //!
    //! \brief sourceSetString Set source lines as single line delimited eoln symbols
    //! \param lines           Source lines as single line delimited eoln symbols
    //!
    void sourceSetString( QString lines );

    //!
    //! \brief read Read previously opened file contents to scaner
    //! \param file Previously opened file
    //! \return     true - if reading successfull
    //!
    bool read( QIODevice *file );

    //!
    //! \brief readFile Read file contents to scaner
    //! \param path     Full path to file
    //! \return         true - if reading successfull
    //!
    bool readFile( const QString &path );

    // SdScaner interface
  protected:
    virtual int  lineIndex() const override { return mLineIndex - 1; }

    virtual bool nextLine() override;
  };

#endif // SDSCANERMULTYLINE_H
