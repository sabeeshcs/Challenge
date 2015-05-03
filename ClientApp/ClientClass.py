#usr/bin/python


import sys, socket, getopt, os

class CConnectionAcceptor:
	m_iPort = 0
	m_sHost = ''
	m_iCount = 0
	m_iSocketFd = 0
	m_pSocket = None
	m_iConnect = 0

	def __init__(self, sHost, iPort, iCount):
		self.m_iPort = iPort
		self.m_iCount = iCount
		self.m_sHost = sHost
		self.CreateSocket();
	
	def __del__(self):
		if self.m_iSocketFd > 0:
			self.m_iSocketFd.close();
	
	def CreateSocket(self):
		self.m_pSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
	
	def Connect(self):
		bRet = False;
		try:
			self.m_iSocketFd = self.m_pSocket.connect( (self.m_sHost, self.m_iPort) )
			bRet = True
		except Exception, e:
			print "Sorry Can't Connect to the Server! Is the Server running in Host '%s' and Port '%s'?" %(self.m_sHost, self.m_iPort)
			print "Error : ( %s )." %(e)
		return bRet

	def SendMessage(self, sMessage):
		bRet = False
		if self.m_iSocketFd <= 0:
			try:
				iSend = self.m_pSocket.send(sMessage);
				if iSend == 0:
					raise RuntimeError ("Socket connection faild!")
				else:
					bRet = True
			except Exception, e:
				print("Sorry! Cann't send message! Please check the error %s" %(e))
		else:
			print("Sorry! Connection faild!")
		return bRet

	def ReadMessage(self):
		sMessage = ''
		if self.m_iSocketFd <= 0:			
			try:
				sMessage = self.m_pSocket.recv(1024);
			except Exception, e:
				print("Sorry! Cann't read message from Server!  Please check the error %s" %(e))
		else:
			 print("Sorry! Connection faild!")
		return sMessage

	def Communicate(self):
		if self.m_iSocketFd <= 0:
			iPrID = os.getpid()	#curretn process ID
			sLogFile = "logfile_" +str(iPrID) + ".txt"

			#while True:   #for a loop until user enter BYE
				#sSendMessage = raw_input("Input command:")  # for input a command from command prompt by the user
			sSendMessage = 'GETMESSAGE'	#always send the same message for <n> times
			iCount = 0
			iPrID = os.getpid()
			with open( sLogFile, "a") as logfile:
				while iCount <= self.m_iCount:
					if len(sSendMessage) > 0:
						if self.SendMessage(sSendMessage) == True:
							sReadMessage = self.ReadMessage()
							logfile.write(str(iCount + 1) + sReadMessage)
							print sReadMessage
						#if sSendMessage == "BYE":
						#	break
					else:
						break
					if iCount + 1 == self.m_iCount:
						sSendMessage = 'BYE'
					iCount = iCount + 1
				logfile.close()
			print "Your Logfile is %s" %(sLogFile)

def main(argv):
	sHost = ''
	iPort = 0
	iCount = 0
	bContinue = True
	try:
		opts, args = getopt.getopt(argv,"h:p:c:",["hhost=", "pport=", "ccount="])
	except getopt.GetoptError:
		print 'ClientClass.py -h <host> -p <port> -c <count>'
		sys.exit(2)

	if len(opts) == 0 and len(args) == 0:
		print 'Please enter the options like...'
		print 'ClientClass.py -h <host> -p <port> -c <count>'
		sys.exit(2)
	
	for opt, arg in opts:
		if opt in ("-h", "--hhost"):
			sHost = arg
		elif opt in ("-p", "--pport"):
			iPort = arg
		elif opt in ("-c", "--ccount"):
			iCount = arg

	if len(sHost) <= 0 or iPort <= 0 or iCount <= 0:
		bContinue = False
	if bContinue == True:
		bContinue = iPort.isdigit()
		if bContinue == False:
			print 'Port must be a number!'

	if bContinue == True:
		bContinue = iCount.isdigit()
		if bContinue == False:
			print 'Count must be a number!'
	

	if bContinue == False:
		print 'Input Error!'
		print 'Please use the command like < ClientClass.py -h <host> -p <port> -c <count> >'
		sys.exit(2)
	if bContinue == True:
		pCConnectionAcceptor = CConnectionAcceptor(sHost, int(iPort), int(iCount))
		if pCConnectionAcceptor.Connect() == True:
			pCConnectionAcceptor.Communicate()
		del pCConnectionAcceptor

		
if __name__ == '__main__':
	main(sys.argv[1:])
