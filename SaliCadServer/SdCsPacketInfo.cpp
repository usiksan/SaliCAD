/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdCsPacketInfo.h"

SdCsPacketInfo::SdCsPacketInfo(int cmd, int len)
  {
  setCommand( cmd, len );
  }



//Command access
void SdCsPacketInfo::setCommand(int cmd, int len)
  {
  //Setup command
  mCommand = (quint8)cmd;
  //and clear lenght
  setLenght( len );
  }





//Lenght access
void SdCsPacketInfo::setLenght(int len)
  {
  mLenght[0] = (quint8) (len & 0xff);
  mLenght[1] = (quint8) ((len >> 8) & 0xff);
  mLenght[2] = (quint8) ((len >> 16) & 0xff);
  mLenght[3] = (quint8) ((len >> 24) & 0xff);
  }



int SdCsPacketInfo::lenght() const
  {
  int val;
  val = mLenght[3];
  val <<= 8;
  val |= mLenght[2];
  val <<= 8;
  val |= mLenght[1];
  val <<= 8;
  val |= mLenght[0];
  return val;
  }
