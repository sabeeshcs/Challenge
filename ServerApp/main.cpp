/* 
 * File:   main.cpp
 * Author: Sabeesh
 *
 * Created on 19 April, 2015, 2:48 PM
 */

//#include <cstdlib>
//#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
//#include<sys/types.h>
//#include<sys/socket.h>
//#include<netinet/in.h>
#include<pthread.h>
//#include <iostream>
//#include <fstream>
//#include <strings.h>
//#include <stdlib.h>
//#include <string>

#include "ConnectionAcceptor.h"
#include "ConnectionHandler.h"

using namespace std;

/*
 * 
 */

string GetRandomString(int len)
{
   srand(time(0));
   string str = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
   int iPos;
   while(str.size() != len) {
    iPos = ((rand() % (str.size() - 1)));
    str.erase (iPos, 1);
   }
   return str;
   
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

   //int iSocketFd = 0;
    int iPortNumber = 8787;
     
        if( argc > 3  )
        {
                cout<<"Usage : server [-p <port>]"<<endl;
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
            }
            else
            {
                cout<<"Unknown option! Please use \'-p\' "<<endl;
                return -1;
            }
        }
        
        
        pCConnectionAcceptor = new CConnectionAcceptor( iPortNumber);
       
        if(pCConnectionAcceptor->Start()  == 0)
        {
           while( pCConnectionHandler = pCConnectionAcceptor->Connect() )
           {
              pthread_t pThread;
              pthread_create(&pThread, 0, Handler, static_cast<void*>(pCConnectionHandler));
           }            
        }

        return 0;
}

