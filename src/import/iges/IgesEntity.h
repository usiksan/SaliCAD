#ifndef IGESENTITY_H
#define IGESENTITY_H

#include <QByteArray>
#include <QList>

class IgesReader;

#define IgesIntEmpty 0x80000000

class IgesEntity
  {
    int  mFields[19];
  protected:
    int  mParameterData() const { return mFields[0]; }      //2 [0]
    int  mStructure() const { return mFields[1]; }          //3 [1]
    int  mLineFontPattern() const { return mFields[2]; }    //4 [2]
    int  mLevel() const { return mFields[3]; }              //5 [3]
    int  mView() const { return mFields[4]; }               //6 [4]
    int  mTransformMatrix() const { return mFields[5]; }    //7 [5]
    int  mLabelDisplayAssoc() const { return mFields[6]; }  //8 [6]
    char mStatus[8];          //9
    int  mLineWeightNumber() const { return mFields[7]; }   //12 [7]
    int  mColorNumber() const { return mFields[8]; }        //13 [8]
    int  mParameterLineCount() const { return mFields[9]; } //14 [9]
    int  mFormNumber() const { return mFields[10]; }        //15 [10]
    char mEntityLabel[8];     //18
    int  mEntitySupscript() const { return mFields[11]; }   //19 [11]
  public:
    IgesEntity();

    void         setField( int fieldIndex, const QByteArray &ar );

    virtual bool parse(IgesReader *reader );

    static IgesEntity *build( int type );
  };

using IgesEntityPtr = IgesEntity*;

using IgesEntityPtrList = QList<IgesEntityPtr>;

#endif // IGESENTITY_H
