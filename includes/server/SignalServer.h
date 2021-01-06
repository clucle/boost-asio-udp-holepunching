/*****************************************************************//**
 * \file   SignalServer.h
 * \brief  
 * 
 * \author clucle
 * \date   December 2020
 *********************************************************************/


#pragma once

#include "network/PacketBuffer.h"
#include "network/TcpConnector.h"


class SignalServer
{
public:
	typedef boost::asio::ip::tcp tcp;

public:
	SignalServer( tcp::socket socket );

	void OnRead( PacketBuffer& packetBuffer );

private:
	TcpConnectorPtr tcpConnector;
};

typedef std::shared_ptr< SignalServer > SignalServerPtr;
