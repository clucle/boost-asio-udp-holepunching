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
