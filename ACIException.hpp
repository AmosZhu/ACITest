/*
 * ACIException.hpp
 *
 *  Created on: May 25, 2016
 *      Author: amos
 */

#ifndef ACIEXCEPTION_HPP_
#define ACIEXCEPTION_HPP_

class ACIException {
public:
	ACIException();
	ACIException(int _errno);
	ACIException(char* _reason);

	std::string Error();

private:
	std::string m_reason;
};

#endif /* ACIEXCEPTION_HPP_ */
