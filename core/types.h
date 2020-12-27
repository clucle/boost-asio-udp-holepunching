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
	UInt8 data[ RFC5389_SIZE ];
};

struct XOR_MAPPED_ADDRESS
{
	UInt8  pad;                        // 1
	UInt8  family;                     // 1
	UInt16 port;                       // 2
	UInt8  xAddress[ X_ADDRESS_SIZE ]; // 16
};
