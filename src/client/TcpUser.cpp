/*****************************************************************//**
 * \file   TcpUser.cpp
 * \brief  
 * 
 * \author wjden
 * \date   January 2021
 *********************************************************************/


#include "TcpUser.h"


TcpUser::TcpUser(
	boost::asio::io_context& ioContext,
	const tcp::resolver::results_type& endpoints ) :
	m_tcpClient( std::make_shared< TcpClient >( ioContext, endpoints ) )
{
	m_tcpClient->SetReadCallback(
		std::bind( &TcpUser::OnRead, this, std::placeholders::_1 )
	);
}

void TcpUser::Write( const NetworkMessage& msg )
{
	m_tcpClient->Write( msg );
}

void TcpUser::Close()
{
	m_tcpClient->Close();
}

void TcpUser::OnRead( NetworkMessage& networkMessage )
{
	std::cout << "HELLO\n";
}
