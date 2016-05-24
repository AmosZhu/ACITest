/*
 * main.cpp
 *
 *  Created on: May 23, 2016
 *      Author: amos
 */

#include <iostream>
#include <string.h>
#include "ACICommon.h"
#include "ACIClient.hpp"

#define MAXLINE 4096
#define PORTNUM 30001

int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("usage: ./client <ipaddress>\n");
		exit(0);
	}

	ACIClient* client = new ACIClient();
	if (!client->Initalize()) {
		std::cout << client->Error() << std::endl;
		return -1;
	}

	if (!client->Connect(argv[1], PORTNUM)) {
		std::cout << client->Error() << std::endl;
		return -1;
	}

	/*
	 * Construct a 'q' Message to start an order
	 */
	aci_msg_body msg;
	q_MSG qmsg;
	msg.Msg_Type = LS2MS(MESSAGE_QCODE);
	msg.Number_Of_Parameters = LS2MS(sizeof(q_MSG));
	qmsg.trpsno=0x000f&1;
	qmsg.pri=0x000f&1;
	qmsg.para[0] = LS2MS(1); //Reserved, fixed value 1
	qmsg.para[1] = LS2MS(1); // iKey
	qmsg.para[2] = LS2MS(1); //Fetch station number
	qmsg.para[3] = LS2MS(3); //Deliver station number

	memcpy(msg.Parameters, &qmsg, sizeof(q_MSG));
	/*
	 * Send to client
	 */
	if (!client->Send(msg, sizeof(msg))) {
		std::cout << client->Error() << std::endl;
		return -1;
	}

	/*
	 * Read from system manager
	 */
	while(client->Read(msg))
	{
		std::cout<<ACIClient::MsgInterpret(msg)<<std::endl;
	}


	return 0;
}
