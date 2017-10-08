/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Packet - is the transfer header. Id describes information block.
*/
#ifndef SDCSPACKET_H
#define SDCSPACKET_H

#include <QString>

class SdCsPacket
  {
    quint8 mCommand;   //Command with block description
    quint8 mLenght[4]; //Block lenght
    quint8 mOffset[4]; //Offset block in the group
  public:
    SdCsPacket();

    //Command access
    void setCommand( int cmd );
    int  command() const { return mCommand; }

    //Lenght access
    void setLenght( int len );
    int  lenght() const;

    //Offset access
    void setOffset( int off );
    int  offset() const;
  };

#endif // SDCSPACKET_H
