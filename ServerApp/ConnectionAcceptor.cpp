#include <sys/types.h>
#include<sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "ConnectionAcceptor.h"
#include "ConnectionHandler.h"

CConnectionAcceptor::CConnectionAcceptor(int iport )
    : m_iPort(iport), m_sAddress(""), m_blistening(false)
{

}

CConnectionAcceptor::~CConnectionAcceptor()
{

}

int CConnectionAcceptor::Start()
{
    if (m_blistening == true) 
    {
        return 1;
    }

   //m_iSocketFd = socket(AF_INET, SOCK_STREAM, 0);
    m_iSocketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if( m_iSocketFd < 0)
    {
        cout<<"Sorry! Can't create a Scoket!";
        return 1;
    }
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(m_iPort);
    
    if (m_sAddress.size() > 0) {
        inet_pton(AF_INET, m_sAddress.c_str(), &(serv_addr.sin_addr));
    }
    else {
        serv_addr.sin_addr.s_addr = INADDR_ANY;
    }

    int iResult = bind(m_iSocketFd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if( iResult != 0)
    {
        cout<<"Sorry! Can't create a Scoket!";
        return iResult;
    }

    //iResult = listen(m_iSocketFd, 5);
    iResult = listen( m_iSocketFd, SOMAXCONN);
    
    if (iResult != 0) 
    {
        /*switch ( iResult )
        {
            case EBADF:
                cout<<"Not a valid descriptor"<<endl;
                break;                
            case EINCONN:
                cout<<"Socket is already connected"<<endl;
                break;  
            case EINVAL:
                 cout<<"Socket has not been bound to a local address."<<endl;
                 break;
            case EMFILE:
                cout<<"No more socket descriptors are available."<<endl;
                break;
            case ENETDOWN:
                cout<<"Network subsystem has failed."<<endl;
                break;
            case ENOBUFS:
                cout<<"No buffer space is available." << endl;
                break;
            case ENOTSOCK:
                cout<<"Not a socket"<< endl
                break;
            case EOPNOTSUPP:
                cout << "Socket that supports listen()."<<endl;
                break;                        
        };*/
                
        cout<<"Sorry! listen faild!";
        return iResult;
    }
    
    m_blistening = true;
    return iResult;    
    
}

CConnectionHandler* CConnectionAcceptor::Connect()
{
    if( m_blistening== false)
        return NULL; struct 
            
    sockaddr_in client_addr;
    socklen_t len;
        
    memset(&client_addr, 0, sizeof(client_addr));
    len = sizeof(client_addr);
    
    int iSockFd = ::accept(m_iSocketFd, (struct sockaddr*)&client_addr, &len);
    if( iSockFd < 0)
    {
        cout<<"Sorry! accept connection faild!";
        return NULL;
    }

    return new CConnectionHandler(iSockFd);  
    
}