#ifndef SDM3DFUNINPUT_H
#define SDM3DFUNINPUT_H

#include "SdM3dFunction.h"

#include <QTableWidget>

class SdM3dFunInput : public SdM3dFunction
  {
  protected:
    QTableWidget *mTableWidget;
    int           mRow;
    char          mInputParamTypes[2];
  public:
    SdM3dFunInput( char resultType, QTableWidget *tableWidget );
  };

#endif // SDM3DFUNINPUT_H
