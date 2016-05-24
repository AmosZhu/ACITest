/*
 * ACIClient.cpp
 *
 *  Created on: May 23, 2016
 *      Author: amos
 */

#include "ACIClient.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <sstream>
#include <string.h>
#include "ACICommon.h"

#define BUFLENGTH (1024)

ACIClient::ACIClient() {
	m_errno = 0;
	m_fdSock = -1;
}

ACIClient::ACIClient(const ACIClient& _obj) {
	m_errno = 0;
}

ACIClient::ACIClient(ACIClient&& _obj) {
	m_errno = 0;
}

ACIClient::~ACIClient() {
	m_errno = 0;

}

bool ACIClient::Initalize() {
	if ((m_fdSock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		m_errno = errno;
		return false;
	}

	return true;
}

bool ACIClient::Connect(char* _ipAddr, int _port) {
	bool res = true;
	struct sockaddr_in servaddr;

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(_port);
	if (inet_pton(AF_INET, _ipAddr, &servaddr.sin_addr) <= 0) {
		m_errno = errno;
		return false;
	}

	if (connect(m_fdSock, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0) {
		m_errno = errno;
		return false;
	}

	return res;
}

bool ACIClient::Send(const aci_msg_body& _msg, int _numOfBytes) {

	unsigned short wtemp = 0;
	aci_msg_pack sendMsg;
	memset(&sendMsg, 0, sizeof(aci_msg_pack));

	/*
	 * Encapsulate the header
	 */

	sendMsg.header.Header_Key = LS2MS(0x87CD);
	sendMsg.header.Size_Of_Header = LS2MS(0x0008);
	wtemp = _numOfBytes + 4;
	sendMsg.header.Size_Of_Message = LS2MS(wtemp);
	sendMsg.header.Function_Code = LS2MS(0x0001);

	memcpy(&sendMsg.message, &_msg, wtemp);

	if (send(m_fdSock, (void*) &sendMsg, sizeof(sendMsg), 0) < 0) {
		m_errno = errno;
		return false;
	}

	return true;

}

bool ACIClient::Read(aci_msg_body& _msg) {
	unsigned char buff[BUFLENGTH];
	int len;
	if ((len=recv(m_fdSock, buff, BUFLENGTH, 0)) < 0) {
		m_errno = errno;
		return false;
	}

	return SplitMessage(buff, nullptr, &_msg,len);

}

std::string ACIClient::Error() {
	std::ostringstream stringStream;
	stringStream << strerror(errno);
	std::string errorInfo = stringStream.str();
	return errorInfo;
}

bool ACIClient::SplitMessage(unsigned char* _buf, aci_msg_header* _header,
		aci_msg_body* _body,int _len) {
	if (_buf == nullptr)
		return false;

	if (_header != nullptr)
		memcpy(&_header, _buf, HEADER_LENGTH);

	if (_body != nullptr)
		memcpy(_body, _buf + HEADER_LENGTH, sizeof(aci_msg_body));

	return true;
}

aci_msg_t ACIClient::GetMessageType(const aci_msg_body& _msg) {
	aci_msg_t msgType;

	/*
	 * Convert from HSB to LSB first
	 */

	switch (MS2LS(_msg.Msg_Type)) {
	case MESSAGE_BCODE: {
		msgType = MESSAGE_B;
	}
		break;
	case MESSAGE_MCODE: {
		msgType = MESSAGE_M;
	}
		break;
	case MESSAGE_QCODE: {
		msgType = MESSAGE_Q;
	}
		break;
	case MESSAGE_SCODE: {
		msgType = MESSAGE_S;
	}
		break;
	default: {
		msgType = MESSAGE_UNKNOWN;
	}
		break;
	}
	return msgType;
}

std::string ACIClient::MsgInterpret(aci_msg_body& _msg) {
	std::string res;
	std::ostringstream stringStream;

	switch (MS2LS(_msg.Msg_Type)) {
	case MESSAGE_BCODE: {
		res = "b\t";
	}
		break;
	case MESSAGE_MCODE: {
		res = "m\t";
	}
		break;
	case MESSAGE_QCODE: {
		res = "q\t";
	}
		break;
	case MESSAGE_SCODE: {
		res = "s\t";
	}
		break;
	default: {
		res = "?\t";
	}
		break;
	}

	return res;
}
