#include <DBEntity.h>


void DBEntity::convertContact2Entity(const ContactItem &contact, ContactEntity &entity)
{
    entity.contactId = contact.contactId;
    entity.sectionId = contact.sectionId;
    entity.sectionName = contact.sectionName;
    entity.parentId = contact.parentId;
    entity.parentName = contact.parentName;
    entity.grade = contact.grade;
    entity.sortNum = contact.sortNum;
    entity.userNum = contact.userNum;
    entity.userId = contact.userId;
    entity.userName = contact.userName;
    entity.headUrl = contact.headUrl;
    entity.headPath = contact.headPath;
    entity.userSex = contact.userSex;
}

void DBEntity::convertEntity2Contact(const ContactEntity &entity, ContactItem &contact)
{
    contact.contactId = entity.contactId;
    contact.sectionId = entity.sectionId;
    contact.sectionName = entity.sectionName;
    contact.parentId = entity.parentId;
    contact.parentName = entity.parentName;
    contact.grade = entity.grade;
    contact.sortNum = entity.sortNum;
    contact.userNum = entity.userNum;
    contact.userId = entity.userId;
    contact.userName = entity.userName;
    contact.headUrl = entity.headUrl;
    contact.headPath = entity.headPath;
    contact.userSex = entity.userSex;
}
