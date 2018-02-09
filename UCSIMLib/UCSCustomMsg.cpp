#include "UCSCustomMsg.h"

UCSCustomMsg::UCSCustomMsg()
{

}

UCSCustomMsg::UCSCustomMsg(QByteArray data)
    : m_data(data)
{

}

QByteArray UCSCustomMsg::data() const
{
    return m_data;
}

void UCSCustomMsg::setData(const QByteArray &data)
{
    m_data = data;
}
