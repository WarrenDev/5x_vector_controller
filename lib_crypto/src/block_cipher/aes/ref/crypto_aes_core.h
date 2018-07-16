
/**
* crypto_aes_core.h
*
* \copyright
* Copyright 2015 Advanced Embedded Frameworks, LLC. All Rights Reserved.
*
* \author
* Albert E. Warren Jr. (warrendev@outlook.com)
*
* \brief crypto AES core module header.
*/

/**
* @version 3.0 (December 2000)
*
* Optimised ANSI C code for the Rijndael cipher (now AES)
*
* @author Vincent Rijmen <vincent.rijmen@esat.kuleuven.ac.be>
* @author Antoon Bosselaers <antoon.bosselaers@esat.kuleuven.ac.be>
* @author Paulo Barreto <paulo.barreto@terra.com.br>
*
* This code is hereby placed in the public domain.
*
* THIS SOFTWARE IS PROVIDED BY THE AUTHORS ''AS IS'' AND ANY EXPRESS
* OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
* EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef __CRYPTO_AES_CORE_H
#define __CRYPTO_AES_CORE_H

# ifdef   __cplusplus
extern "C" {
# endif

#define MAXKC   (256/32)
#define MAXKB   (256/8)
#define MAXNR   14

typedef unsigned char   u8;
typedef unsigned short  u16;
typedef unsigned int    u32;

enum {
	AES_MODE_ECB = 1,
	AES_MODE_CBC = 2,
	AES_ENC_TYPE = 1,		/* cipher unique type */
	AES_DECRYPTION = 0,
	AES_ENCRYPTION = 1,
	AES_BLOCK_SIZE = 16
};

enum {
	AES_BITSIZE_128 = 128,	/* 128 bit size */
	AES_BITSIZE_192 = 192,	/* 192 bit size */
	AES_BITSIZE_256 = 256	/* 256 bit size */
};

/*  The structure for key information */
typedef struct {
	int     decrypt;
	int     Nr;									/* key-length-dependent number of rounds */
	u8		mode;								/* AES mode */
	u32 	reg[AES_BLOCK_SIZE / sizeof(u32)];	/* for CBC mode */
	u32     ek[4 * (MAXNR + 1)];				/* encrypt key schedule */
	u32     dk[4 * (MAXNR + 1)];				/* decrypt key schedule */
} aes_ctx;

void     aes_core_set_key(aes_ctx *, unsigned char *, int, int);
void     aes_core_decrypt(aes_ctx *, unsigned char *, unsigned char *);
void     aes_core_encrypt(aes_ctx *, unsigned char *, unsigned char *);

# ifdef   __cplusplus
} /* extern "C" */
# endif

#endif /* __CRYPTO_AES_CORE_H */
