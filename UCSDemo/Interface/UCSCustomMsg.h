#ifndef UCSCUSTOMMSG_H
#define UCSCUSTOMMSG_H

#include <QObject>
#include <qglobal.h>
#include <UCSMsgContent.h>

/*!
 * \brief The UCSCustomMsg class 自定义消息
 */
class UCSIMLIBSHARED_EXPORT UCSCustomMsg : public UCSMsgContent
{
public:
    explicit UCSCustomMsg();
    explicit UCSCustomMsg(QByteArray data);
    ~UCSCustomMsg(){}

    QByteArray data() const;

    void setData(const QByteArray &data);

private:
    ///< 传输的字节流。最大64K >
    QByteArray m_data;
};

#endif // UCSCUSTOMMSG_H
