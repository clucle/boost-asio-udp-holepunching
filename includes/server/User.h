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
	User();

	User( tcp::socket socket );

	void Initialize();

	void SetId( UInt32 id );

	void SetRegistered( bool registered ) { m_registered = registered; }

	void SetIP( UInt32 ip ) { m_ip = ip; }

	void SetPort( UInt16 port ) { m_port = port; }

	UInt32 GetId();

	bool GetRegistered() { return m_registered; }

	UInt32 GetIP() { return  m_ip; }

	UInt16 GetPort() { return m_port; }

	void OnRead( NetworkMessage& networkMessage );

	void Write( NetworkMessage& networkMessage );

private:
	TcpConnectorPtr m_tcpConnector;

	UInt32 m_id;

	bool m_registered;
	UInt32 m_ip;
	UInt16 m_port;

	SignalServerProtocolPtr m_protocol;
};

typedef std::shared_ptr< User > UserPtr;

typedef std::weak_ptr< User > UserWeakPtr;