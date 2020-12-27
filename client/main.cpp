/*****************************************************************//**
 * \file   main.cpp
 * \brief  
 * 
 * \author clucle
 * \date   December 2020
 *********************************************************************/


#define BOOST_ALL_NO_LIB

#include <iostream>
#include <boost/asio.hpp>
#include "core/UdpSocket.h"
#include "RFC5389Builder.h"


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
        udpSocket.SendTo( (char*)( rfc5389.data ), sizeof( RFC5389 ) );

        // recv stun response packet
        udpSocket.RecvFrom();
        
        const char* packet = udpSocket.GetData();
        size_t packetSize = udpSocket.GetDataLength();

        // print stun response packet
        for ( size_t i = 0; i < packetSize; i++ )
            printf( "%d %.2x\n", i,  (unsigned char)( packet[ i ] ) );
        printf( "\n" );

        int a; std::cin >> a;
    }
    catch ( std::exception& e )
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}