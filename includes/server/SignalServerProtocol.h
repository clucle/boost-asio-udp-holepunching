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
	// ������ udp ���� public address ��Ŷ ���� ��û
	ProtocolRequestAddress = 0,
	ProtocolRequestAddressResult = 1,

	// �ٸ� ������ address ���� ��û
	ProtocolUserAddressRead = 2,
	ProtocolUserAddressReadResult = 3,

	// ���� ���� �� �ٸ� �����鿡�� address ����
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