#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>

#include "ConnectionHandler.h"

using namespace std;

CConnectionHandler::CConnectionHandler(int iSockFd) : m_iSockFd (iSockFd)
{
     
}
 
CConnectionHandler::CConnectionHandler()
{
     
}
 
CConnectionHandler::CConnectionHandler(const CConnectionHandler& pCConnectionHandler)
{
     
}
 
CConnectionHandler::~CConnectionHandler()
{ 
    cout<<"Closing the connection " << m_iSockFd << endl;
    close(m_iSockFd);
}
  
ssize_t CConnectionHandler::ReadMessage(char* buffer, size_t len)
{
    return read(m_iSockFd, buffer, len);
}

ssize_t CConnectionHandler::SendMessage(const char* buffer, size_t len)
{
    return write(m_iSockFd, buffer, len);
}

int CConnectionHandler::GetSocketID()
{
    return m_iSockFd;
}
