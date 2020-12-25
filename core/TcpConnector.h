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
	void _Read();

	void _OnRead();

private:
	tcp::socket m_socket;

	std::function< void( void ) > m_readCallback;

	enum { MAX_LENGTH = 1024 };
	char m_data[ MAX_LENGTH ];
};

typedef std::shared_ptr< TcpConnector > TcpConnectorPtr;
