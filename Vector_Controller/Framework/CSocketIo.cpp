
/**
* CSocketIo.cpp
*
* \copyright
* Copyright 2017 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  Implementation of the CSocketIo class.
* 		  Uses the FNET network stack service.
*/

#include "aef/embedded/system/system_core.h"
#include "CSocketIo.h"

/**
* CSocketIo class constructors
*/
CSocketIo::CSocketIo(fnet_address_family_t ProtocolFamily, fnet_socket_type_t Type, uint32_t Protocol)
	: m_socket(FNET_ERR)
	, m_ProtocolFamily(ProtocolFamily)
	, m_Type(Type)
	, m_Protocol(Protocol)
	, m_netsrv(nullptr)
{
	service_manager_vtable_t* service_manager = system_get_service_manager();
	m_netsrv = service_manager->getservice(SRV_FNET_NETWORK);
	CreateSocket();
}

CSocketIo::CSocketIo(fnet_socket_t socket, fnet_address_family_t ProtocolFamily, fnet_socket_type_t Type, uint32_t Protocol)
	: m_socket(socket)
	, m_ProtocolFamily(ProtocolFamily)
	, m_Type(Type)
	, m_Protocol(Protocol)
	, m_netsrv(nullptr)
{
	service_manager_vtable_t* service_manager = system_get_service_manager();
	m_netsrv = service_manager->getservice(SRV_FNET_NETWORK);
}

/**
* CSocketIo class destructor
*/
CSocketIo::~CSocketIo()
{
	CloseSocket ();
}

/**
* CSocketIo::GstSocket
*
* Get the value of the class socket identifier
*
* \param none
*
* \return Socket identifier
*/
fnet_socket_t CSocketIo::GetSocket ( void )
{
	return m_socket;
}

/**
* CSocketIo::CreateSocket
*
* Create a new socket.
*
* \param none
*
* \return Socket identifier
*/
int CSocketIo::CreateSocket( void )
{
	if ( m_netsrv != NULL )
	{
		socket_parms_t parms = { m_ProtocolFamily, m_Type, m_Protocol };
		socket_desc_t socket = { FNET_ERR };
		uint32_t bytes_returned = 0L;
		service_status_t result = m_netsrv->iocontrol ( IOCTL_FNET_STACK_SOCKET,
				                                        &parms,
														sizeof(socket_parms_t),
														&socket,
														sizeof(socket_desc_t),
														&bytes_returned );
		if ( (result == SERVICE_STATUS_SUCCESS) && (bytes_returned == sizeof(socket_desc_t)) )
		{
			m_socket = socket.fnet_socket;
			return SOCKETIO_NOERROR;
		}
	}
	return SOCKETIO_ERROR;
}

/**
* CSocketIo::CloseSocket
*
* Close the socket.
*
* \param none
*
* \return 1 on success
*         0 on failure
*/
int CSocketIo::CloseSocket ( void )
{
	if ( m_netsrv != NULL )
	{
		close_parms_t parms = { m_socket };
		if ( m_netsrv->iocontrol ( IOCTL_FNET_STACK_SOCKET_CLOSE,
				                   &parms,
								   sizeof(close_parms_t),
								   NULL,
								   0L,
								   NULL ) == SERVICE_STATUS_SUCCESS )
		{
			return SOCKETIO_NOERROR;
		}
	}
	return SOCKETIO_ERROR;
}

/**
* CSocketIo::SetOptions
*
* Set a a socket option value.
*
* \param level		Option level
* \param name		Option name
* \param value		Option value
* \param size		Option size
*
* \return 1 on success
*         0 on failure
*/
int CSocketIo::SetOptions ( fnet_protocol_t level, fnet_socket_options_t name, void* value, fnet_size_t size )
{
	if ( m_netsrv != NULL )
	{
		setopt_parms_t parms = { m_socket, level, name, value, size };
		if ( m_netsrv->iocontrol ( IOCTL_FNET_STACK_SOCKET_SETOPT,
				                   &parms,
								   sizeof(setopt_parms_t),
								   NULL,
								   0L,
								   NULL ) == SERVICE_STATUS_SUCCESS )
		{
			return SOCKETIO_NOERROR;
		}
	}
	return SOCKETIO_ERROR;
}

/**
* CSocketIo::GetOptions
*
* Get a a socket option value.
*
* \param level		Option level
* \param name		Option name
* \param value		Option value
* \param size		Option size
*
* \return 1 on success
*         0 on failure
*/
int CSocketIo::GetOptions ( fnet_protocol_t level, fnet_socket_options_t name, void* value, fnet_size_t* size )
{
	if ( m_netsrv != NULL )
	{
		getopt_parms_t parms = { m_socket, level, name, value, size };
		if ( m_netsrv->iocontrol ( IOCTL_FNET_STACK_SOCKET_GETOPT,
				                   &parms,
								   sizeof(getopt_parms_t),
								   NULL,
								   0L,
								   NULL ) == SERVICE_STATUS_SUCCESS )
		{
			return SOCKETIO_NOERROR;
		}
	}
	return SOCKETIO_ERROR;
}

