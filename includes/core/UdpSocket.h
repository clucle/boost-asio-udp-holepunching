/*****************************************************************//**
 * \file   UdpSocket.h
 * \brief  
 * 
 * \author clucle
 * \date   December 2020
 *********************************************************************/


#pragma once

#include <string>
#include <boost/asio.hpp>


using boost::asio::ip::udp;

class UdpSocket
{
public:
    UdpSocket(
        boost::asio::io_context& ioContext,
        const std::string& host,
        const std::string& port );

    void SendTo( const char* data, size_t size );

    void RecvFrom();

    const char* GetData() const;

    size_t GetDataLength();

private:
    boost::asio::io_context& m_ioContext;
    udp::resolver::results_type m_endpoints;
    udp::socket m_socket;

    enum { max_length = 1024 };
    char m_data[ max_length ];
    size_t m_dataLength;
};

