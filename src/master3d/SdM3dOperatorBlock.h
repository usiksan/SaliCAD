#ifndef SDM3DOPERATORBLOCK_H
#define SDM3DOPERATORBLOCK_H

#include "SdM3dOperator.h"

#include <QList>

class SdM3dOperatorBlock : public SdM3dOperator
  {
    QList<SdM3dOperatorPtr> mOperatorList;
  public:
    SdM3dOperatorBlock();
    ~SdM3dOperatorBlock();

    void append( SdM3dOperatorPtr op ) { mOperatorList.append( op ); }


    // SdM3dOperator interface
  public:
    virtual void execute() override;
  };

#endif // SDM3DOPERATORBLOCK_H