/**
* CSocketIo::BindAddr
*
* Associate a local address with the socket.
*
* \param addr		Local address
* \param size		Size of the address
*
* \return 1 on success
*         0 on failure
*/
int CSocketIo::BindAddr ( struct sockaddr* addr, uint16_t size )
{
	if ( m_netsrv != NULL )
	{
		bind_parms_t parms = { m_socket, addr, size };
		if ( m_netsrv->iocontrol ( IOCTL_FNET_STACK_SOCKET_BIND,
				                   &parms,
								   sizeof(bind_parms_t),
								   NULL,
								   0L,
								   NULL ) == SERVICE_STATUS_SUCCESS )
		{
			return SOCKETIO_NOERROR;
		}
	}
	return SOCKETIO_ERROR;
}

/**
* CSocketIo::ListenTo
*
* Enter the listen state, where the socket is listening for an incoming connection
*
* \param backlog	backlog
*
* \return 1 on success
*         0 on failure
*/
int CSocketIo::ListenTo ( uint16_t backlog )
{
	if ( m_netsrv != NULL )
	{
		listen_parms_t parms = { m_socket, backlog };
		if ( m_netsrv->iocontrol ( IOCTL_FNET_STACK_SOCKET_LISTEN,
				                   &parms,
								   sizeof(listen_parms_t),
								   NULL,
								   0L,
								   NULL ) == SERVICE_STATUS_SUCCESS )
		{
			return SOCKETIO_NOERROR;
		}
	}
	return SOCKETIO_ERROR;
}

/**
* CSocketIo::ConnectTo
*
* Establish a connection on the socket.
*
* \param addr		Address of the socket that the connection should be made
* \param size		Length of the address parameter
*
* \return 1 on success
*         0 on failure
*/
int CSocketIo::ConnectTo (struct sockaddr* addr, uint32_t size )
{
	if ( m_netsrv != NULL )
	{
		connect_parms_t parms = { m_socket, addr, size };
		if ( m_netsrv->iocontrol ( IOCTL_FNET_STACK_SOCKET_CONNECT,
				                   &parms,
								   sizeof(connect_parms_t),
								   NULL,
								   0L,
								   NULL ) == SERVICE_STATUS_SUCCESS )
		{
			return SOCKETIO_NOERROR;
		}
	}
	return SOCKETIO_ERROR;
}

/**
* CSocketIo::AcceptRequest
*
* Accept the connection request on the socket
*
* \param addr		Address of the remote host
* \param size		Size of the address parameter
*
* \return New socket container class for the connection
*         NULL on failure
*/
CSocketIo* CSocketIo::AcceptRequest ( struct sockaddr* addr, uint32_t* size )
{
	CSocketIo* accept_socket = nullptr;
	if ( m_netsrv != NULL )
	{
		accept_parms_t parms = { m_socket, addr, size };
		socket_desc_t socket = { FNET_ERR };
		uint32_t bytes_returned = 0L;
		service_status_t result = m_netsrv->iocontrol ( IOCTL_FNET_STACK_SOCKET_ACCEPT,
				                                        &parms,
														sizeof(accept_parms_t),
														&socket,
														sizeof(socket_desc_t),
														&bytes_returned );
		if ( (result == SERVICE_STATUS_SUCCESS) && (bytes_returned == sizeof(socket_desc_t)) )
		{
			accept_socket = new CSocketIo (socket.fnet_socket);
		}
	}
	return accept_socket;
}

/**
* CSocketIo::SendPacket
*
* Send data on a connected socket
*
* \param buffer		Pointer to the data to send
* \param size		Length of the data to send
* \param flags		Flags
*
* \return 1 on success
*         0 on failure
*/
int CSocketIo::SendPacket ( uint8_t* buffer, fnet_size_t size, fnet_flag_t flags )
{
	if ( m_netsrv != NULL )
	{
		uint32_t bytes_transferred = 0L;
		send_parms_t parms = { m_socket, buffer, size, flags };
		if ( m_netsrv->iocontrol ( IOCTL_FNET_STACK_SOCKET_SEND,
				                   &parms,
								   sizeof(send_parms_t),
								   NULL,
								   0L,
								   &bytes_transferred ) == SERVICE_STATUS_SUCCESS )
		{
			return SOCKETIO_NOERROR;
		}
	}
	return SOCKETIO_ERROR;
}

/**
* CSocketIo::RecvPacket
*
* Receive data on a connected socket
*
* \param buffer		Pointer to the memory area for data received
* \param size		Length of the data to receive and also size of the buffer
* \param flags		Flags
*
* \return 1 on success
*         0 on failure
*/
int CSocketIo::RecvPacket ( uint8_t* buffer, fnet_size_t size, fnet_flag_t flags )
{
	if ( m_netsrv != NULL )
	{
		uint32_t bytes_transferred = 0L;
		recv_parms_t parms = { m_socket, buffer, size, flags };
		if ( m_netsrv->iocontrol ( IOCTL_FNET_STACK_SOCKET_RECV,
				                   &parms,
								   sizeof(recv_parms_t),
								   NULL,
								   0L,
								   &bytes_transferred ) == SERVICE_STATUS_SUCCESS )
		{
			return (int)bytes_transferred;
		}
	}
	return SOCKETIO_ERROR;
}

