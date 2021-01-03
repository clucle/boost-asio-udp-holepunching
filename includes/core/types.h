/*****************************************************************//**
 * \file   types.h
 * \brief  
 * 
 * \author clucle
 * \date   December 2020
 *********************************************************************/


#pragma once

#include <cstdint>
#include "const.h"


typedef uint8_t UInt8;
typedef uint16_t UInt16;
typedef uint32_t UInt32;

//////////////////////////////////////////////////////////////////////
/// Stun Type
struct RFC5389
{
	UInt16 messageType;
	UInt16 messageLength;
	UInt32 magicCookie;
	UInt8 transactionId[ RFC5389_SIZE - 8 ];
};

const UInt16 STUN_ATTRIBUTES_TYPE_XOR_MAPPED_ADDRESS = 0x20;


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
