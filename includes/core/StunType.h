/*****************************************************************//**
 * \file   StunType.h
 * \brief  
 * 
 * \author clucle
 * \date   January 2021
 *********************************************************************/


#include "types.h"


enum { RFC5389_SIZE = 20 };
enum { RFC5389_TRANSACTION_ID_SIZE = 12 };
enum { X_ADDRESS_SIZE = 16 };

const UInt16 STUN_ATTRIBUTES_TYPE_XOR_MAPPED_ADDRESS = 0x20;

struct RFC5389
{
	UInt16 messageType;
	UInt16 messageLength;
	UInt32 magicCookie;
	UInt8 transactionId[ RFC5389_TRANSACTION_ID_SIZE ];
};

enum class STUN_MESSAGE_CLASS : UInt16
{
	REQUEST = 0x00,
	INDICATION = 0x01,
	RESPONSE_SUCCESS = 0x02,
	RESPONSE_ERROR = 0x03
};

enum STUN_FAMILY : UInt8
{
	IPv4 = 0x1,
	IPv6 = 0x2,

	MAX
};

struct STUN_ATTRIBUTES
{
	UInt16 type;
	UInt16 length;
};

struct XOR_MAPPED_ADDRESS
{
	UInt8  pad;                        // 1
	UInt8  family;                     // 1
	UInt16 port;                       // 2
	UInt8  xAddress[ X_ADDRESS_SIZE ]; // 16
};
