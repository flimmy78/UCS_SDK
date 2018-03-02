#include "midLeftContactTreeModel.h"
#include "treeItem.h"

#include <QFont>
#include <QIcon>
#include <QString>
#include <QDebug>

MidLeftContactTreeModel::MidLeftContactTreeModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    QList<QVariant> rootData;
    rootData << "Contact";
    m_pRootItem = new TreeItem(rootData);
}

MidLeftContactTreeModel::~MidLeftContactTreeModel()
{
    delete m_pRootItem;
}

QVariant MidLeftContactTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
    return QVariant();
}

QModelIndex MidLeftContactTreeModel::index(int row, int column, const QModelIndex &parent) const
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

QModelIndex MidLeftContactTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return QModelIndex();
    }

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parent();

    if (parentItem == m_pRootItem)
    {
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

int MidLeftContactTreeModel::rowCount(const QModelIndex &parent) const
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

int MidLeftContactTreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    }

    return m_pRootItem->columnCount();
}

QVariant MidLeftContactTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    ContactUtil contactUtil;
    if (item->data(0) == "pri")
    {
        contactUtil = m_privateList.at(0);
    }
    else
    {
        for (int i = 0; i < m_organizationList.count(); i++)
        {
            contactUtil = m_organizationList.at(i);
            if (contactUtil.sectionId == item->data(0))
            {
                break;
            }
        }
    }

    if (role == Qt::DisplayRole)
    {
        return contactUtil.name;
    }
    else if (role == Qt::DecorationRole && index.column() == 0)
    {
        if (contactUtil.sex == 0)
        {
            return QIcon(":/images/u1185.png");
        }
        else if (contactUtil.sex == 1)
        {
            return QIcon(":/images/u1183.png");
        }
    }
//    else if (role == Qt::FontRole)
//    {
//        return QFont("微软雅黑", 10, QFont::Normal, false);
//    }
    else if (role == GroupRole)
    {
        return contactUtil.sex < 0;
    }
    else if (role == SexRole)
    {
        return contactUtil.sex;
    }

    return QVariant();
}

Qt::ItemFlags MidLeftContactTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags flags = /*Qt::ItemIsEditable |*/ QAbstractItemModel::flags(index);
    return flags;
}

bool MidLeftContactTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole)
    {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        ContactUtil *contactUtil;
        for (int i = 0; i < m_organizationList.count(); i++)
        {
            contactUtil = (ContactUtil*)(&(m_organizationList.at(i)));
            if (contactUtil->sectionId == item->data(0))
            {
                QString text = value.toString();
                contactUtil->name = value.toString();

                qDebug() << "setData " << text;
                break;
            }
        }
        emit dataChanged(index, index);
        return true;
    }

    return false;
}

void MidLeftContactTreeModel::importJSON(QString jsonFile)
{
    ContactUtil contact;
    contact.sectionId = "top";
    contact.name = QStringLiteral("组织架构");
    contact.sex = -1;
    contact.grade = 0;

    m_organizationList = Util::parseJson(jsonFile);
    m_organizationList.insert(0, contact);

    ContactUtil priContact;
    priContact.sectionId = "pri";
    priContact.name = QStringLiteral("常用联系人");
    priContact.sex = -1;
    priContact.grade = 0;
    m_privateList.append(priContact);

    setupModelData(m_pRootItem);
}

void MidLeftContactTreeModel::setupModelData(TreeItem *parent)
{
    QList<TreeItem*> parents;
    QList<TreeItem*> orgParents;
    QList<TreeItem*> priParents;
    TreeItem *orgParent;
    TreeItem *priParent;
    QList<QVariant> orgData;
    QList<QVariant> priData;

    parents << parent;

    ContactUtil priContact = m_privateList.at(0);
    QList<QVariant> columnData1;
    columnData1 << priContact.sectionId;
    priParent = new TreeItem(columnData1, parents.last());
    parents.last()->appendChild(priParent);


    for (int i = 0; i < m_organizationList.count(); i++)
    {
        ContactUtil contact = m_organizationList.at(i);
        QList<QVariant> columnData;
        columnData << contact.sectionId;

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

TreeItem *MidLeftContactTreeModel::item(TreeItem *item, ContactUtil contact)
{
    TreeItem *treeItem = NULL;
    if (item == NULL)
    {
        return treeItem;
    }

    QString parentSecId = contact.parentSecId;
    if (!QString::compare(item->data(0).toString(), parentSecId, Qt::CaseSensitive))
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
