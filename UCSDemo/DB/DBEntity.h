#ifndef DBENTITY_H
#define DBENTITY_H
#include <QObject>
#include <QString>
#include <QMap>
#include <QVariant>
#include <UCSIMDefine.h>
#include <UCSConversation.h>
#include <UCSTcp/UCSStackDefine.h>

///< 更新时用到的键值对，为此，所有实体的字段都用QString表示 >
typedef QMap<QString, QString> MapValues, MapConditions;

class DBEntity
{
public:
    explicit DBEntity() {}
};

#endif // DBENTITY_H
