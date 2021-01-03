/*****************************************************************//**
 * \file   TcpAcceptor.h
 * \brief  
 * 
 * \author clucle
 * \date   December 2020
 *********************************************************************/


#pragma once

#include <boost/asio.hpp>


class TcpAcceptor
{
public:
typedef boost::asio::ip::tcp tcp;

public:
    TcpAcceptor( 
        boost::asio::io_context& ioContext,
        const tcp::endpoint& endpoint );

protected:
    virtual void Accept() = 0;

protected:
    tcp::acceptor m_acceptor;
};

typedef std::shared_ptr< TcpAcceptor > TcpAcceptorPtr;