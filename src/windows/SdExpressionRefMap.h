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

  SdExpressionRefMap derived class from SdScriptRefMap represents map which associates key and value.
  Key is name of variable or param.  Value is value of variable or param.

  SdExpressionRefMap represents map for schematic calculations
*/
#ifndef SDEXPRESSIONREFMAP_H
#define SDEXPRESSIONREFMAP_H

#include "script/SdScriptRefMap.h"
#include <QTableWidget>
#include <QMap>

class SdExpressionRefMap : public SdScriptRefMap
  {
    QTableWidget      *mTable;   //!< Visual table for edit referenced variables and its values
    QList<QString>     mRefList; //!< List of keys used to reorganize visual variable table
    QMap<QString,int>  mRefMap;  //!< Map of keys with associated indexes in mTable
  public:
    SdExpressionRefMap( QTableWidget *table );

    //!
    //! \brief parseEnd Call at end of parsing to reorganize visual table of variables
    //!
    void parseEnd();

    // SdScriptRefMap interface
  public:
    //!
    //! \brief varInit Appends key into map
    //! \param key     Appended key
    //!
    //! Overrided function. It append key to mRefList if it yet appended
    virtual void    varInit(const QString &key) override;

    //!
    //! \brief varGet Returns value associated with this key
    //! \param key    Key for which value must be returned
    //! \return       Value associated with this key
    //!
    //! Overrided function. It extract value of key from visual table
    virtual QString varGet(const QString &key) const override;

    //!
    //! \brief varSet Replace previously value associated with key with new value
    //! \param key    Key for which value must be replaced
    //! \param val    New value associated with key
    //!
    //! Overrided function. It replace value associated with key with new value
    //! in visual table
    virtual void    varSet(const QString &key, const QString &val) override;
  };

#endif // SDEXPRESSIONREFMAP_H
