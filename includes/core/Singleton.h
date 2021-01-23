/*****************************************************************//**
 * \file   Singleton.h
 * \brief  
 * 
 * \author wjden
 * \date   January 2021
 *********************************************************************/

#pragma once

template < typename T >
class Singleton
{

public:
    Singleton()
    {
        ms_instance = (T*)( this );
    }

    virtual ~Singleton() {}

public:
    static T& GetInstance()
    {
        return *ms_instance;
    }

private:
    static T* ms_instance;
};

template< typename T > T* Singleton< T >::ms_instance = nullptr;