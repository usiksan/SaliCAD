/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Script language is C-style hi-level language for programming 3d models, 2d parametric graphics and
  schematic calculations.

  SdScriptRefMap represents map which associates key and value. Key is name of variable or param.
  Value is value of variable or param.

  All member functions are pure virtual and must be overriding in derived classes
*/
#ifndef SDSCRIPTREFMAP_H
#define SDSCRIPTREFMAP_H

#include <QString>

class SdScriptRefMap
  {
  public:
    SdScriptRefMap() {}
    virtual ~SdScriptRefMap() {}

    //!
    //! \brief varInit Appends key into map
    //! \param key     Appended key
    //!
    virtual void    varInit( const QString &key ) = 0;

    //!
    //! \brief varGet Returns value associated with this key
    //! \param key    Key for which value must be returned
    //! \return       Value associated with this key
    //!
    virtual QString varGet( const QString &key ) const = 0;

    //!
    //! \brief varSet Replace previously value associated with key with new value
    //! \param key    Key for which value must be replaced
    //! \param val    New value associated with key
    //!
    virtual void    varSet( const QString &key, const QString &val ) = 0;
  };

#endif // SDSCRIPTREFMAP_H
