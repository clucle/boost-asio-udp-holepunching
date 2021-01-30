/*****************************************************************//**
 * \file   TcpUser.h
 * \brief  
 * 
 * \author wjden
 * \date   January 2021
 *********************************************************************/


#pragma once


#include <memory>
#include <mutex>
#include <boost/asio.hpp>
#include "network/TcpClient.h"
#include "network/UdpSocket.h"


class TcpUser
	: public std::enable_shared_from_this< TcpUser >
{
public:
	typedef std::vector< UdpSocket > OtherUserVector;

public:
	TcpUser(
		boost::asio::io_context& ioContext,
		const tcp::resolver::results_type& endpoints );

	void Write( const NetworkMessage& msg );

	void Close();

	void OnRead( NetworkMessage& networkMessage );

	void Broadcast( std::string msg );

private:
	boost::asio::io_context& m_ioContext;

	TcpClientPtr m_tcpClient;

	OtherUserVector m_others;

	std::mutex m_othersMutex;
};
