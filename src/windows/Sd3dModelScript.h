#ifndef SD3DMODELSCRIPT_H
#define SD3DMODELSCRIPT_H

#include <QString>
#include <QList>

struct Sd3dModelScript
  {
    QString mName;
    QString mDescription;
    QString mScript;

    using Sd3dModelScriptList = QList<Sd3dModelScript>;

    static Sd3dModelScriptList scriptList();
  };

using Sd3dModelScriptList = Sd3dModelScript::Sd3dModelScriptList;


#endif // SD3DMODELSCRIPT_H
