/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  SdScaner - base scaner for text import

  Predefined tokens:
    'f' - for floats
    'd' - for integer
  Ordinary tokens (but may be others):
    'n' - for names
    's' - for strings
    Special chars like '(' represents itself.
*/
#ifndef SDSCANER_H
#define SDSCANER_H

#include <QString>

class SdScaner
  {
  protected:
    char    mToken;      //!< Current scanned token
    QString mLine;       //!< Input line
    int     mIndex;      //!< Current char index
    bool    mEndOfScan;  //!< Flag of scan completion
    QString mError;      //!< Error string
    QString mTokenValue; //!< Value of current token
  public:
    SdScaner();

    //!
    //! \brief lineIndex Return index of current line
    //! \return          Index of current line
    //!
    virtual int  lineIndex() const = 0;

    //!
    //! \brief nextLine Take next line from source
    //! \return         true if there is next line
    //!
    virtual bool nextLine() { return false; }

    //!
    //! \brief tokenNext Take next token. Must be redefined in child classes
    //!
    virtual void tokenNext() = 0;

    //!
    //! \brief error Append error string to error system
    //! \param msg   Error message
    //!
    virtual void error( const QString msg );

    //!
    //! \brief errorGet Get full error list as single string
    //! \return         Full error list as single string
    //!
    QString      errorGet() const { return mError; }

    //!
    //! \brief isError Checks if there is scaner error
    //! \return        true when there scaner error
    //!
    bool         isError() const { return !mError.isEmpty(); }

    //!
    //! \brief token Get last scanned token
    //! \return      Last scanned token
    //!
    char         token() const { return mToken; }

    //!
    //! \brief tokenNeed Check if last scanned token equal to tokenId and if not then append error with errorMsg
    //! \param tokenId   Needed token ident
    //! \param errorMsg  Error message which appended if tokenId not equal to last scanned token
    //! \return          true when tokenId equals to last scanned token
    //!
    bool         tokenNeed( char tokenId, const QString errorMsg = QString{} );

    //!
    //! \brief tokenNeedValue Check if last scanned token equals to tokenVal and return value itself
    //! \param tokenVal       Needed token ident
    //! \return               Value of token if last scanned token equals to tokenVal
    //!
    QString      tokenNeedValue(char tokenVal);

    //!
    //! \brief tokenNeedValue Check if last scanned token equals to tokenId and return value itself
    //! \param tokenId        Needed token ident
    //! \param value          Value of token if last scanned token equals to tokenId
    //! \param errorMsg       Error message which appended to error list if last scanned token not equals to tokenId
    //! \return               true when tokenId equals to last scanned token
    //!
    bool         tokenNeedValue( char tokenId, QString &value, const QString errorMsg );

    //!
    //! \brief tokenNeedValueInt Treat next token as integer
    //! \param tokenVal          Token id for integer
    //! \return                  Value of integer
    //!
    int          tokenNeedValueInt(char tokenVal) { return tokenNeedValue(tokenVal).toInt(); }

    //!
    //! \brief tokenNeedValueInt  Check if last scanned token equals to tokenId and return value itself as integer
    //! \param tokenId            Needed token ident
    //! \param value              Value of token if last scanned token equals to tokenId
    //! \param errorMsg           Error message which appended to error list if last scanned token not equals to tokenId
    //! \return                   true when tokenId equals to last scanned token and no error in conversion to integer
    //!
    bool         tokenNeedValueInt( char tokenId, int &value, const QString errorMsg );

    //!
    //! \brief tokenNeedValueFloat Treat next token as float
    //! \param tokenVal            Token id for float
    //! \return                    Value of float
    //!
    float        tokenNeedValueFloat(char tokenVal) { return tokenNeedValue(tokenVal).toFloat(); }

    //!
    //! \brief tokenNeedValueDouble Treat next token as double
    //! \return                     Value of double
    //!
    double       tokenNeedValueDouble() { return tokenNeedValue('f').toDouble(); }

    //!
    //! \brief tokenNeedValueFloat  Check if last scanned token equals to tokenId and return value itself as float
    //! \param tokenId              Needed token ident
    //! \param value                Value of token if last scanned token equals to tokenId
    //! \param errorMsg             Error message which appended to error list if last scanned token not equals to tokenId
    //! \return                     true when tokenId equals to last scanned token and no error in conversion to float
    //!
    bool         tokenNeedValueFloat( char tokenId, float &value, const QString errorMsg );

    //!
    //! \brief tokenValue Return last token value as string
    //! \return           Last token value as string
    //!
    QString      tokenValue() const { return mTokenValue; }

    //!
    //! \brief tokenValueInt Return last token value as int
    //! \return              Last token value as int
    //!
    int          tokenValueInt() const { return mTokenValue.toInt(); }

    //!
    //! \brief tokenValueFloat Return last token value as float
    //! \return                Last token value as float
    //!
    float        tokenValueFloat() const { return mTokenValue.toFloat(); }

    //!
    //! \brief tokenValueDouble Return last token value as double
    //! \return                 Last token value as double
    //!
    double       tokenValueDouble() const { return mTokenValue.toDouble(); }

    //!
    //! \brief matchTokenValue Checks if last scanned token equals to tokenId and token value equals to val. If both are true then take next token
    //! \param tokenId         Needed token ident
    //! \param val             Value of token to compare
    //! \return                true when last scanned token equals to tokenId and token value equals to val.
    //!
    bool         matchTokenValue(char tokenId, const QString &val );

    //!
    //! \brief matchToken Compares last scanned token to tokenId and if equals then take next token
    //! \param tokenId    Token ident to compare
    //! \return           true if last scanned token equals to tokenId
    //!
    bool         matchToken( char tokenId );

    //!
    //! \brief line Returns current scan line
    //! \return     Current scan line
    //!
    QString      line() const { return mLine; }

    //!
    //! \brief lineSet Sets new line to scan
    //! \param line    New line to scan
    //!
    void         lineSet( const QString &line );

    //!
    //! \brief blank Skeeps all characters act as spaces
    //!
    void         blank();

    //!
    //! \brief isEndOfScan Return status of endOfScan flag which indicate end of stream
    //! \return            Status of endOfScan flag which indicate end of stream
    //!
    bool         isEndOfScan() const { return mEndOfScan; }

    //!
    //! \brief isEndOfScanOrError Return status of stopping scan. Scan is stoped when error
    //!                           happens or end of stream reached
    //! \return                   Status of stopping scan
    //!
    bool         isEndOfScanOrError() const { return mEndOfScan || isError();}

    //!
    //! \brief scanName Scan name. Name consists of letters, digits and _ sign
    //!
    void         scanName();

    //!
    //! \brief scanDouble Scan double digit in common format -123.345e-24
    //! \param allowSign  Allow scan trailing sign
    //! \param useComma   Use comma as delimiter -123,345e-24
    //!
    void         scanDouble( bool allowSign, bool useComma );

    //!
    //! \brief scanInteger Scan integer digit in format -1234
    //! \param allowSign   Allow scan trailing sign
    //!
    void         scanInteger( bool allowSign );

    //!
    //! \brief scanString          Scan string closed with closeChar
    //! \param closeChar           Character used to close string
    //! \param guardChar           Character used as special guarding (\n)
    //! \param unclosedStringError Error line when string not closed to end of line
    //!
    void         scanString( QChar closeChar, QChar guardChar, const QString unclosedStringError );

    //!
    //! \brief skeepBlock Skeep block bounded with openToken and closeToken. Block may be nested
    //! \param openToken  Mark start of block
    //! \param closeToken Mark end of block
    //!
    void         skeepBlock( char openToken, char closeToken );
  };

#endif // SDSCANER_H
