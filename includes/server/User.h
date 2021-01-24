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
#include "SignalServerProtocol.h"


class User
	: public std::enable_shared_from_this< User >
{
public:
	typedef boost::asio::ip::tcp tcp;

public:
	User( tcp::socket socket );

	void Initialize();

	void SetId( UInt32 id );

	UInt32 GetId();

	void OnRead( NetworkMessage& networkMessage );

	void Write( NetworkMessage& networkMessage );

private:
	TcpConnectorPtr m_tcpConnector;

	UInt32 m_id;

	SignalServerProtocolPtr m_protocol;
};

typedef std::shared_ptr< User > UserPtr;

typedef std::weak_ptr< User > UserWeakPtr;