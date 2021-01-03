/*****************************************************************//**
 * \file   SignalServer.cpp
 * \brief  
 * 
 * \author clucle
 * \date   December 2020
 *********************************************************************/


#include "SignalServer.h"


SignalServer::SignalServer( tcp::socket socket ) :
	tcpConnector( std::make_shared<TcpConnector>( std::move( socket ) ) )
{
	tcpConnector->SetReadCallback(
		std::bind( &SignalServer::OnRead, this, std::placeholders::_1 )
	);

	tcpConnector->OnAccept();
}

void SignalServer::OnRead( PacketBuffer& packetBuffer )
{
	tcpConnector->Write( packetBuffer );
}