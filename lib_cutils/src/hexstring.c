
/**
* hexstring.c
*
* \copyright
* Copyright 2018 Advnaced Embedded Frameworks, Inc. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. <warrendev@outlook.com>
*
* \brief  Implementation of the hexstring code module
*
*/

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "aef/cutils/hexstring.h"

/**
* Base64 Character Set
*/
static const char base64_chars[] =
{
    'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z', \
    'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z', \
    '0','1','2','3','4','5','6','7','8','9','+','/'
};

/**
* @fn      isValidBase64
*
* @brief   Check to see if the input byte is valid Base64
*
* @param   byte         Input character
*
* @return  TRUE or FALSE
*/
bool isValidBase64(unsigned char byte)
{
    return (isalnum(byte) || (byte == '+') || (byte == '/'));
}

/**
* @fn      findBase64Character
*
* @brief   Find index of Base64 input character
*
* @param   c         Input character
*
* @return  Base64 character or -1
*/
int8_t findBase64Character(unsigned char c)
{
    uint16_t i = 0;

    //Check for Valid Characters
    if(!isValidBase64(c))
    {
        return -1;
    }

    //Return Character Index
    while(i < strlen(base64_chars))
    {
        if(c == base64_chars[i])
        {
            return i;
        }
        else
        {
            i++;
        }
    }

    //Character Not Found
    return -1;
}

/**
* @fn      encode_base64
*
* @brief   Encode byte buffer to Base64
*
* @param   inputBuffer          Pointer to data to encode
* @param   inputSize            Length of the input data
* @param   outputBuffer         Poitner to output buffer
* @param   outputMaxSize        Maximum size of the output buffer
*
* @return  Output size or -1
*/
uint16_t encode_base64(const uint8_t* inputBuffer, uint16_t inputSize, uint8_t* outputBuffer, uint16_t outputMaxSize)
{
    int16_t i = 0;
    int16_t j = 0;
    uint16_t output_bytes = 0;
    unsigned char input_array_wnd[3];

    //Compute output size
    uint16_t outputSize = 4*((uint16_t)ceil(inputSize/3));

    //Ensure output buffer is big enough for the encoding. i.e. for input n output size = 4*ceil(n/3)
    if(outputMaxSize < outputSize)
    {
        return 0;
    }

    //Iterate Through Input Bytes Converting to Base64
    while (inputSize--)
    {
        //Grab some bytes
        input_array_wnd[i++] = *(inputBuffer++);

        //If the window is full then encode
        if (i == 3)
        {
            outputBuffer[output_bytes++] = base64_chars[(input_array_wnd[0] & 0xFC) >> 2];
            outputBuffer[output_bytes++] = base64_chars[((input_array_wnd[0] & 0x03) << 4) + ((input_array_wnd[1] & 0xF0) >> 4)];
            outputBuffer[output_bytes++] = base64_chars[((input_array_wnd[1] & 0x0F) << 2) + ((input_array_wnd[2] & 0xC0) >> 6)];
            outputBuffer[output_bytes++] = base64_chars[(input_array_wnd[2] & 0x3F)];
            //Reset Window Count
            i = 0;
        }
    }

    //Check to see if a partial window remains
    if (i)
    {
        int16_t output_marker = output_bytes;

        //Pre-fill with padding
        for(j = i; j < 3; j++)
        {
            input_array_wnd[j] = 0;
        }

        //Encode Last Window
        outputBuffer[output_bytes++] = base64_chars[(input_array_wnd[0] & 0xFC) >> 2];
        outputBuffer[output_bytes++] = base64_chars[((input_array_wnd[0] & 0x03) << 4) + ((input_array_wnd[1] & 0xF0) >> 4)];
        outputBuffer[output_bytes++] = base64_chars[((input_array_wnd[1] & 0x0F) << 2) + ((input_array_wnd[2] & 0xC0) >> 6)];
        outputBuffer[output_bytes++] = base64_chars[(input_array_wnd[2] & 0x3F)];

        //Overwrite padding with '='
        while((i++ < 3))
        {
            outputBuffer[output_marker + i]= '=';
        }
    }
    return output_bytes;
}

/**
* @fn      decode_base64
*
* @brief   Encode byte buffer to Base64
*
* @param   inputBuffer          Pointer to data to encode
* @param   inputSize            Length of the input data
* @param   outputBuffer         Poitner to output buffer
* @param   outputMaxSize        Maximum size of the output buffer
*
* @return  Output size or -1
*/
uint16_t decode_base64(const uint8_t* inputBuffer, uint16_t inputSize, uint8_t* outputBuffer, uint16_t outputMaxSize)
{
    int16_t i = 0;
    int16_t j = 0;
    uint16_t output_bytes = 0;
    uint16_t input_index = 0;
    unsigned char input_array_wnd[4];

    //Compute output size
    //  uint16_t outputSize = 3*(inputSize/4); //a little over considering padding but ok

    //Ensure output buffer is big enough for the decoding. i.e. for input n output size = 3*(n/4)
    //-AJL if(outputMaxSize < outputSize) {
    //-AJL  return 0;
    //-AJL }

    //Iterate Through Input Bytes Converting to Base64
    // Do not process padding or invalid characters
    while (inputSize-- && (inputBuffer[input_index] != '=') && isValidBase64(inputBuffer[input_index]))
    {
        //Grab some bytes
        input_array_wnd[i++] = (unsigned char)(findBase64Character(inputBuffer[input_index++]));

        //If the window is full then decode
        if (i == 4)
        {
            outputBuffer[output_bytes++] = (input_array_wnd[0] << 2) + ((input_array_wnd[1] & 0x30) >> 4);
            outputBuffer[output_bytes++] = ((input_array_wnd[1] & 0x0F) << 4) + ((input_array_wnd[2] & 0x3C) >> 2);
            outputBuffer[output_bytes++] = ((input_array_wnd[2] & 0x03) << 6) + input_array_wnd[3];
            //Reset Window Count
            i = 0;
        }
    }

    //Check to see if a parital window remains
    if (i)
    {
        //Pre-fill with padding
        for(j = i; j < 3; j++)
        {
            input_array_wnd[j] = 0;
        }

        //Decode last window
        if(i >= 1)
        {
            outputBuffer[output_bytes++] = (input_array_wnd[0] << 2) + ((input_array_wnd[1] & 0x30) >> 4);
        }
        if(i == 3)
        {
            outputBuffer[output_bytes++] = ((input_array_wnd[1] & 0x0F) << 4) + ((input_array_wnd[2] & 0x3C) >> 2);
        }
    }
    return output_bytes;
}

/**
* data2HexStr - Convert data buffer to hex string.
*
* \param   pData       Pointer to data buffer
* \param   size        Length of the data buffer in bytes
* \param   buffer      Destination buffer
*
* \return  Pointer to destination buffer
*/
char* data2HexStr( uint8_t* pData, uint8_t size, char* buffer )
{
    uint8_t i;
    char  hex[] = "0123456789ABCDEF";
    char  *pStr = buffer;

    for ( i = 0; i < size; i++ )
    {
        *pStr++ = hex[*pData >> 4];
        *pStr++ = hex[*pData++ & 0x0F];
    }

    *pStr = 0;
    return buffer;
}

