#ifndef SDSCRIPTVALUEVARIABLE3D_H
#define SDSCRIPTVALUEVARIABLE3D_H

#include "SdScriptValueVariable.h"


class SdScriptValueVariable3d : public SdScriptValueVariable
  {
  protected:
    QList<int> mRefList;
  public:
    SdScriptValueVariable3d();

    //!
    //! \brief toIndex Convert object to index list in reference list
    //! \return        Index list in reference list
    //!
    virtual QList<int> toIndexList() const override { return mRefList; }

    //!
    //! \brief assign Assignment value to variable
    //! \param src    source of value
    //!
    virtual void       assign(SdScriptValuePtr src) override { mRefList = src->toIndexList(); }
  };

#endif // SDSCRIPTVALUEVARIABLE3D_H
