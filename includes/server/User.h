/*****************************************************************//**
 * \file   User.h
 * \brief  
 * 
 * \author clucle
 * \date   December 2020
 *********************************************************************/


#pragma once

#include "network/NetworkMessage.h"
#include "network/TcpConnector.h"


class User
{
public:
	typedef boost::asio::ip::tcp tcp;

public:
	User( tcp::socket socket );

	void OnRead( NetworkMessage& networkMessage, TcpConnectorPtr tcpConnector );

private:
	TcpConnectorPtr m_tcpConnector;
};

typedef std::shared_ptr< User > UserPtr;
