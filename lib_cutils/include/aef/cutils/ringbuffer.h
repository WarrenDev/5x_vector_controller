
/**
* ringbuffer.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  Definitions used by the ring buffer code module.
*
*/

#ifndef INCLUDE_AEF_CUTILS_RINGBUFFER_H_
#define INCLUDE_AEF_CUTILS_RINGBUFFER_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
    volatile uint16_t head;
    volatile uint16_t tail;
    volatile uint16_t size;
    volatile uint8_t data[];
} RingBuffer;

void buf_reset		(RingBuffer *buf, int32_t size);
int buf_len			(const RingBuffer *buf);
int buf_isfull		(const RingBuffer *buf);
int buf_isempty		(const RingBuffer *buf);
uint8_t buf_get_byte(RingBuffer *buf);
void buf_put_byte	(RingBuffer *buf, uint8_t val);

#ifdef __cplusplus
}
#endif


#endif /* INCLUDE_AEF_CUTILS_RINGBUFFER_H_ */
