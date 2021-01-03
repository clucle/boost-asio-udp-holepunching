/*****************************************************************//**
 * \file   main.cpp
 * \brief  
 * 
 * \author clucle
 * \date   December 2020
 *********************************************************************/


#include <iostream>
#include <boost/asio.hpp>
#include "core/UdpSocket.h"
#include "core/RFC5389Builder.h"


void print_ip( UInt32 ip )
{
    unsigned char bytes[ 4 ];
    bytes[ 0 ] = ip & 0xFF;
    bytes[ 1 ] = ( ip >> 8 ) & 0xFF;
    bytes[ 2 ] = ( ip >> 16 ) & 0xFF;
    bytes[ 3 ] = ( ip >> 24 ) & 0xFF;
    printf( "%d.%d.%d.%d\n", bytes[ 3 ], bytes[ 2 ], bytes[ 1 ], bytes[ 0 ] );
}

int main( int argc, char* argv[] )
{
    try
    {
        boost::asio::io_context ioContext;
        const char* stunServerAddr = "stun.l.google.com";
        const char* stunServerPort = "19302";

        UdpSocket udpSocket( ioContext, stunServerAddr, stunServerPort );

        // build stun request packet
        RFC5389Builder rfcBuilder;
        RFC5389 rfc5389 = rfcBuilder.Build();
        
        // send stun request packet
        udpSocket.SendTo( (char*)( &rfc5389 ), sizeof( RFC5389 ) );

        // recv stun response packet
        udpSocket.RecvFrom();
        
        const char* packet = udpSocket.GetData();
        size_t packetSize = udpSocket.GetDataLength();

        // print stun response packet
        for ( size_t i = 0; i < packetSize; i++ )
            printf( "%zd %.2x\n", i,  (unsigned char)( packet[ i ] ) );

        const RFC5389* response =
            reinterpret_cast< const RFC5389* >( packet );

        UInt16 messageType   = ntohs( response->messageType );
        UInt16 messageLength = ntohs( response->messageLength );
        UInt32 magicCookie   = ntohl( response->magicCookie );

        std::cout << std::hex << static_cast<int>( messageType ) << '\n';
        std::cout << std::hex << static_cast<int>( messageLength ) << '\n';
        std::cout << std::hex << static_cast<int>( magicCookie ) << '\n';
        
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

        std::cout << "attributes : " << (int)type << '\n';
        printf( "%.2x\n", (unsigned char)( type ) );
        std::cout << "length : " << (int)length << '\n';
        printf( "%.2x\n", (unsigned char)( length ) );

        switch ( type )
        {
        case STUN_ATTRIBUTES_TYPE_XOR_MAPPED_ADDRESS:
            {
                const XOR_MAPPED_ADDRESS* xorMappedAddress =
                    reinterpret_cast< const XOR_MAPPED_ADDRESS* >(
                        packet + sizeof( RFC5389 ) + sizeof( STUN_ATTRIBUTES ) );

                UInt8  family = xorMappedAddress->family;
                UInt16 port   = ntohs( xorMappedAddress->port );
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
                        print_ip( ip );

                        std::cout << std::dec << "port   : " << port << '\n';
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
    }
    catch ( std::exception& e )
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}