/*****************************************************************//**
 * \file   RFC5389Builder.h
 * \brief  
 * 
 * \author clucle
 * \date   December 2020
 *********************************************************************/


#pragma once

#include "types.h"


class RFC5389Builder
{
public:
	enum { TRANSACTION_ID_LENGTH = 16 };

public:
	RFC5389Builder();

	RFC5389 Build();

private:
	void _GenerateRandomTransactionId();

private:
	UInt16 messageType;
	UInt16 messageLength;
	UInt32 magicCookie;
	UInt8  transactionId[ TRANSACTION_ID_LENGTH ];
};

