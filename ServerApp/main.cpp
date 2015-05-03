/* 
 * File:   main.cpp
 * Author: Sabeesh
 *
 * Created on 19 April, 2015, 2:48 PM
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include<pthread.h>

#include "ConnectionAcceptor.h"
#include "ConnectionHandler.h"

using namespace std;

/*
 * 
 */

#define DAEMON_NAME "vdaemon"

void process(){

    syslog (LOG_NOTICE, "Writing to my Syslog");
}   

string GetRandomString(int len)
{
    string str = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    string strRet = "";
    int max = 62;
    int min = 1;
    int n = 0;
    while( strRet.size() <= len )
    {
        n = rand()%(str.size()-1)+1;
        strRet.append(str, n,1);
    }
    
    return strRet;   
}


void* Handler(void* pParam)
{
    pthread_detach(pthread_self());
    CConnectionHandler* pCConnectionHandler = NULL;
    bool bContinue = true;
    char buff[1024];
    int iCount = 0;
    
    
    pCConnectionHandler = static_cast<CConnectionHandler*> (pParam);
     
    if( pCConnectionHandler)
    {
        while(bContinue)
        {
            memset(&buff, 0, sizeof(buff));
            iCount = pCConnectionHandler->ReadMessage(buff, 1023);
            if( iCount <= 0 )
            {
                bContinue = false;
                break;
            }
            
            std::string sMessage (buff);
            
            if( sMessage.compare("BYE") == 0 )
            {
                //memset(&buff, 0, sizeof(buff));
                pCConnectionHandler->SendMessage("Bye...", strlen("Bye..."));
                bContinue = false;
                break;
            }
            else if( sMessage.compare("GETMESSAGE") == 0)
            {
                string sStr =  " The Message is " + GetRandomString(50) + "\n";
                const char* pRetString =  sStr.c_str() ; 
                pCConnectionHandler->SendMessage(pRetString, strlen(pRetString));
            }
            else
            {
                pCConnectionHandler->SendMessage("Bad Request\n", strlen("Bad Request\n"));
            }
        }
    }
    
    if( !bContinue && pCConnectionHandler)
    {
        delete pCConnectionHandler;
    }

    pCConnectionHandler = NULL;
}

int main(int argc, char** argv) {

    //port 1024 and 65535.
    //less than 1024 is well-known ports
   //int iSocketFd = 0;
    int iPortNumber = 8787;
     
    //Set our Logging Mask and open the Log
    setlogmask(LOG_UPTO(LOG_NOTICE));
    openlog(DAEMON_NAME, LOG_CONS | LOG_NDELAY | LOG_PERROR | LOG_PID, LOG_USER);

    syslog(LOG_INFO, "Entering Daemon");

    pid_t pid, sid;
    
        if( argc > 3  )
        {
               // cout<<"Usage : server [-p <port>]"<<endl;
                return -1;
        }
    
        CConnectionAcceptor* pCConnectionAcceptor = NULL;
        CConnectionHandler* pCConnectionHandler = NULL;
        
        if( argc == 3 )
        {
            string sOption = argv[1];
            if( sOption.compare("-p") == 0)
            {
                iPortNumber = atoi(argv[2]);
                if( iPortNumber < 1024)
                {
                    //cout<<"well know port is not allowed for this application "<<endl;
                    return -1;
                }
            }
            else
            {
                //cout<<"Unknown option! Please use \'-p\' "<<endl;
                return -1;
            }
        }
        
    //Fork the Parent Process
    pid = fork();

    if (pid < 0) { exit(EXIT_FAILURE); }

    if (pid > 0) { exit(EXIT_SUCCESS); }

    //Change File Mask
    umask(0);

    //Create a new Signature Id 
    sid = setsid();
    if (sid < 0) { exit(EXIT_FAILURE); }

    //Change Directory
    //If we cant find the directory we exit with failure.
    if ((chdir("/")) < 0) { exit(EXIT_FAILURE); }

    //Close Standard File Descriptors
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
  
        pCConnectionAcceptor = new CConnectionAcceptor( iPortNumber);
       
        if(pCConnectionAcceptor->Start()  == 0)
        {
           while( pCConnectionHandler = pCConnectionAcceptor->Connect() )
           {
              pthread_t pThread;
              pthread_create(&pThread, 0, Handler, static_cast<void*>(pCConnectionHandler));
           }            
        }

        closelog ();
        
        return 0;
}

