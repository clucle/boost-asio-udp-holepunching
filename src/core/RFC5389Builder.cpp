/*****************************************************************//**
 * \file   RFC5389Builder.cpp
 * \brief  
 * 
 * \author wjden
 * \date   December 2020
 *********************************************************************/


#include "RFC5389Builder.h"
#include <bitset>
#include <iostream>

#ifdef __linux__ 
#include <netinet/in.h>
#elif _WIN32
#include <winsock.h>
#else

#endif


RFC5389Builder::RFC5389Builder() :
	messageClass( 0 ),
	messageLength( 0 ),
	magicCookie( 0x2112A442 ),
	transactionId()
{
	_GenerateRandomTransactionId();
}

RFC5389 RFC5389Builder::Build()
{
	RFC5389 rfc5389 = { 0, };

	UInt16 messageTypeField = 0x0001; // only binding method
	messageTypeField |= ( messageClass & 0x02 ) << 7;
	messageTypeField |= ( messageClass & 0x01 ) << 4;

	UInt16 messageTypeFieldBigEndian = htons( messageTypeField );
	UInt16 messageLengthBigEndian = htons( messageLength );
	UInt32 magicCookieBigEndian = htonl( magicCookie );

	memcpy( &rfc5389.messageType,   &messageTypeFieldBigEndian, 2 );
	memcpy( &rfc5389.messageLength, &messageLengthBigEndian,    2 );
	memcpy( &rfc5389.magicCookie,   &magicCookieBigEndian,      4 );
	memcpy( &rfc5389.transactionId, transactionId,              TRANSACTION_ID_LENGTH );

	return rfc5389;
}

void RFC5389Builder::SetMessageClass( STUN_MESSAGE_CLASS stunMessageClass )
{
	messageClass = static_cast< UInt16 >( stunMessageClass );
}

void RFC5389Builder::_GenerateRandomTransactionId()
{
	for ( int idx = 0; idx < TRANSACTION_ID_LENGTH; idx++ )
	{
		transactionId[ idx ] = rand() % 256;
	}
}