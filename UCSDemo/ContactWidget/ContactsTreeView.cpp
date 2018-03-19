#include "ContactsTreeView.h"
#include <QFileInfo>
#include <QDir>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonObject>

#include "ContractTreeItemDelegate.h"
#include "treeItem.h"
#include "CommonHelper.h"
#include "UCSLogger.h"
#include "HttpDownloadPicture.h"

ContactsTreeView::ContactsTreeView(QWidget *parent)
    : QTreeView(parent)
{
    setMouseTracking(true);
    setExpandsOnDoubleClick(true);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    setHeaderHidden(true);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setFrameShadow(QFrame::Plain);
    setFrameShape(QFrame::NoFrame);
    /* 去除item选中时的虚线边框 */
    setFocusPolicy(Qt::NoFocus);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setCursor(Qt::ArrowCursor);
    setObjectName("ContractsTreeView");

    /* set icon size for items */
    setIconSize(QSize(25, 25));

    init();
    initConnection();
    initMenu();
//    loadStyleSheet();
}

ContactsTreeView::~ContactsTreeView()
{
    if (m_pRestApi)
    {
        delete m_pRestApi;
        m_pRestApi = Q_NULLPTR;
    }

    if (m_pDownloadWatcher)
    {
        m_pDownloadWatcher->cancel();
        m_pDownloadWatcher->waitForFinished();
    }
}

void ContactsTreeView::doUpdateContacts()
{
    QString userId = CommonHelper::readSetting("Login", "userId", "").toString();
    QString pwd = CommonHelper::readUserPwd("pwd");

    m_pRestApi->doGetContacts(userId, pwd);
}

ContactItem ContactsTreeView::downloadHeader(const ContactItem &contact)
{
    if (contact.headUrl.isEmpty())
    {
        return contact;
    }

    ContactItem newContact = contact;
    QString saveDir = CommonHelper::userTempPath();
    newContact.headPath = HttpDownloadPicture::downloadBlock(contact.headUrl, saveDir);

    return newContact;
}

void ContactsTreeView::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);

    QModelIndex index = currentIndex();

    QString userId = index.data(userIdRole).toString();
    if (userId.isEmpty())
    {
        TreeItem* item = static_cast<TreeItem*>(index.internalPointer());
        if (item->data(0) == "pri")
        {
            m_pGroupMenu->exec(QCursor::pos());
        }
    }
    else
    {
        m_pPersonMenu->exec(QCursor::pos());
    }
}

void ContactsTreeView::init()
{
    m_pRestApi = new UPlusRestApi;
    m_pDownloadWatcher = new QFutureWatcher<ContactItem>(this);

    m_pContactModel = new ContactTreeItemModel(this);
    this->setModel(m_pContactModel);

    ContractTreeItemDelegate *delegate = new ContractTreeItemDelegate(this);
    this->setItemDelegate(delegate);
}

void ContactsTreeView::initConnection()
{
    connect(this, SIGNAL(clicked(QModelIndex)), this, SLOT(onItemClicked(QModelIndex)));
    connect(this, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onItemDoubleClicked(QModelIndex)));
    connect(m_pRestApi, SIGNAL(sigOnGetContactsReply(QByteArray,int)),
            this, SLOT(onUpdateContactsReply(QByteArray,int)));

    connect(m_pDownloadWatcher, SIGNAL(resultReadyAt(int)), this, SLOT(onHeaderReady(int)));
}

void ContactsTreeView::initMenu()
{
    m_pGroupMenu = new QMenu(this);
    m_pPersonMenu = new QMenu(this);
    m_pGroupMenu->setFixedWidth(130);
    m_pPersonMenu->setFixedWidth(130);

    /* 分组右键菜单 */
    QAction *pAddGroup = new QAction(QStringLiteral("添加分组"));
    QAction *pDelGroup = new QAction(QStringLiteral("删除该组"));
    QAction *pAddPerson = new QAction(QStringLiteral("添加联系人"));

    connect(pAddGroup, SIGNAL(triggered(bool)), this, SLOT(onAddGroupAction(bool)));
    connect(pAddPerson, SIGNAL(triggered(bool)), this, SLOT(onAddContactAction(bool)));
    connect(pDelGroup, SIGNAL(triggered(bool)), this, SLOT(onDeleteGroupAction(bool)));

    m_pGroupMenu->addAction(pAddGroup);
    m_pGroupMenu->addAction(pAddPerson);
    m_pGroupMenu->addSeparator();
    m_pGroupMenu->addAction(pDelGroup);

    /* 联系人右键菜单 */
    QAction *pDelAct = new QAction(QIcon(":/images/btn_del.png"), QStringLiteral("删除该联系人"));
    QAction *pMsgAct = new QAction(QIcon(":/images/btn_del.png"), QStringLiteral("发消息"));
    QAction *pAudioAct = new QAction(QIcon(":/images/btn_del.png"), QStringLiteral("音频通话"));
    QAction *pVideoAct = new QAction(QIcon(":/images/btn_del.png"), QStringLiteral("视频通话"));

    connect(pDelAct, SIGNAL(triggered(bool)), this, SLOT(onRemoveContactAction(bool)));
    connect(pMsgAct, SIGNAL(triggered(bool)), this, SLOT(onSendMessageAction(bool)));
    connect(pAudioAct, SIGNAL(triggered(bool)), this, SLOT(onAudioCallAction(bool)));
    connect(pVideoAct, SIGNAL(triggered(bool)), this, SLOT(onVideoCallAction(bool)));

    m_pPersonMenu->addAction(pMsgAct);
    m_pPersonMenu->addAction(pAudioAct);
    m_pPersonMenu->addAction(pVideoAct);
    m_pPersonMenu->addSeparator();
    m_pPersonMenu->addAction(pDelAct);
}

