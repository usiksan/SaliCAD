/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  SdScanerMultyline - Append to scaner function to scan from multyline source, such as files
*/
#ifndef SDSCANERMULTYLINE_H
#define SDSCANERMULTYLINE_H

#include "SdScaner.h"

#include <QStringList>
#include <QIODevice>

class SdScanerMultyline : public SdScaner
  {
    QStringList mLines;     //!< Source line list
    int         mLineIndex; //!< Index of next line to analis
  public:
    SdScanerMultyline();

    //!
    //! \brief sourceSetStringList Set source lines as ready-made line list
    //! \param lines               Ready-made line list
    //!
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
    //!
    //! \brief lineIndex Return index of current line
    //! \return          Index of current line
    //!
    virtual int  lineIndex() const override { return mLineIndex - 1; }

    //!
    //! \brief nextLine Take next line from source
    //! \return         true if there is next line
    //!
    virtual bool nextLine() override;
  };

#endif // SDSCANERMULTYLINE_H
