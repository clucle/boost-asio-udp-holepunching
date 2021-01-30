/*****************************************************************//**
 * \file   TcpUser.cpp
 * \brief  
 * 
 * \author wjden
 * \date   January 2021
 *********************************************************************/


#include "TcpUser.h"
#include "network/UdpSocket.h"
#include "server/SignalServerProtocol.h"


static std::string GetIPString( UInt32 ip )
{
	unsigned char bytes[ 4 ];
	bytes[ 0 ] = ip & 0xFF;
	bytes[ 1 ] = ( ip >> 8 ) & 0xFF;
	bytes[ 2 ] = ( ip >> 16 ) & 0xFF;
	bytes[ 3 ] = ( ip >> 24 ) & 0xFF;

	int ipString[ 4 ] = { bytes[ 3 ], bytes[ 2 ], bytes[ 1 ], bytes[ 0 ] };
	return
		std::to_string( ipString[ 0 ] ) + "." +
		std::to_string( ipString[ 1 ] ) + "." +
		std::to_string( ipString[ 2 ] ) + "." +
		std::to_string( ipString[ 3 ] );
}

TcpUser::TcpUser(
	boost::asio::io_context& ioContext,
	const tcp::resolver::results_type& endpoints ) :
	m_ioContext( ioContext ),
	m_tcpClient( std::make_shared< TcpClient >( ioContext, endpoints ) )
{
	m_tcpClient->SetReadCallback(
		std::bind( &TcpUser::OnRead, this, std::placeholders::_1 )
	);
}

void TcpUser::Write( const NetworkMessage& msg )
{
	m_tcpClient->Write( msg );
}

void TcpUser::Close()
{
	m_tcpClient->Close();
}

void TcpUser::OnRead( NetworkMessage& networkMessage )
{
	networkMessage.ReadyToRead();

	ProtocolId protocol = networkMessage.Read< ProtocolId >();

	switch ( protocol )
	{
	case ProtocolId::ProtocolUserAddressNotify:
		{
			UInt32 ip = networkMessage.Read< UInt32 >();
			UInt32 port = networkMessage.Read< UInt16 >();

			//ip port Ãß°¡
			std::string ipString = GetIPString( ip );
			std::string portString = std::to_string( port );

			UdpSocket udpSocket( m_ioContext, ipString, portString );

			m_othersMutex.lock();
			m_others.push_back( std::move( udpSocket ) );
			m_othersMutex.unlock();
		}
		break;
	default:
		break;
	}
}

void TcpUser::Broadcast( std::string msg )
{
	m_othersMutex.lock();
	for ( auto& other : m_others )
	{
		std::cout << "Send To Other\n";
		other.SendTo( msg.c_str(), msg.size() );
	}
	m_othersMutex.unlock();
}
