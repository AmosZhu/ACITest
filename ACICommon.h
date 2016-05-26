/*
 * ACICommon.h
 *
 *  Created on: May 23, 2016
 *      Author: amos
 */

#ifndef ACICOMMON_H_
#define ACICOMMON_H_

/*************************************************************************
 *
 * 						Type redefinition
 *
 ************************************************************************/
typedef unsigned char ACU8;
typedef unsigned short ACU16;
typedef unsigned long ACU32;
typedef unsigned long long ACU64;

typedef char AC8;
typedef short AC16;
typedef long AC32;
typedef long long AC64;

/************************************************************************
 *
 * 						Message Structure
 *
 ************************************************************************/

typedef struct ACI_MESSSAGE_HEADER {
	ACU16 Header_Key;
	ACU16 Size_Of_Header;
	ACU16 Size_Of_Message;
	ACU16 Function_Code;
} aci_msg_header;

typedef struct ACI_MESSSAGE_BODY {
	ACU16 Msg_Type;
	ACU16 Number_Of_Parameters;
	ACU8 Parameters[50];
} aci_msg_body;

typedef struct ACI_MESSSAGE_PACKAGE {
	aci_msg_header header;
	aci_msg_body message;
} aci_msg_pack;

/************************************************************************
 *
 * 						Q Message Definition
 *
 ************************************************************************/
#define ACIQ_PRI_FORMATA 0x00
#define ACIQ_PRI_FORMATB 0x80

#define ACIQ_CODE_IKEYENABLE 0x0001     //ikey include in format B
#define ACIQ_CODE_OMTRENABLE 0x0002		//Start order with OM trace enabled
#define ACIO_CODE_OGDBENABLE 0x0004		//Start order in Debug mode

struct q_MSG {
	ACU8 trpstr;
	ACU8 pri;
	union {
		struct {
			ACU16 para[3];
		} f_A;
		struct {
			ACU16 code;
			ACU16 ikey;
			ACU16 para[3];
		} f_B;

	} _format;

#define qapara  _format.f_A.para
#define qcode  _format.f_B.code
#define qikey  _format.f_B.ikey
#define qbpara  _format.f_B.para
}__attribute__((__packed__));

/************************************************************************
 *
 * 						S Message Definition
 *
 ************************************************************************/
enum ACIS_ORDERSTATUS {
	ACIS_OS_NOTUSED = 1,
	ACIS_OS_PENDING,
	ACIS_OS_TRANSITORY,
	ACIS_OS_TRANSITORY2,
	ACIS_OS_WAITVEHICLE,
	ACIS_OS_TRANSITORY3,
	ACIS_OS_MOVEVEHICLE,
	ACIS_OS_MAX
};

struct s_MSG {
	ACU16 index;
	ACU8 trpstr;
	ACU8 orderstatus;
	ACU16 magic;
	ACU16 magic_2;
	ACU8 carno;
	ACU8 spare;
	ACU16 carstat;
	ACU16 carstn;
}__attribute__((__packed__));

/************************************************************************
 *
 * 						B Message Definition
 *
 ************************************************************************/
struct b_MSG {
	ACU16 index;
	ACU8 trpstr;
	ACU8 status;
	ACU8 parno;
	ACU8 spare;
	ACU16 ikey;
};

/************************************************************************
 *
 * 						M Message Definition
 *
 ************************************************************************/
struct m_MSG {
	ACU16 index;
	ACU8 function;
	ACU8 parno;
	ACU16 p0val;
}__attribute__((__packed__));

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
