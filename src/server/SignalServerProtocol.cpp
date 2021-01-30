/*****************************************************************//**
 * \file   SignalServerProtocol.cpp
 * \brief  
 * 
 * \author wjden
 * \date   January 2021
 *********************************************************************/


#include "SignalServerProtocol.h"
#include "User.h"

SignalServerProtocol::SignalServerProtocol( UserPtr user )
{
	m_user = user;
}

void SignalServerProtocol::OnRecvMessage( NetworkMessage networkMessage )
{
	networkMessage.ReadyToRead();
	
	ProtocolId protocol = networkMessage.Read< ProtocolId >();

	switch ( protocol )
	{
	case ProtocolId::ProtocolRequestAddress:
		{
			UInt32 ip   = networkMessage.Read< UInt32 >();
			UInt16 port = networkMessage.Read< UInt16 >();
			std::cout << "ip : " << ip << " port : " << port << '\n';
		}
		break;
	case ProtocolId::ProtocolRequestAddressResult:
		{
			std::cout << "check complete" << '\n';
		}
		break;
	}

	UserPtr user = m_user.lock();
	if ( user )
		user->Write( networkMessage );
}
