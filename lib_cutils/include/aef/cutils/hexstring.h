
/**
* hexstring.h
*
* \copyright
* Copyright 2018 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief  Definitions used by the hexstring code module
*/

#ifndef INCLUDE_AEF_CUTILS_HEXSTRING_H_
#define INCLUDE_AEF_CUTILS_HEXSTRING_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>

/**
* @fn      isValidBase64
*
* @brief   Check to see if the input byte is valid Base64
*
* @param   byte         Input character
*
* @return  TRUE or FALSE
*/
bool isValidBase64(unsigned char byte);

/**
* @fn      encode_base64
*
* @brief   Encode byte buffer to Base64
*
* @param   inputBuffer          Pointer to data to encode
* @param   inputSize            Length of the input data
* @param   outputBuffer         Pointer to output buffer
* @param   outputMaxSize        Maximum size of the output buffer
*
* @return  Output size or -1
*/
uint16_t encode_base64(const uint8_t* inputBuffer, uint16_t inputSize, uint8_t* outputBuffer, uint16_t outputMaxSize);

/**
* @fn      decode_base64
*
* @brief   Encode byte buffer to Base64
*
* @param   inputBuffer          Pointer to data to encode
* @param   inputSize            Length of the input data
* @param   outputBuffer         Pointer to output buffer
* @param   outputMaxSize        Maximum size of the output buffer
*
* @return  Output size or -1
*/
uint16_t decode_base64(const uint8_t* inputBuffer, uint16_t inputSize, uint8_t* outputBuffer, uint16_t outputMaxSize);

/**
* @fn      data2HexStr
*
* @brief   Convert data buffer to hex string.
*
* @param   pData       Pointer to data buffer
* @param   size        Length of the data buffer in bytes
* @param   buffer      Destination buffer
*
* @return  Pointer to destination buffer
*/
char* data2HexStr( uint8_t* pData, uint8_t size, char* buffer );


#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_AEF_CUTILS_HEXSTRING_H_ */
