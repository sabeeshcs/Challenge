/* 
 * File:   ConnectionHandler.h
 * Author: Sabeesh
 *
 * Created on 19 April, 2015, 4:04 PM
 */
#include <string>


#ifndef CONNECTIONHANDLER_H
#define	CONNECTIONHANDLER_H

using namespace std;

class CConnectionHandler
{
    int m_iSockFd;
public:
    ~CConnectionHandler();
    ssize_t ReadMessage(char* buffer, size_t len);
    ssize_t SendMessage(const char* buffer, size_t len);
    int GetSocketID();
    
    friend class CConnectionAcceptor;
    
private:
    CConnectionHandler(int iSockFd);
    CConnectionHandler();
    CConnectionHandler(const CConnectionHandler& pCConnectionHandler);

};


#endif	/* CONNECTIONHANDLER_H */

