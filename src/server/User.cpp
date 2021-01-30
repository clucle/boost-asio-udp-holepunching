/*****************************************************************//**
 * \file   User.cpp
 * \brief  
 * 
 * \author clucle
 * \date   December 2020
 *********************************************************************/


#include "User.h"
#include <functional>


User::User() :
	m_tcpConnector( nullptr ),
	m_id( 0 ),
	m_registered( false ),
	m_ip( 0 ),
	m_port( 0 )
{
}

User::User( tcp::socket socket ) :
	m_tcpConnector( std::make_shared<TcpConnector>( std::move( socket ) ) ),
	m_id( 0 ),
	m_registered( false ),
	m_ip( 0 ),
	m_port ( 0 )
{

	m_tcpConnector->SetReadCallback(
		std::bind( &User::OnRead, this, std::placeholders::_1 )
	);

	m_tcpConnector->OnAccept();
}

void User::Initialize()
{
	m_protocol = std::make_shared< SignalServerProtocol >( shared_from_this() );
}

void User::SetId( UInt32 id )
{
	m_id = id;
}

UInt32 User::GetId()
{
	return m_id;
}

void User::OnRead( NetworkMessage& networkMessage )
{
	m_protocol->OnRecvMessage( networkMessage );
}

void User::Write( NetworkMessage& networkMessage )
{
	if ( m_tcpConnector )
		m_tcpConnector->Write( networkMessage );
}
