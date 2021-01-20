#include "SdPItemExpression.h"

SdPItemExpression::SdPItemExpression()
  {

  }


QString SdPItemExpression::getType() const
  {
  return QStringLiteral(SD_TYPE_EXPRESSION);
  }



SdClass SdPItemExpression::getClass() const
  {
  }

void SdPItemExpression::cloneFrom(const SdObject *src)
  {
  }

void SdPItemExpression::writeObject(QJsonObject &obj) const
  {
  }

void SdPItemExpression::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  }

QString SdPItemExpression::getIconName() const
  {
  }

quint64 SdPItemExpression::getAcceptedObjectsMask() const
  {
  }
