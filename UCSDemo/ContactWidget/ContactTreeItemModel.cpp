#include "ContactTreeItemModel.h"
#include "treeItem.h"

#include <QFont>
#include <QIcon>
#include <QString>
#include <QDebug>

ContactTreeItemModel::ContactTreeItemModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    QList<QVariant> rootData;
    rootData << "Contact";
    m_pRootItem = new TreeItem(rootData);
}

ContactTreeItemModel::~ContactTreeItemModel()
{
    delete m_pRootItem;
}

QVariant ContactTreeItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section);
    Q_UNUSED(orientation);
    Q_UNUSED(role);

    // FIXME: Implement me!
    return QVariant();
}

QModelIndex ContactTreeItemModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }

    TreeItem *parentItem;
    if (!parent.isValid())
    {
        parentItem = m_pRootItem;
    }
    else
    {
        parentItem = static_cast<TreeItem*>(parent.internalPointer());
    }

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
    {
        return createIndex(row, column, childItem);
    }

    return QModelIndex();
}

QModelIndex ContactTreeItemModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return QModelIndex();
    }

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parentItem();

    if (parentItem == m_pRootItem)
    {
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

int ContactTreeItemModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
    {
        return 0;
    }

    if (!parent.isValid())
    {
        parentItem = m_pRootItem;
    }
    else
    {
        parentItem = static_cast<TreeItem*>(parent.internalPointer());
    }

    return parentItem->childCount();
}

int ContactTreeItemModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    }

    return m_pRootItem->columnCount();
}

QVariant ContactTreeItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    ContactItem contact;
    if (item->data(0) == "pri")
    {
        contact = m_privateList.at(0);
    }
    else
    {
        for (int i = 0; i < m_organizationList->count(); i++)
        {
            contact = m_organizationList->at(i);
            if (contact.contactId == item->data(1))
            {
                break;
            }
        }
    }

    if (role == Qt::DisplayRole)
    {
        if (contact.userId.isEmpty())
        {
            QString name = QString("%1 [%2]").arg(contact.sectionName)
                                            .arg(contact.userNum);
            return name;
        }
        else
        {
            return contact.userName;
        }
    }
    else if (role == Qt::DecorationRole && index.column() == 0)
    {
        if (contact.userSex == 1)
        {
            return QIcon(":/images/u1185.png");
        }
        else if (contact.userSex == 2)
        {
            return QIcon(":/images/u1183.png");
        }
    }
    else if (role == ContactIdRole)
    {
        return contact.contactId;
    }
    else if (role == SectionNameRole)
    {
        return contact.sectionName;
    }
    else if (role == parentIdRole)
    {
        return contact.parentId;
    }
    else if (role == parentNameRole)
    {
        return contact.parentName;
    }
    else if (role == userNumRole)
    {
        return contact.userNum;
    }
    else if (role == userIdRole)
    {
        return contact.userId;
    }
    else if (role == userSexRole)
    {
        return contact.userSex;
    }
    else if (role == headUrlRole)
    {
        return contact.headUrl;
    }
    else if (role == headPathRole)
    {
        return contact.headPath;
    }

    return QVariant();
}

Qt::ItemFlags ContactTreeItemModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags flags = /*Qt::ItemIsEditable |*/ QAbstractItemModel::flags(index);
    return flags;
}

bool ContactTreeItemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole)
    {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        ContactItem *contact;
        for (int i = 0; i < m_organizationList->count(); i++)
        {
            contact = (ContactItem*)(&(m_organizationList->at(i)));
            if (contact->sectionId == item->data(0))
            {
                QString text = value.toString();
                contact->sectionName = value.toString();

                qDebug() << "setData " << text;
                break;
            }
        }
        emit dataChanged(index, index);
        return true;
    }

    return false;
}

void ContactTreeItemModel::importJSON(QString jsonFile)
{
    ContactItem contact;
    contact.sectionId = "top";
    contact.sectionName = QStringLiteral("组织架构");


//    m_organizationList = Util::parseJson(jsonFile);
    m_organizationList->insert(0, contact);

    ContactItem priContact;
    priContact.sectionId = "pri";
    priContact.sectionName = QStringLiteral("常用联系人");
    m_privateList.append(priContact);

    setupModelData(m_pRootItem);
}

void ContactTreeItemModel::refreshModel()
{
    beginResetModel();
    endResetModel();
}

void ContactTreeItemModel::setupModelData(TreeItem *parent)
{
    QList<TreeItem*> parents;
    QList<TreeItem*> orgParents;
    QList<TreeItem*> priParents;
    TreeItem *orgParent;
    TreeItem *priParent;
    QList<QVariant> orgData;
    QList<QVariant> priData;

    parents << parent;

//    ContactItem priContact = m_privateList.at(0);
//    QList<QVariant> columnData1;
//    columnData1 << priContact.sectionId;
//    priParent = new TreeItem(columnData1, parents.last());
//    parents.last()->appendChild(priParent);


    for (int i = 0; i < m_organizationList->count(); i++)
    {
        ContactItem contact = m_organizationList->at(i);
        QList<QVariant> columnData;
        columnData << contact.sectionId << contact.contactId;

        for (int j = 0; j < parents.count(); j++)
        {
            TreeItem *item = this->item(parents.at(j), contact);
            if (item)
            {
                item->appendChild(new TreeItem(columnData, item));
            }
            else
            {
                parents.last()->appendChild(new TreeItem(columnData, parents.last()));
            }
        }
    }

}

TreeItem *ContactTreeItemModel::item(TreeItem *item, ContactItem contact)
{
    TreeItem *treeItem = NULL;
    if (item == NULL)
    {
        return treeItem;
    }

    QString parentId = contact.parentId;
    QString ItemId = item->data(0).toString();
    ///< 查找父节点 >
    if (!QString::compare(ItemId, parentId, Qt::CaseSensitive))
    {
        treeItem = item;
    }
    else
    {
        for (int j = 0; j < item->childCount(); ++j)
        {
            TreeItem *childItem = item->child(j);
            TreeItem *item2 = this->item(childItem, contact);
            if (item2)
            {
                treeItem = item2;
                break;
            }
        }
    }

    return treeItem;
}

void ContactTreeItemModel::setOrganizationList(QList<ContactItem> *organizationList)
{
    m_organizationList = organizationList;
    setupModelData(m_pRootItem);
    refreshModel();
}