/**
* CSocketIo::SendPacketTo
*
* Send data on a connected socket
*
* \param buffer		Pointer to the memory area for data received
* \param size		Length of the data to receive and also size of the buffer
* \param addr		Destination address
* \param addrsize	Length of the address parameter
*
* \return 1 on success
*         0 on failure
*/
int	CSocketIo::SendPacketTo ( uint8_t* buffer, fnet_size_t size, sockaddr* addr, uint32_t addrsize )
{
	if ( m_netsrv != NULL )
	{
		uint32_t bytes_transferred = 0L;
		send_to_parms_t parms = { m_socket, buffer, size, 0, addr, addrsize };
		if ( m_netsrv->iocontrol ( IOCTL_FNET_STACK_SOCKET_SEND_TO,
				                   &parms,
								   sizeof(send_to_parms_t),
								   NULL,
								   0L,
								   &bytes_transferred ) == SERVICE_STATUS_SUCCESS )
		{
			return SOCKETIO_NOERROR;
		}
	}
	return SOCKETIO_ERROR;
}

/**
* CSocketIo::RecvPacketFrom
*
* Receive data on a connected socket
*
* \param buffer		Pointer to the memory area for data received
* \param size		Length of the data to receive and also size of the buffer
* \param addr		Destination address
* \param addrsize	Pointer to the length of the address parameter
*
* \return 1 on success
*         0 on failure
*/
int	CSocketIo::RecvPacketFrom ( uint8_t* buffer, fnet_size_t size, sockaddr* addr, uint32_t* addrsize )
{
	if ( m_netsrv != NULL )
	{
		uint32_t bytes_transferred = 0L;
		recv_from_parms_t parms = { m_socket, buffer, size, 0, addr, addrsize };
		if ( m_netsrv->iocontrol ( IOCTL_FNET_STACK_SOCKET_RECV_FROM,
				                   &parms,
								   sizeof(recv_from_parms_t),
								   NULL,
								   0L,
								   &bytes_transferred ) == SERVICE_STATUS_SUCCESS )
		{
			return SOCKETIO_NOERROR;
		}
	}
	return SOCKETIO_ERROR;
}

/**
* CSocketIo::EndSession
*
* Shutdown a connection on the socket
*
* \param flag		Shutdown flags
*
* \return 1 on success
*         0 on failure
*/
int CSocketIo::EndSession ( fnet_sd_flags_t flag )
{
	if ( m_netsrv != NULL )
	{
		shutdown_parms_t parms = { m_socket, flag };
		if ( m_netsrv->iocontrol ( IOCTL_FNET_STACK_SOCKET_SHUTDOWN,
				                   &parms,
								   sizeof(shutdown_parms_t),
								   NULL,
								   0L,
								   NULL ) == SERVICE_STATUS_SUCCESS )
		{
			return SOCKETIO_NOERROR;
		}
	}
	return SOCKETIO_ERROR;
}

/**
* CSocketIo::GetMACAddr
*
* Retrieve the hardware (MAC) address
*
* \param macAddr		Pointer to the memory area to store the MAC address
*
* \return 1 on success
*         0 on failure
*/
int CSocketIo::GetMACAddr ( uint8_t* macAddr )
{
	if ( m_netsrv != NULL )
	{
		macaddr_parms_t mac_addr;
		if ( m_netsrv->iocontrol ( IOCTL_FNET_STACK_GET_MACADDR,
								   NULL,
								   0L,
				                   &mac_addr,
								   sizeof(macaddr_parms_t),
								   NULL ) == SERVICE_STATUS_SUCCESS )
		{
			memcpy ( macAddr, mac_addr.hw_addr, sizeof(mac_addr.hw_addr) );
			return SOCKETIO_NOERROR;
		}
	}
	return SOCKETIO_ERROR;
}

/**
* CSocketIo::GetIP4Addr
*
* Retrieve the IP4 address
*
* \param macAddr		Pointer to the memory area to store the IP4 address
*
* \return 1 on success
*         0 on failure
*/
int CSocketIo::GetIP4Addr ( uint32_t* ip4Addr )
{
	if ( m_netsrv != NULL )
	{
		ip4addr_parms_t ip4_addr;
		if ( m_netsrv->iocontrol ( IOCTL_FNET_STACK_GET_IP4ADDR,
								   NULL,
								   0L,
				                   &ip4_addr,
								   sizeof(ip4addr_parms_t),
								   NULL ) == SERVICE_STATUS_SUCCESS )
		{
			*ip4Addr = ip4_addr.ip4_addr;
			return SOCKETIO_NOERROR;
		}
	}
	return SOCKETIO_ERROR;
}

