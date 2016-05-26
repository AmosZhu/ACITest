/*
 * ACIException.cpp
 *
 *  Created on: May 25, 2016
 *      Author: amos
 */

#include <sstream>
#include <errno.h>
#include <string.h>
#include "ACIException.hpp"

ACIException::ACIException() {
}

ACIException::ACIException(int _errno) {
	std::ostringstream stringStream;
	stringStream << strerror(_errno);
	std::string errorInfo = stringStream.str();
}

ACIException::ACIException(char* _reason) {
	std::ostringstream stringStream;
	stringStream<<_reason;
	m_reason=stringStream.str();
}

std::string ACIException::Error() {
	return m_reason;
}

