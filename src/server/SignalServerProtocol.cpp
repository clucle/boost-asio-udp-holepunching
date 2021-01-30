/*****************************************************************//**
 * \file   SignalServerProtocol.cpp
 * \brief  
 * 
 * \author wjden
 * \date   January 2021
 *********************************************************************/


#include "SignalServerProtocol.h"
#include "User.h"
#include "UserManager.h"
#include "common/utils.h"
#include "network/UdpSocket.h"

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

			if ( ip == 0 || port == 0 ) break;

			std::thread chk( [ip, port]()
				{
					boost::asio::io_context ioContext;

					std::string ipString = GetIPString( ip );
					std::string portString = std::to_string( port );

					std::cout << "Request ip : " << ipString << " port : " << portString << '\n';

					UdpSocket udpSocket( ioContext, ipString, portString );
					udpSocket.SendTo( "chk", 3 );
				} );

			chk.join();

			UserPtr user = m_user.lock();
			user->SetIP( ip );
			user->SetPort( port );
		}
		break;
	case ProtocolId::ProtocolRequestAddressResult:
		{
			std::cout << "check complete" << '\n';
			UserPtr user = m_user.lock();
			UserManager::GetInstance().Register( user->GetId(), user->GetIP(), user->GetPort() );
		}
		break;
	}

	// UserPtr user = m_user.lock();
	// if ( user )
	// 	user->Write( networkMessage );
}
