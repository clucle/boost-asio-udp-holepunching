/*****************************************************************//**
 * \file   utils.h
 * \brief  
 * 
 * \author clucle
 * \date   January 2021
 *********************************************************************/

#pragma once

#include <iostream>
#include "core/types.h"


void PrintIP( UInt32 ip )
{
    unsigned char bytes[ 4 ];
    bytes[ 0 ] = ip & 0xFF;
    bytes[ 1 ] = ( ip >> 8 ) & 0xFF;
    bytes[ 2 ] = ( ip >> 16 ) & 0xFF;
    bytes[ 3 ] = ( ip >> 24 ) & 0xFF;
    printf( "%d.%d.%d.%d\n", bytes[ 3 ], bytes[ 2 ], bytes[ 1 ], bytes[ 0 ] );
}

std::string GetIPString( UInt32 ip )
{
    unsigned char bytes[ 4 ];
    bytes[ 0 ] = ip & 0xFF;
    bytes[ 1 ] = ( ip >> 8 ) & 0xFF;
    bytes[ 2 ] = ( ip >> 16 ) & 0xFF;
    bytes[ 3 ] = ( ip >> 24 ) & 0xFF;

    int ipString[ 4 ] = { bytes[ 3 ], bytes[ 2 ], bytes[ 1 ], bytes[ 0 ] };
    return 
        std::to_string( ipString[ 0 ] ) + "." +
        std::to_string( ipString[ 1 ] ) + "." +
        std::to_string( ipString[ 2 ] ) + "." +
        std::to_string( ipString[ 3 ] );
}
