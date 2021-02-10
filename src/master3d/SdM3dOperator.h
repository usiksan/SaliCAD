#ifndef SDM3DOPERATOR_H
#define SDM3DOPERATOR_H


class SdM3dOperator
  {
  public:
    SdM3dOperator();
    virtual ~SdM3dOperator() {}

    virtual void execute() = 0;
  };

#endif // SDM3DOPERATOR_H
