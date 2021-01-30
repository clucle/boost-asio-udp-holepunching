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
#include <network/TcpClient.h>


class TcpUser
	: public std::enable_shared_from_this< TcpUser >
{
public:
	typedef std::vector< std::pair< UInt32, UInt16 > > OtherUserVector;

public:
	TcpUser(
		boost::asio::io_context& ioContext,
		const tcp::resolver::results_type& endpoints );

	void Write( const NetworkMessage& msg );

	void Close();

	void OnRead( NetworkMessage& networkMessage );

private:
	TcpClientPtr m_tcpClient;

	OtherUserVector others;

	std::mutex m_othersMutex;
};
