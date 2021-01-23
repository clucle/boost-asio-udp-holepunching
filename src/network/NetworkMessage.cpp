/**
 * MinigameServer - a simple flip card game server
 * Copyright (C) 2020  clucle <wjdenwls123@gmail.com>
 * Github : http://github.com/clucle
 */


#define _CRT_SECURE_NO_WARNINGS

#include "NetworkMessage.h"

/**
 * @brief Constructor
 * ��Ŷ ������ �ʱ�ȭ
 * @param tcp::socket socket
 * @return void
 */
NetworkMessage::NetworkMessage()
	: m_data(), m_bodyLength( 0 ), m_readPosition( 0 )
{
}

/**
 * @brief ��Ŷ��ü�� �б� �������� ����
 * @return const char* : ����� ����Ű�� ������
 */
const char* NetworkMessage::GetData() const
{
	return m_data;
}

/**
 * @brief ��Ŷ��ü�� ����
 * @return char* : ����� ����Ű�� ������
 */
char* NetworkMessage::GetData()
{
	return m_data;
}

/**
 * @brief ��Ŷ��ü ����
 * @return size_t : ��� ���� + ��Ŷ �ٵ� ����
 */
size_t NetworkMessage::GetLength() const
{
	size_t length = HEADER_LENGTH + m_bodyLength;
	return length;
}

/**
 * @brief ����� ������ ��Ŷ �б� �������� ����
 * ���� �����Ϳ� ���� �� �� ����Ѵ�
 * @return const char* : �ٵ� ����Ű�� ������
 */
const char* NetworkMessage::GetBody() const
{
	const char* body = m_data + HEADER_LENGTH;
	return body;
}

/**
 * @brief ����� ������ ��Ŷ ����
 * ���� �����Ϳ� ���� �� �� ����Ѵ�
 * @return char* : �ٵ� ����Ű�� ������
 */
char* NetworkMessage::GetBody()
{
	char* body = m_data + HEADER_LENGTH;
	return body;
}

/**
 * @brief ��Ŷ �ٵ��� ����
 * @return size_t : ��Ŷ �ٵ� ����
 */
size_t NetworkMessage::GetBodyLength() const
{
	return m_bodyLength;
}

/**
 * @brief �޾ƿ� ��Ŷ���� ������ ��
 * ����� ��Ŷ ���̸� �ް�, ��Ŷ ���̰� valid ���� üũ
 * @return bool : ��Ŷ ���ڵ��� ���� ����
 */
bool NetworkMessage::DecodeHeader()
{
	// ��Ŷ ���̸� �޾ƿ� char �迭
	char header[ HEADER_LENGTH + 1 ] = "";
	strncat( header, m_data, HEADER_LENGTH );
	// bodyLength �� �Ľ�
	m_bodyLength = atoi( header );

	bool success = true;
	if ( m_bodyLength > MAX_BODY_LENGTH )
	{
		// bodyLength �� �ʹ� �� invalid �ϴٸ� �߰� �� �� ����
		m_bodyLength = 0;
		success = false;
	}
	return success;
}

/**
 * @brief ������ ���� ��Ŷ�� ����
 * ����� ��Ŷ ���̸� ����� �߰�����
 * @return void
 */
void NetworkMessage::EncodeHeader()
{
	// ��Ŷ ���̸� �Է��� char �迭
	char header[ HEADER_LENGTH + 1 ] = "";
	sprintf( header, "%4d", static_cast<int>( m_bodyLength ) );
	// ��Ŷ ����� ���̸� �ٿ�����
	memcpy( m_data, header, HEADER_LENGTH );
}

/**
 * @brief ��Ŷ ���� �غ�
 * @return void
 */
void NetworkMessage::ReadyToRead()
{
	m_readPosition = 0;
}

/**
 * @brief �� ����Ʈ�� ����
 * @return Byte : �� ����Ʈ
 */
UInt8 NetworkMessage::ReadByte()
{
	if ( !_CanRead( 1 ) )
	{
		return 0;
	}
	return m_data[ HEADER_LENGTH + m_readPosition++ ];
}


/**
 * @brief ��Ʈ�� ���̸� ���, ��Ʈ���� ��
 * @param uint16_t stringLen : ���ڼ��� ���� �ʿ� ���ٸ� �̸� ������
 * @return std::string : �� ��Ʈ��
 */
std::string NetworkMessage::ReadString( uint16_t stringLen )
{
	if ( stringLen == 0 )
	{
		stringLen = Read<uint16_t>();
	}

	if ( !_CanRead( stringLen ) )
	{
		return std::string();
	}

	char* body = reinterpret_cast<char*>( m_data ) + HEADER_LENGTH + m_readPosition; //does not break strict aliasing
	m_readPosition += stringLen;
	return std::string( body, stringLen );
}

/**
 * @brief ��Ŷ�� ����Ʈ ������ �߰���Ŵ
 * param uint8 value : ��Ŷ�� �߰��� ����Ʈ ��
 * @return bool : �߰��� ���� �ߴ��� ����
 */
bool NetworkMessage::AppendByte( UInt8 value )
{
	if ( !_CanAdd( 1 ) )
	{
		// Byte �� �߰��� �ڸ��� ���ڶ�� ����
		return false;
	}
	// �� �ڸ��� 1 byte�� �߰��ϰ� length�� 1 �߰�
	m_data[ HEADER_LENGTH + m_bodyLength++ ] = value;
	return true;
}

/**
 * @brief ��Ŷ�� ��Ʈ�� ������ �߰���Ŵ
 * @param cosnt std::string& value : ��Ŷ�� �߰��� ��Ʈ�� ���۷���
 * @return bool : �߰��� ���� �ߴ��� ����
 */
bool NetworkMessage::AppendString( const std::string& value )
{
	// string �� ����
	size_t stringLength = value.length();
	// string�� ��������� �߰��� ����Ʈ���� �߰������� �ʿ�
	if ( !_CanAdd( stringLength + PREFIX_BYTE_FOR_STRING ) )
	{
		return false;
	}
	// ��Ʈ�� �տ� ���ڼ��� �߰�����
	Append<UInt16>( static_cast<UInt16>( stringLength ) );
	// ���ڸ��� ��Ʈ�� �߰��ϰ� length�� ��Ʈ�� ���̸�ŭ �߰�
	memcpy( m_data + HEADER_LENGTH + m_bodyLength, value.c_str(), stringLength );
	m_bodyLength += stringLength;

	return true;
}

/**
 * @brief ��Ŷ ���ۿ� ������ �߰� �� �� �ִ��� Ȯ��
 * @param size_t size : �߰��� ����
 * @return bool : �߰��� ���� �ߴ��� ����
 */
bool NetworkMessage::_CanAdd( size_t size ) const
{
	// ������ �߰��ؼ� MAX_BODY_LENGTH ���� �۴ٸ� �߰� �� �� ����
	return ( m_bodyLength + size ) < MAX_BODY_LENGTH;
}

bool NetworkMessage::_CanRead( size_t size ) const
{
	if ( ( m_readPosition + size ) > m_bodyLength )
	{
		return false;
	}
	return true;
}
