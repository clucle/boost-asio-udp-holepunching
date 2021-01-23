/*****************************************************************//**
 * \file   SignalServer.cpp
 * \brief  
 * 
 * \author clucle
 * \date   December 2020
 *********************************************************************/


#include "SignalServer.h"
#include <functional>


SignalServer::SignalServer( tcp::socket socket ) :
	m_tcpConnector( std::make_shared<TcpConnector>( std::move( socket ) ) )
{
	m_tcpConnector->SetReadCallback(
		std::bind( &SignalServer::OnRead, this, std::placeholders::_1, m_tcpConnector )
	);

	m_tcpConnector->OnAccept();
}

void SignalServer::OnRead( NetworkMessage& networkMessage, TcpConnectorPtr tcpConnector )
{
	networkMessage.ReadyToRead();
	std::cout << networkMessage.ReadString() << '\n';
	
	tcpConnector->Write( networkMessage );
}