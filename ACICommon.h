/*
 * ACICommon.h
 *
 *  Created on: May 23, 2016
 *      Author: amos
 */

#ifndef ACICOMMON_H_
#define ACICOMMON_H_

/************************************************************************
 *
 * 						Message Structure
 *
 ************************************************************************/

typedef struct ACI_MESSSAGE_HEADER {
	unsigned short Header_Key;
	unsigned short Size_Of_Header;
	unsigned short Size_Of_Message;
	unsigned short Function_Code;
} aci_msg_header;

typedef struct ACI_MESSSAGE_BODY {
	unsigned short Msg_Type;
	unsigned short Number_Of_Parameters;
	unsigned char Parameters[50];
} aci_msg_body;

typedef struct ACI_MESSSAGE_PACKAGE {
	aci_msg_header header;
	aci_msg_body message;
} aci_msg_pack;

struct q_MSG {
	unsigned char trpsno;
	unsigned char pri;
	unsigned short para[5];
};

struct s_MSG {
	unsigned short index;
	unsigned char trpsno;
	unsigned char ordstat;
	unsigned short magic;

	unsigned short magic_2;
	unsigned char carno;
	unsigned char spare;
	unsigned short status;
	unsigned short station;
	unsigned short magic_3;
};

struct b_MSG {
	unsigned short index;
	unsigned char trpsno;
	unsigned char status;
	unsigned char poffs;

	unsigned char sp;
	unsigned short ikey;
};

typedef enum ACI_MESSAGE_TYPE {
	MESSAGE_B = 0, MESSAGE_M, MESSAGE_Q, MESSAGE_S, MESSAGE_UNKNOWN,
} aci_msg_t;

/************************************************************************
 *
 * 						Global Macro
 *
 ************************************************************************/
#define HEADER_LENGTH (8)

/*
 * 	Convert between least significant bit and most significant bit
 */
#define LS2MS(a) (((a>>8)&0x00FF)|(((a<<8)&0xFF00)))
#define MS2LS(a) (((a>>8)&0x00FF)|(((a<<8)&0xFF00)))

#define MESSAGE_BCODE (0x0062)
#define MESSAGE_MCODE (0x006D)
#define MESSAGE_QCODE (0x0071)
#define MESSAGE_SCODE (0x0073)

#endif /* ACICOMMON_H_ */
