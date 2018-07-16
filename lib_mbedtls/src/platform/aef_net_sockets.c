
/**
* aef_net_sockets.c
*
* \copyright
* Copyright 2018 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  TCP/IP or UDP/IP networking functions for use with mbedTLS.
*/

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_AEF_FNET_C)

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdlib.h>
#endif

#include "mbedtls/net_sockets.h"

#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>

static service_vtable_t* netsrv = NULL;		// AEF FNET TCP/IP sockets service

static int read ( socket_desc_t* sd, uint8_t* buf, int length )
{
	uint32_t bytes_transferred = 0L;
	recv_parms_t parms = { .socket = sd->socket, .buffer = buf,	.length = length, .flags = 0 };
	netsrv->iocontrol ( IOCTL_FNET_STACK_SOCKET_RECV,
			            &parms,
						sizeof(recv_parms_t),
						NULL,
						0L,
						&bytes_transferred );
	return ( (bytes_transferred) ? bytes_transferred : -1 );
}

static int write ( socket_desc_t* sd, const uint8_t* buf, int length )
{
	uint32_t bytes_transferred = 0L;
	send_parms_t parms = { .socket = sd->socket, .buffer = (uint8_t*)buf, .length = length, .flags = 0 };
	if ( netsrv->iocontrol ( IOCTL_FNET_STACK_SOCKET_SEND,
			                 &parms,
							 sizeof(send_parms_t),
							 NULL,
							 0L,
							 &bytes_transferred ) == SERVICE_STATUS_SUCCESS )
	{
		return bytes_transferred;
	}
	return FNET_ERR;
}

static int close ( socket_desc_t* sd )
{
	/**
	* Destroy the network socket
	*/
	close_parms_t close_parms = { .socket = sd->socket };
	netsrv->iocontrol ( IOCTL_FNET_STACK_SOCKET_CLOSE,
  	           	   	    &close_parms,
						sizeof(close_parms_t),
						NULL,
						0L,
						NULL );
	sd->socket = FNET_ERR;
	return 0;
}

/*
 * Prepare for using the sockets interface
 */
static int net_prepare( void )
{
	service_manager_vtable_t* service_manager = system_get_service_manager();
	netsrv = service_manager->getservice(SRV_FNET_NETWORK);
    return( 0 );
}

/*
 * Initialize a context
 */
void mbedtls_net_init( mbedtls_net_context *ctx )
{
    ctx->fd = FNET_ERR;
    ctx->sd.socket = FNET_ERR;
}

/*
 * Initiate a TCP connection with host:port and the given protocol
 */
int mbedtls_net_connect( mbedtls_net_context *ctx, const char *host,
                         const char *port, int proto )
{
    socket_desc_t sd = ctx->sd;
    struct sockaddr resolvedAddr;

    int ret;
    if( ( ret = net_prepare() ) != 0 )
        return( ret );

	memset ( &resolvedAddr, 0, sizeof(struct sockaddr) );
	fnet_inet_ptos ( host, &resolvedAddr );
	resolvedAddr.sa_family = AF_INET;
	resolvedAddr.sa_port   = FNET_HTONS((short)atoi(port));

	socket_parms_t socket_parms = { AF_INET, SOCK_STREAM, IPPROTO_IP };
	sd.fnet_socket = FNET_ERR;
	if ( netsrv->iocontrol ( IOCTL_FNET_STACK_SOCKET,
			                 &socket_parms,
							 sizeof(socket_parms_t),
							 &sd,
							 sizeof(socket_desc_t),
							 NULL ) != SERVICE_STATUS_SUCCESS )
	{
		return MBEDTLS_ERR_NET_SOCKET_FAILED;
	}

	/**
	* Connect to the server
	*/
	connect_parms_t connect_parms = { sd.fnet_socket, &resolvedAddr, sizeof(struct sockaddr) };
	if ( netsrv->iocontrol ( IOCTL_FNET_STACK_SOCKET_CONNECT,
			                 &connect_parms,
							 sizeof(connect_parms_t),
							 NULL,
							 0L,
							 NULL ) == SERVICE_STATUS_SUCCESS )
	{
		return 0;
	}
	return MBEDTLS_ERR_NET_CONNECT_FAILED;
}

/*
 * Create a listening socket on bind_ip:port
 */
int mbedtls_net_bind( mbedtls_net_context *ctx, const char *bind_ip, const char *port, int proto )
{
	(void)ctx;
	(void)bind_ip;
	(void)port;
	(void)proto;

	int ret;
    if( ( ret = net_prepare() ) != 0 )
        return( ret );

    return( FNET_ERR );
}

/*
 * Check if the requested operation would be blocking on a non-blocking socket
 * and thus 'failed' with a negative return value.
 *
 * Note: on a blocking socket this function always returns 0!
 */
static int net_would_block( const mbedtls_net_context *ctx )
{
    return( 0 );
}

/*
 * Accept a connection from a remote client
 */
