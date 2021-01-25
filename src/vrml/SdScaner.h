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
    QString mTokenValue; //!< Value of current token
  public:
    SdScaner();

    virtual int  lineIndex() const = 0;

    virtual bool nextLine() { return false; }

    virtual void tokenNext() = 0;

    virtual void error( const QString msg );

    char         token() const { return mToken; }

    QString      tokenNeedValue(char tokenVal);

    int          tokenNeedValueInt(char tokenVal) { return tokenNeedValue(tokenVal).toInt(); }

    float        tokenNeedValueFloat(char tokenVal) { return tokenNeedValue(tokenVal).toFloat(); }

    QString      tokenValue() const { return mTokenValue; }

    int          tokenValueInt() const { return mTokenValue.toInt(); }

    float        tokenValueFloat() const { return mTokenValue.toFloat(); }

    double       tokenValueDouble() const { return mTokenValue.toDouble(); }

    bool         matchTokenValue( const QString &val );

    bool         matchToken( char token );

    QString      line() const { return mLine; }

    void         lineSet( const QString &line );

    //!< Skeep spaces
    void         blank();

    bool         isEndOfScan() const { return mEndOfScan; }

    void         scanName();

    //!
    //! \brief scanDouble Scan double digit in common format -123.345e-24
    //! \param allowSign  Allow scan trailing sign
    //! \param useComma   Use comma as delimiter -123,345e-24
    //!
    void scanDouble( bool allowSign, bool useComma );

    //!
    //! \brief scanInteger Scan integer digit in format -1234
    //! \param allowSign   Allow scan trailing sign
    //!
    void scanInteger( bool allowSign );

    void skeepBlock( char openToken, char closeToken );
  };

#endif // SDSCANER_H