void ContactsTreeView::loadStyleSheet()
{
    QFile file(":/Resources/TreeView/TreeView.qss");
    file.open(QFile::ReadOnly);
    if (file.isOpen())
    {
        QString styleSheet = this->styleSheet();
        styleSheet += QString(file.readAll());
        this->setStyleSheet(styleSheet);
        file.close();
    }
}

void ContactsTreeView::parseContactData()
{
    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(m_contactData, &jsonError);
    if ( jsonError.error == QJsonParseError::NoError &&
         document.isObject())
    {
        int ret;
        QString retMsg;
        QJsonObject rootObj = document.object();
        if (rootObj.contains("ret"))
        {
            ret = rootObj["ret"].toInt();
        }

        if (rootObj.contains("retMsg"))
        {
            retMsg = rootObj["retMsg"].toString();
        }

        if (ret != 0)
        {
            UCS_LOG(UCSLogger::kTraceError, this->objectName(),
                    QString(QStringLiteral("更新通讯录")).append(retMsg));
            return;
        }
        if (rootObj.contains("version"))
        {
            QString newVersion = rootObj["version"].toString();
            if (QString::compare(newVersion, m_contactVer, Qt::CaseInsensitive) == 0)
            {
                return;
            }
            else
            {
                m_contactVer = newVersion;
            }

#if 0   ///< 未使用数据库或文件，暂不适用 >
            QString oldVersion = CommonHelper::readSetting("Contact", "version", "");
            if (QString::compare(newVersion, oldVersion, Qt::CaseInsensitive) == 0)
            {
                return;
            }
            CommonHelper::saveSetting("Contact", "version", newVersion);
#endif
        }

        if (rootObj.contains("response"))
        {
            ContactList sectionList;
            int contactId = 0;

            m_contactList.clear();

            ContactItem topItem;
            topItem.contactId = contactId++;
            topItem.sectionId = "toporg";
            topItem.sectionName = QStringLiteral("云之讯");
//            m_contactList.append(topItem);

            QJsonArray contacts = rootObj["response"].toArray();
            for (int idx = 0; idx < contacts.size(); ++idx)
            {
                bool isExist = false;
                ContactItem section;
                section.contactId = contactId++;

                QJsonObject contactObj = contacts.at(idx).toObject();

                if (contactObj.contains("name"))
                {
                    section.sectionName = contactObj["name"].toString();
                }
                if (contactObj.contains("grade"))
                {
                    section.grade = contactObj["grade"].toInt();
                }
                if (contactObj.contains("sectionid"))
                {
                    section.sectionId = contactObj["sectionid"].toString();
                }
                if (contactObj.contains("sortNum"))
                {
                    section.sortNum = contactObj["sortNum"].toInt();
                }
                if (contactObj.contains("personNum"))
                {
                    section.userNum = contactObj["personNum"].toInt();
                }
                if (section.grade == 1)
                {
                    section.parentId = topItem.sectionId;
                    topItem.userNum += section.userNum;
                }

                for (int idx = 0; idx < sectionList.size(); ++idx)
                {
                    if (section.sectionId == sectionList.at(idx).sectionId)
                    {
                        isExist = true;
                        section.contactId = sectionList.at(idx).contactId;
                        section.parentId = sectionList.at(idx).parentId;
                        section.parentName = sectionList.at(idx).parentName;

                        sectionList.replace(idx, section);
                    }
                }
                if (isExist == false)
                {
                    sectionList.append(section);
                }

                if (contactObj.contains("person"))
                {
                    QJsonArray persons = contactObj["person"].toArray();
                    for (int index = 0; index < persons.size(); ++index)
                    {
                       ContactItem person;
                       person.contactId = contactId++;
//                       person.sectionId = section.sectionId;
                       person.parentId = section.sectionId;
                       person.parentName = section.sectionName;
                       person.grade = section.grade;

                       QJsonObject personObj = persons.at(index).toObject();
                       if (personObj.contains("userid"))
                       {
                           person.userId = personObj["userid"].toString();
//                           person.sectionId = person.userId;
                       }
                       if (personObj.contains("name"))
                       {
                           person.userName = personObj["name"].toString();
                       }
                       if (personObj.contains("headimg"))
                       {
                           person.headUrl = personObj["headimg"].toString();
                       }
                       if (personObj.contains("sex"))
                       {
                           person.userSex = personObj["sex"].toInt();
                       }
                       sectionList.append(person);
                    }
                }

                if (contactObj.contains("section"))
                {
                    QJsonArray childArray = contactObj["section"].toArray();
                    for (int index = 0; index < childArray.size(); ++index)
                    {
                        QJsonObject childObj = childArray.at(index).toObject();
                        ContactItem childSection;

                        childSection.contactId = contactId++;
                        childSection.parentId = section.sectionId;
                        childSection.parentName = section.sectionName;

                        if (childObj.contains("name"))
                        {
                            childSection.sectionName = childObj["name"].toString();
                        }
                        if (childObj.contains("grade"))
                        {
                            childSection.grade = childObj["grade"].toInt();
                        }
                        if (childObj.contains("sectionid"))
                        {
                            childSection.sectionId = childObj["sectionid"].toString();
                        }
                        sectionList.append(childSection);
                    }
                }
            }

            m_contactList.append(topItem);
            m_contactList.append(sectionList);
            m_pContactModel->setOrganizationList(&m_contactList);
            startDownloadHeader();
        }
    }
}

