
#ifndef PROXY_PACKAGE_H
#define PROXY_PACKAGE_H

#include <stdlib.h>


#include <sys/types.h>
#include <sys/stat.h>
#ifndef _WIN32
#include <unistd.h>
#endif
#include "iMd5.h"

#include "ucspkg.h"
#include "ucscompress.h"
#include "ucssyncdef.h"
#include "xyssl.h"
#include "PackageHeader.h"
#include "IMmsgstruct.h"
#include "ProxyProtocol.h"
#include <map>
#include <string>
#include <iostream>

#ifndef _WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif
#include <stdio.h>

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
using namespace std;
using namespace Ucs;
using namespace Comm;


typedef unsigned char uint8_t ;


#define IN
#define OUT
int  pack(unsigned int cmd, BodyHead_t* bodyHead, void* bodyData, char* buf, unsigned int& size); 

int  unpack(unsigned int cmd, BodyHead_t* bodyHead, void* bodyData, char* buf, unsigned int size);

#endif
