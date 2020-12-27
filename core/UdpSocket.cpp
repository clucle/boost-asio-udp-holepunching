/*****************************************************************//**
 * \file   UdpSocket.cpp
 * \brief  
 * 
 * \author clucle
 * \date   December 2020
 *********************************************************************/


#include "UdpSocket.h"


UdpSocket::UdpSocket(
    boost::asio::io_context& ioContext,
    const std::string& host,
    const std::string& port ) :
    m_ioContext( ioContext ),
    m_socket( ioContext, udp::endpoint( udp::v4(), 0 ) ),
    m_data(),
    m_dataLength( 0 )
{
    udp::resolver resolver( m_ioContext );
    m_endpoints = resolver.resolve( udp::v4(), host, port );
};

void UdpSocket::SendTo( const char* data, size_t size )
{
    m_socket.send_to(
        boost::asio::buffer( data, size ), *m_endpoints.begin() );
}

void UdpSocket::RecvFrom()
{
    udp::endpoint senderEndpoint;
    m_dataLength = m_socket.receive_from(
        boost::asio::buffer( m_data, max_length ), senderEndpoint );
}


const char* UdpSocket::GetData() const
{
    return m_data;
}

size_t UdpSocket::GetDataLength()
{
    return m_dataLength;
}
