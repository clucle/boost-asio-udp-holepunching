/*****************************************************************//**
 * \file   SignalServerProtocol.h
 * \brief  
 * 
 * \author wjden
 * \date   January 2021
 *********************************************************************/


#pragma once


#include <iostream>
#include <memory>
#include "core/types.h"
#include "network/NetworkMessage.h"


class User;


enum ProtocolId : UInt8
{
	// 서버에 udp 소켓 public address 패킷 검증 요청
	ProtocolRequestAddress = 0,
	ProtocolRequestAddressResult = 1,

	// 다른 유저들 address 정보 요청
	ProtocolUserAddressRead = 2,
	ProtocolUserAddressReadResult = 3,

	// 유저 입장 시 다른 유저들에게 address 전달
	ProtocolUserAddressNotify = 4,

	MAX,
};


class SignalServerProtocol
{
public:
	typedef std::shared_ptr< User > UserPtr;

	typedef std::weak_ptr< User > UserWeakPtr;

public:
	SignalServerProtocol( UserPtr user );

	void OnRecvMessage( NetworkMessage networkMessage );

private:
	UserWeakPtr m_user;
};

typedef std::shared_ptr< SignalServerProtocol > SignalServerProtocolPtr;