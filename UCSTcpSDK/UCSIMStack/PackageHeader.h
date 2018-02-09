﻿/*****************************************************************************
 *
 * File:        PackageHeader.h
 * Author:      iotazhang
 * Revision:    $Id$
 * Description: 数据包头
 *
 *****************************************************************************/

#ifndef UCS_PROXY_PACKAGE_HEADER_H
#define UCS_PROXY_PACKAGE_HEADER_H

#include "ProxyProtocol.h"
#include "UcsByteBuffer.h"

/**
 * 数据包头部
 */
struct PackageHeader {
    /**
     * 构造函数
     */
    PackageHeader();

    unsigned int packageLength;
    unsigned short headerLength;
    unsigned short version;
    unsigned int cmd;
    unsigned int seq;

    /**
     * 重置
     */
    void reset();

    /**
     * 获取payload长度
     */
    unsigned int getPayloadLength() const;

    /**
     * 从缓冲中读取, 读取后packageLength和headerLength被改为剩余未读取长度
     * @return 数据包头部是否正确
     */
    bool read(Ucs::ByteBuffer& buffer);

    /**
     * 写入缓冲
     */
    void write(Ucs::ByteBuffer& buffer);
};


#endif
