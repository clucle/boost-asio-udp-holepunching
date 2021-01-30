/*****************************************************************//**
 * \file   main.cpp
 * \brief  
 * 
 * \author clucle
 * \date   December 2020
 *********************************************************************/


#include <iostream>
#include <boost/asio.hpp>
#include "TcpUser.h"
#include "core/RFC5389Builder.h"
#include "common/utils.h"
#include "network/UdpSocket.h"
#include "network/NetworkMessage.h"
#include "server/SignalServerProtocol.h"


int main( int argc, char* argv[] )
{
    try
    {
        // TCP 家南
        boost::asio::io_context ioContextTcp;

        using boost::asio::ip::tcp;
        tcp::resolver resolver( ioContextTcp );
        auto endpoints = resolver.resolve( "192.168.0.29", "12111" );
        TcpUser tcpClient( ioContextTcp, endpoints );

        std::thread t( [&ioContextTcp]() { ioContextTcp.run(); } );

        // UDP 家南
        boost::asio::io_context ioContext;
        const char* stunServerAddr = "stun.l.google.com";
        const char* stunServerPort = "19302";

        UdpSocket udpSocket( ioContext, stunServerAddr, stunServerPort );

        // build stun request packet
        RFC5389Builder rfcBuilder;
        RFC5389 rfc5389;
        char foo[ sizeof( struct RFC5389 ) ] = { 0, };

        rfcBuilder.SetMessageClass( STUN_MESSAGE_CLASS::REQUEST );
        rfc5389 = rfcBuilder.Build();
        
        // send stun request packet
        udpSocket.SendTo( (char*)( &rfc5389 ), sizeof( RFC5389 ) );

        // recv stun response packet
        udpSocket.RecvFrom();
        
        const char* packet = udpSocket.GetData();
        size_t packetSize = udpSocket.GetDataLength();

        const RFC5389* response =
            reinterpret_cast< const RFC5389* >( packet );

        UInt16 messageType   = ntohs( response->messageType );
        UInt16 messageLength = ntohs( response->messageLength );
        UInt32 magicCookie   = ntohl( response->magicCookie );

        // convert STUN ATTRIBUTES
        const STUN_ATTRIBUTES* attributes =
            reinterpret_cast< const STUN_ATTRIBUTES* >( packet + sizeof( RFC5389 ) );

        if ( !attributes )
        {
            std::cerr << "Convert Error\n";
            return 0;
        }

        UInt16 type   = ntohs( attributes->type );
        UInt16 length = ntohs( attributes->length );

        switch ( type )
        {
        case STUN_ATTRIBUTES_TYPE_XOR_MAPPED_ADDRESS:
            {
                const XOR_MAPPED_ADDRESS* xorMappedAddress =
                    reinterpret_cast< const XOR_MAPPED_ADDRESS* >(
                        packet + sizeof( RFC5389 ) + sizeof( STUN_ATTRIBUTES ) );

                UInt8  family = xorMappedAddress->family;
                UInt16 port   = ntohs( xorMappedAddress->port ) ^ ( magicCookie >> 16 );
                UInt8  address[ X_ADDRESS_SIZE ];

                switch ( family )
                {
                case STUN_FAMILY::IPv4:
                    {
                        memcpy( address, xorMappedAddress->xAddress, 4 );

                        UInt32 ip =
                            ( address[ 0 ] << 24 ) |
                            ( address[ 1 ] << 16 ) |
                            ( address[ 2 ] << 8 ) |
                            ( address[ 3 ] << 0 );
                        ip ^= magicCookie;
                        PrintIP( ip );

                        std::cout << std::dec << "port   : " << port << '\n';

                        NetworkMessage msg;
                        msg.Append( ProtocolId::ProtocolRequestAddress );
                        msg.Append( ip );
                        msg.Append( port );
                        msg.EncodeHeader();
                        tcpClient.Write( msg );

                        break;
                    }
                case STUN_FAMILY::IPv6:
                    {
                        memcpy( address, xorMappedAddress->xAddress, 16 );
                        break;
                    }
                default:
                    {
                        std::cerr << "Family Error\n";
                        return 0;
                    }
                }

                break;
            }

        }

        std::thread recvUdp( [ &udpSocket, &tcpClient ]() {
            while ( true )
            {
                udpSocket.RecvFrom();
                const char* packet = udpSocket.GetData();
                size_t packetSize = udpSocket.GetDataLength();

                std::string str( packet, packet + packetSize );

                if ( str == "chk" )
                {
                    NetworkMessage msg;
                    msg.Append( ProtocolId::ProtocolRequestAddressResult );
                    msg.EncodeHeader();
                    tcpClient.Write( msg );
                }
                else
                {
                    std::cout << str << '\n';
                }
            }
        } );

        // udp 烹脚且芭
        std::string str;
        while ( std::cin >> str )
        {
            tcpClient.Broadcast( str );
        }

        tcpClient.Close();
        t.join();


    }
    catch ( std::exception& e )
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}