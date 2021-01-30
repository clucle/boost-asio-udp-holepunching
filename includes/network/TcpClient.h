/*****************************************************************//**
 * \file   TcpClient.h
 * \brief  
 * 
 * \author clucle
 * \date   January 2021
 *********************************************************************/


#pragma once

#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include "NetworkMessage.h"

using boost::asio::ip::tcp;

typedef std::deque<NetworkMessage> NetworkMessageDeque;

class TcpClient
    : public std::enable_shared_from_this< TcpClient >
{
public:
    TcpClient(
        boost::asio::io_context& ioContext,
        const tcp::resolver::results_type& endpoints );

    void Write( const NetworkMessage& msg );

    void Close();

    void SetReadCallback( std::function< void( NetworkMessage& ) > readCallback )
    {
        m_readCallback = readCallback;
    }

private:
    void _Connect( const tcp::resolver::results_type& endpoints );

    void _ReadHeader();

    void _ReadBody();

    void _Write();

private:
    boost::asio::io_context& m_ioContext;
    tcp::socket m_socket;
    NetworkMessage m_networkMessage;
    NetworkMessageDeque m_networkMessageDeque;

    std::function< void( NetworkMessage& ) > m_readCallback;
};

typedef std::shared_ptr< TcpClient > TcpClientPtr;
