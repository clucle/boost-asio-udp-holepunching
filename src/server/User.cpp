/*****************************************************************//**
 * \file   User.cpp
 * \brief  
 * 
 * \author clucle
 * \date   December 2020
 *********************************************************************/


#include "User.h"
#include <functional>


User::User( tcp::socket socket ) :
	m_tcpConnector( std::make_shared<TcpConnector>( std::move( socket ) ) )
{

	std::cout << this << '\n';
	std::cout << &(this->m_tcpConnector) << '\n';

	m_tcpConnector->SetReadCallback(
		std::bind( &User::OnRead, this, std::placeholders::_1, m_tcpConnector )
	);

	m_tcpConnector->OnAccept();
}

void User::SetId( UInt32 id )
{
	m_id = id;
}

UInt32 User::GetId()
{
	return m_id;
}

void User::OnRead( NetworkMessage& networkMessage, TcpConnectorPtr tcpConnector )
{
	std::cout << this << '\n';
	std::cout << &(this->m_tcpConnector) << '\n';

	networkMessage.ReadyToRead();
	std::cout << networkMessage.ReadString() << '\n';
	
	tcpConnector->Write( networkMessage );
}