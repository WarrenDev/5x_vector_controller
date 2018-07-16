
/**
* ringbuffer.c
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  Ring buffer code module implementation
*/

#include "../../lib_cutils/include/aef/cutils/ringbuffer.h"

__attribute__( ( always_inline ) ) inline void buf_reset (RingBuffer *buf, int32_t size)
{
	buf->head = buf->tail = 0;
	buf->size = size;
}


__attribute__( ( always_inline ) ) inline int buf_len(const RingBuffer *buf)
{
    int len = buf->tail - buf->head;
    if (len < 0)
        len += buf->size;

    return len;
}


__attribute__( ( always_inline ) ) inline int buf_isfull(const RingBuffer *buf)
{
    return buf_len(buf) == (buf->size-1);
}


__attribute__( ( always_inline ) ) inline int buf_isempty(const RingBuffer *buf)
{
    return buf->head == buf->tail;
}


__attribute__( ( always_inline ) ) inline int advance(uint16_t i, uint16_t size)
{
    if (++i >= size)             // Avoid modulo or divide
        i = 0;

    return i;
}


__attribute__( ( always_inline ) ) inline uint8_t buf_get_byte(RingBuffer *buf)
{
    const uint8_t item = buf->data[buf->head];
    buf->head = advance(buf->head, buf->size);
    return item;
}


__attribute__( ( always_inline ) ) inline void buf_put_byte(RingBuffer *buf, uint8_t val)
{
    buf->data[buf->tail] = val;
    buf->tail = advance(buf->tail, buf->size);
}