int mbedtls_net_accept( mbedtls_net_context *bind_ctx,
                        mbedtls_net_context *client_ctx,
                        void *client_ip, size_t buf_size, size_t *ip_len )
{
    int ret;
    service_status_t status;
    struct sockaddr_in client_addr;

#if defined(__socklen_t_defined) || defined(_SOCKLEN_T) ||  \
    defined(_SOCKLEN_T_DECLARED) || defined(__DEFINED_socklen_t)
    socklen_t n = (socklen_t) sizeof( client_addr );
    socklen_t type_len = (socklen_t) sizeof( type );
#else
    uint32_t n = (int) sizeof( client_addr );
#endif

    socket_desc_t sd;
    accept_parms_t accept_parms =
    {
    		.socket  = bind_ctx->sd.fnet_socket,
			.addr    = (struct sockaddr *) &client_addr,
			.addrlen = &n
    };
	status = netsrv->iocontrol ( IOCTL_FNET_STACK_SOCKET_ACCEPT,
			                 	 &accept_parms,
								 sizeof(accept_parms_t),
								 &sd,
								 sizeof(socket_desc_t),
								 NULL );
    ret = ( status == SERVICE_STATUS_SUCCESS ) ? 0 : FNET_ERR;

    if( ret < 0 )
    {
        if( net_would_block( bind_ctx ) != 0 )
            return( MBEDTLS_ERR_SSL_WANT_READ );

        return( MBEDTLS_ERR_NET_ACCEPT_FAILED );
    }

    if( client_ip != NULL )
    {
        if( client_addr.sin_family == AF_INET )
        {
            struct sockaddr_in *addr4 = (struct sockaddr_in *) &client_addr;
            *ip_len = sizeof( addr4->sin_addr.s_addr );

            if( buf_size < *ip_len )
                return( MBEDTLS_ERR_NET_BUFFER_TOO_SMALL );

            memcpy( client_ip, &addr4->sin_addr.s_addr, *ip_len );
        }
        else
        {
            struct sockaddr_in6 *addr6 = (struct sockaddr_in6 *) &client_addr;
            *ip_len = sizeof( addr6->sin6_addr.s6_addr );

            if( buf_size < *ip_len )
                return( MBEDTLS_ERR_NET_BUFFER_TOO_SMALL );

            memcpy( client_ip, &addr6->sin6_addr.s6_addr, *ip_len);
        }
    }

    return( 0 );
}

/*
 * Set the socket blocking or non-blocking
 */
int mbedtls_net_set_block( mbedtls_net_context *ctx )
{
	return 0;
}

int mbedtls_net_set_nonblock( mbedtls_net_context *ctx )
{
	return 0;
}

/*
 * Portable usleep helper
 */
void mbedtls_net_usleep( unsigned long usec )
{
	time_delay ( ( usec + 999 ) / 1000 );
}

/*
 * Read at most 'len' characters
 */
int mbedtls_net_recv( void *ctx, unsigned char *buf, size_t len )
{
    int ret;
    socket_desc_t sd = ((mbedtls_net_context *) ctx)->sd;

    if( sd.fnet_socket < 0 )
        return( MBEDTLS_ERR_NET_INVALID_CONTEXT );

    ret = (int) read( &sd, buf, len );

    if( ret < 0 )
    {
        if( net_would_block( ctx ) != 0 )
            return( MBEDTLS_ERR_SSL_WANT_READ );

        return( MBEDTLS_ERR_NET_RECV_FAILED );
    }

    return( ret );
}

/*
 * Read at most 'len' characters, blocking for at most 'timeout' ms
 */
int mbedtls_net_recv_timeout( void *ctx, unsigned char *buf, size_t len,
                      uint32_t timeout )
{
	(void)timeout;
    socket_desc_t sd = ((mbedtls_net_context *) ctx)->sd;

    if( sd.fnet_socket < 0 )
        return( MBEDTLS_ERR_NET_INVALID_CONTEXT );

    /* This call will not block */
    return( mbedtls_net_recv( ctx, buf, len ) );
}

/*
 * Write at most 'len' characters
 */
int mbedtls_net_send( void *ctx, const unsigned char *buf, size_t len )
{
    int ret;
    socket_desc_t sd = ((mbedtls_net_context *) ctx)->sd;

    if( sd.fnet_socket < 0 )
        return( MBEDTLS_ERR_NET_INVALID_CONTEXT );

    ret = (int) write( &sd, buf, len );

    if( ret < 0 )
    {
        if( net_would_block( ctx ) != 0 )
            return( MBEDTLS_ERR_SSL_WANT_WRITE );

        return( MBEDTLS_ERR_NET_SEND_FAILED );
    }

    return( ret );
}

/*
 * Gracefully close the connection
 */
void mbedtls_net_free( mbedtls_net_context *ctx )
{
    if( ctx->sd.fnet_socket == FNET_ERR )
        return;

	shutdown_parms_t shutdown_parms = { .socket = ctx->sd.fnet_socket, .how = 2 };
	netsrv->iocontrol ( IOCTL_FNET_STACK_SOCKET_SHUTDOWN,
  	           		    &shutdown_parms,
						sizeof(shutdown_parms_t),
						NULL,
						0L,
						NULL );
    close( &ctx->sd );

    ctx->sd.fnet_socket = FNET_ERR;
}

#endif /* MBEDTLS_AEF_FNET_C */
