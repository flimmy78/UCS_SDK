#ifndef CONTACT_TREE_ITEM_MODEL_H
#define CONTACT_TREE_ITEM_MODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

class TreeItem;

enum ContactRole
{
    SectionNameRole = Qt::UserRole,
    SectionIdRole,
    parentIdRole,
    gradeRole,
    userNumRole,
    userIdRole,
    userNameRole,
    userSexRole,
    headUrlRole,
    headPathRole,
};

typedef struct ContactItem
{
    ContactItem() {
        sectionName = "";
        sectionId = "";
        parentId = "";
        grade = 0;
        sortNum = 0;
        userNum = 0;
        userId = "";
        userName = "";
        userSex = 0;
        headUrl = "";
        headPath = "";
    }

    int     contactId;  ///< 唯一标识 >
    QString sectionName;   ///< 直属部门 >
    QString sectionId;  ///< 部门Id >
    QString parentId;   ///< 上一级部门 >
    int     grade;      ///< 部门层级 >
    int     sortNum;    ///< 顶级部门排序 >
    int     userNum;    ///< 本部门人员数 >
    QString userId;     ///< 人员Id >
    QString userName;   ///< 人员名称 >
    int     userSex;    ///< 人员性别 >
    QString headUrl;    ///< 头像地址 >
    QString headPath;   ///< 头像本地路径 >
} ContactItem;
typedef QList<ContactItem> ContactList;

class ContactTreeItemModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit ContactTreeItemModel(QObject *parent = 0);
    ~ContactTreeItemModel();

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    // Fetch data dynamically:
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // for a editable model
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    // custom functionality:
    void importJSON(QString jsonFile);

    void refreshModel();
    void setOrganizationList(ContactList *organizationList);

private:
    void setupModelData(TreeItem *parent);
    TreeItem *item(TreeItem *item, ContactItem contract);

private:
    ContactList *m_organizationList;
    QList<ContactItem> m_privateList;
//    QMap<QString, QList> m_listMap;
    TreeItem *m_pRootItem;
};

#endif // CONTACT_TREE_ITEM_MODEL_H
