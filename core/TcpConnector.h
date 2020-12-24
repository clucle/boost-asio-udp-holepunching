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

private:
	void _Read();

	void _OnRead();

protected:
	virtual void OnRead();

private:
	tcp::socket m_socket;

	enum { MAX_LENGTH = 1024 };
	char m_data[ MAX_LENGTH ];
};

typedef std::shared_ptr< TcpConnector > TcpConnectorPtr;
