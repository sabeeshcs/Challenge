
#include <iostream>
#include <string.h>

using namespace std;
/* 
 * File:   ConnectionAcceptor.h
 * Author: Sabeesh
 *
 * Created on 19 April, 2015, 2:49 PM
 */

#ifndef CONNECTIONACCEPTOR_H
#define	CONNECTIONACCEPTOR_H

class CConnectionHandler;

class CConnectionAcceptor
{
        int m_iPort;
        int m_iSocketFd;
        bool m_blistening;
        string m_sAddress;

        public:
                CConnectionAcceptor(int iport );
                ~CConnectionAcceptor();
                int Start();
                CConnectionHandler* Connect();
                

        private:
                CConnectionAcceptor()
                {

                }

};




#endif	/* CONNECTIONACCEPTOR_H */

