/*****************************************************************//**
 * \file   SignalServer.cpp
 * \brief  
 * 
 * \author clucle
 * \date   December 2020
 *********************************************************************/


#include "SignalServer.h"


SignalServer::SignalServer( tcp::socket socket ) :
	TcpConnector( std::move( socket ) )
{
}

void SignalServer::OnRead()
{
	std::cout << "on read called" << '\n';
}
