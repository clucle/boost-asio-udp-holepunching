/*****************************************************************//**
 * \file   main.cpp
 * \brief  
 * 
 * \author clucle
 * \date   January 2021
 *********************************************************************/


#include <iostream>
#include <boost/asio.hpp>
#include "core/RFC5389Builder.h"
#include "common/utils.h"
#include "network/UdpSocket.h"


void TestRFC5389Builder()
{
    // build stun request packet
    RFC5389Builder rfcBuilder;
    RFC5389 rfc5389;
    char foo[ sizeof( struct RFC5389 ) ] = { 0, };

    // REQUEST
    rfcBuilder.SetMessageClass( STUN_MESSAGE_CLASS::REQUEST );
    rfc5389 = rfcBuilder.Build();
    memcpy( foo, &rfc5389, sizeof foo );
    std::cout << "REQUEST\n";
    for ( size_t i = 0; i < sizeof foo; i++ )
        printf( "%.2x ", (unsigned char)( foo[ i ] ) );
    std::cout << '\n';

    // INDICATION
    rfcBuilder.SetMessageClass( STUN_MESSAGE_CLASS::INDICATION );
    rfc5389 = rfcBuilder.Build();
    memcpy( foo, &rfc5389, sizeof foo );
    std::cout << "INDICATION\n";
    for ( size_t i = 0; i < sizeof foo; i++ )
        printf( "%.2x ", (unsigned char)( foo[ i ] ) );
    std::cout << '\n';

    // RESPONSE_SUCCSES
    rfcBuilder.SetMessageClass( STUN_MESSAGE_CLASS::RESPONSE_SUCCESS );
    rfc5389 = rfcBuilder.Build();
    memcpy( foo, &rfc5389, sizeof foo );
    std::cout << "RESPONSE_SUCCSES\n";
    for ( size_t i = 0; i < sizeof foo; i++ )
        printf( "%.2x ", (unsigned char)( foo[ i ] ) );
    std::cout << '\n';

    // RESPONSE_ERROR
    rfcBuilder.SetMessageClass( STUN_MESSAGE_CLASS::RESPONSE_ERROR );
    rfc5389 = rfcBuilder.Build();
    memcpy( foo, &rfc5389, sizeof foo );
    std::cout << "RESPONSE_ERROR\n";
    for ( size_t i = 0; i < sizeof foo; i++ )
        printf( "%.2x ", (unsigned char)( foo[ i ] ) );
    std::cout << '\n';
}

int main( int argc, char* argv[] )
{
    TestRFC5389Builder();

    return 0;
}