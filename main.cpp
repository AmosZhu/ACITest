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
#include "ACIException.hpp"

#define MAXLINE 4096
#define PORTNUM 30001

void Prompt() {
	std::cout << "/****************************************************************************" << std::endl;
	std::cout << " *" << std::endl;
	std::cout << " *		1.Start a new order" << std::endl;
	std::cout << " *		2.Cancel an order" << std::endl;
	std::cout << " *		9.quit" << std::endl;
	std::cout << " *" << std::endl;
	std::cout << " ***************************************************************************/" << std::endl;
	std::cout << "Your Choice: ";
}

bool StartNewOrder() {

	try {
		ACIClient* client = ACIClient::GetInstance();
		/*
		 * Construct a 'q' Message to start an order
		 */
		aci_msg_body msg;
		q_MSG qmsg;
		memset(&qmsg, 0, sizeof(q_MSG));
		msg.Msg_Type = LS2MS(MESSAGE_QCODE);
		msg.Number_Of_Parameters = LS2MS(sizeof(q_MSG));
		qmsg.trpstr = 1;
		qmsg.pri = ACIQ_PRI_FORMATB;
		qmsg.qcode = LS2MS(ACIQ_CODE_IKEYENABLE);
		qmsg.qikey = LS2MS(2);
		qmsg.qbpara[0] = LS2MS(1); //ikey in parameter
		qmsg.qbpara[1] = LS2MS(1); //Fetch station number
		qmsg.qbpara[2] = LS2MS(3); //Deliver station number

		memcpy(msg.Parameters, &qmsg, sizeof(q_MSG));
		/*
		 * Send to client
		 */
		client->Send(msg, sizeof(msg));

		/*
		 * Read from system manager
		 */
		while (client->Read(msg)) {
			std::cout << ACIClient::MsgInterpret(msg) << std::endl;
			if (client->GetMessageType(msg) == MESSAGE_S) {
				/*
				 * Construct a 'm' message response
				 */
				s_MSG smsg;
				m_MSG mmsg;
				memcpy(&smsg, msg.Parameters, sizeof(smsg));
				mmsg.index = smsg.index;
				mmsg.function = 1;
				mmsg.parno = 18;
				mmsg.p0val = smsg.magic;

				msg.Msg_Type = LS2MS(MESSAGE_MCODE);
				msg.Number_Of_Parameters = LS2MS(sizeof(m_MSG));
				memcpy(msg.Parameters, &mmsg, sizeof(m_MSG));
				client->Send(msg, sizeof(msg));

			}
		}
	} catch (ACIException& e) {
		std::cout << e.Error() << std::endl;
		return false;
	}

	return true;
}

int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("usage: ./client <ipaddress>\n");
		exit(0);
	}
	int selection;
	bool quit = false;

	ACIClient* client = ACIClient::GetInstance();
	try {
		client->Initalize();
		client->Connect(argv[1], PORTNUM);
	} catch (ACIException& e) {
		std::cout << e.Error() << std::endl;
		return -1;
	}

	/*
	 * Output a prompt for user
	 */
	Prompt();

	while (!quit) {
		std::cin >> selection;
		switch (selection) {
		case 1: {
			if (!StartNewOrder())
				quit = true;
		}
		break;
		case 9: {
			quit = true;
		}
		break;
		}
	}

	return 0;
}
