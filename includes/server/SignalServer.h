/*****************************************************************//**
 * \file   SignalServer.h
 * \brief  
 * 
 * \author clucle
 * \date   December 2020
 *********************************************************************/


#pragma once

#include "network/NetworkMessage.h"
#include "network/TcpConnector.h"


class SignalServer
{
public:
	typedef boost::asio::ip::tcp tcp;

public:
	SignalServer( tcp::socket socket );

	void OnRead( NetworkMessage& networkMessage, TcpConnectorPtr tcpConnector );

private:
	TcpConnectorPtr m_tcpConnector;
};

typedef std::shared_ptr< SignalServer > SignalServerPtr;