void ContactsTreeView::startDownloadHeader()
{
    m_pDownloadWatcher->setFuture(QtConcurrent::mapped(m_contactList, ContactsTreeView::downloadHeader));
}

void ContactsTreeView::onItemClicked(QModelIndex index)
{
    int contactId = index.data(ContactIdRole).toInt();
    QString userId = index.data(userIdRole).toString();

    UCS_LOG(UCSLogger::kTraceDebug, this->objectName(),
            QString("onItemClicked() contactId: %1 userId: %2")
                    .arg(contactId).arg(userId));

    if (!userId.isEmpty())
    {
        for (int idx = 0; idx < m_contactList.size(); ++idx)
        {
            ContactItem contact = m_contactList.at(idx);
            if (contactId == contact.contactId)
            {
                QString parentName = index.data(parentNameRole).toString();
                if (!parentName.isEmpty())
                {
                    QModelIndex parent = index.parent();
                    while(parent.isValid())
                    {
                        QString name = parent.data(parentNameRole).toString();
                        if (!name.isEmpty())
                        {
                            parentName.insert(0, QString("%1/").arg(name));
                        }
                        else
                        {
                            break;
                        }
                        parent = parent.parent();
                    }
                }
                contact.parentName = parentName;
                emit sigItemClicked(contact);
            }
        }
    }
}

void ContactsTreeView::onItemDoubleClicked(QModelIndex index)
{
    int contactId = index.data(ContactIdRole).toInt();
    QString userId = index.data(userIdRole).toString();

    UCS_LOG(UCSLogger::kTraceDebug, this->objectName(),
            QString("onItemDoubleClicked() contactId: %1 userId: %2")
                    .arg(contactId).arg(userId));
}

void ContactsTreeView::onItemPressed(QModelIndex index)
{
    int contactId = index.data(ContactIdRole).toInt();
    QString userId = index.data(userIdRole).toString();

    UCS_LOG(UCSLogger::kTraceDebug, this->objectName(),
            QString("onItemDoubleClicked() contactId: %1 userId: %2")
                    .arg(contactId).arg(userId));
}

void ContactsTreeView::onAddGroupAction(bool checked)
{
    Q_UNUSED(checked);
}

void ContactsTreeView::onDeleteGroupAction(bool checked)
{
    Q_UNUSED(checked);
}

void ContactsTreeView::onAddContactAction(bool checked)
{
    Q_UNUSED(checked);
}

void ContactsTreeView::onRemoveContactAction(bool checked)
{
    Q_UNUSED(checked);
    qDebug() << "remove " << currentIndex().data().toString();
}

void ContactsTreeView::onSendMessageAction(bool checked)
{
    Q_UNUSED(checked);
    qDebug() << "msg " << currentIndex().data().toString();
}

void ContactsTreeView::onAudioCallAction(bool checked)
{
    Q_UNUSED(checked);
    qDebug() << "audio " << currentIndex().data().toString();
}

void ContactsTreeView::onVideoCallAction(bool checked)
{
    Q_UNUSED(checked);
    qDebug() << "video " << currentIndex().data().toString();
}

void ContactsTreeView::onUpdateContactsReply(QByteArray data, int code)
{
    if (code == HTTP_OK)
    {
        m_contactData = data;

#if 0
        QString filename = CommonHelper::userDataPath() + "/contacts.json";
        QFileInfo fileInfo(filename);
        if (fileInfo.exists())
        {
            fileInfo.dir().remove(fileInfo.fileName());
        }

        QFile file(filename);
        if (file.open(QIODevice::WriteOnly))
        {
            file.write(m_contactData);
        }
        file.close();
#endif

        parseContactData();
    }
    else
    {
        UCS_LOG(UCSLogger::kTraceError, this->objectName(),
                QString(QStringLiteral("拉取通讯录失败(%1)")).arg(code));
    }
}

void ContactsTreeView::onHeaderReady(int index)
{
    m_contactList.replace(index, m_pDownloadWatcher->resultAt(index));
}
