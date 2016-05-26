/*
 * ACIClient.hpp
 *
 *  Created on: May 23, 2016
 *      Author: amos
 */

#ifndef ACICLIENT_HPP_
#define ACICLIENT_HPP_

#include <string>
#include "ACICommon.h"


class ACIClient {
public:
	ACIClient();
	ACIClient(const ACIClient& _obj);
	ACIClient(ACIClient&& _obj);
	~ACIClient();

public:
	bool Initalize();
	bool Connect(char* _ipAddr,int _port);
	bool Send(const aci_msg_body& _msg, int _numOfBytes);
	bool Read(aci_msg_body& _msg);
	bool Terminate();

public:
	static ACIClient* GetInstance(void);
	static aci_msg_t GetMessageType(const aci_msg_body& _msg);
	static bool SplitMessage(unsigned char* _buf,aci_msg_header* _header,aci_msg_body* _body,int _len);
	static std::string MsgInterpret(aci_msg_body& _body);

private:
	int m_fdSock; //Socket description
	int m_errno;
	static ACIClient* m_hInstatnce;
};

#endif /* ACICLIENT_HPP_ */
