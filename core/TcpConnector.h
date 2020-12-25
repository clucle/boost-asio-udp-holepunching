/*****************************************************************//**
 * \file   TcpConnector.h
 * \brief  
 * 
 * \author clucle
 * \date   December 2020
 *********************************************************************/


#pragma once

#define BOOST_ALL_NO_LIB

#include <iostream>
#include <boost/asio.hpp>
#include "PacketBuffer.h"


class TcpConnector
	: public std::enable_shared_from_this< TcpConnector >
{
public:
	typedef boost::asio::ip::tcp tcp;

public:
	TcpConnector( tcp::socket socket );

	void OnAccept();

public:
	void SetReadCallback( std::function< void( void ) > readCallback );

private:
	void _ReadHeader();

	void _ReadBody();

	void _OnRead();

private:
	tcp::socket m_socket;

	std::function< void( void ) > m_readCallback;

	PacketBuffer buffer;
};

typedef std::shared_ptr< TcpConnector > TcpConnectorPtr;
