#include "UCSTextMsg.h"

UCSTextMsg::UCSTextMsg()
{

}

UCSTextMsg::UCSTextMsg(QString content)
    : m_content(content)
{

}

QString UCSTextMsg::content() const
{
    return m_content;
}
