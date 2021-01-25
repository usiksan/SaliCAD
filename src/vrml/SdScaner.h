#ifndef SDSCANER_H
#define SDSCANER_H

#include <QString>

class SdScaner
  {
    char    mToken;      //!< Current scanned token
    QString mLine;       //!< Input line
    int     mIndex;      //!< Current char index
    bool    mEndOfScan;  //!< Flag of scan completion
    QString mTokenValue; //!< Value of current token
  public:
    SdScaner();

  protected:
    virtual int  lineIndex() const = 0;

    virtual bool nextLine() { return false; }

    void lineSet( const QString &line );

    //!< Skeep spaces
    void blank();

    bool isEndOfScan() const { return mEndOfScan; }

    void scanName();

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
  };

#endif // SDSCANER_H
