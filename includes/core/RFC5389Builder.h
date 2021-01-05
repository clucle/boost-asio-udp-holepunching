/*****************************************************************//**
 * \file   RFC5389Builder.h
 * \brief  
 * 
 * \author clucle
 * \date   December 2020
 *********************************************************************/


#pragma once

#include "StunType.h"


class RFC5389Builder
{
public:
	enum { TRANSACTION_ID_LENGTH = 16 };

public:
	RFC5389Builder();

	RFC5389 Build();

	void SetMessageClass( STUN_MESSAGE_CLASS stunMessageClass );

private:
	void _GenerateRandomTransactionId();

private:
	      UInt16 messageClass;
	      UInt16 messageLength;
	const UInt32 magicCookie;
	      UInt8  transactionId[ TRANSACTION_ID_LENGTH ];
};

