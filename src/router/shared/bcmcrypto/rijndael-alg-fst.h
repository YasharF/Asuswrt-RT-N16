/* $Id: rijndael-alg-fst.h,v 1.1.1.1 2008/07/21 09:20:24 james26_jang Exp $ */

/**
 * rijndael-alg-fst.h
 *
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
#ifndef __RIJNDAEL_ALG_FST_H
#define __RIJNDAEL_ALG_FST_H

#define MAXKC	(256/32)
#define MAXKB	(256/8)
#define MAXNR	14

#include <typedefs.h>

int rijndaelKeySetupEnc(uint32 rk[/*4*(Nr + 1)*/], const uint8 cipherKey[], int keyBits);
int rijndaelKeySetupDec(uint32 rk[/*4*(Nr + 1)*/], const uint8 cipherKey[], int keyBits);
void rijndaelEncrypt(const uint32 rk[/*4*(Nr + 1)*/], int Nr, const uint8 pt[16], uint8 ct[16]);
void rijndaelDecrypt(const uint32 rk[/*4*(Nr + 1)*/], int Nr, const uint8 ct[16], uint8 pt[16]);

#ifdef INTERMEDIATE_VALUE_KAT
void rijndaelEncryptRound(const uint32 rk[/*4*(Nr + 1)*/], int Nr, uint8 block[16], int rounds);
void rijndaelDecryptRound(const uint32 rk[/*4*(Nr + 1)*/], int Nr, uint8 block[16], int rounds);
#endif /* INTERMEDIATE_VALUE_KAT */

#endif /* __RIJNDAEL_ALG_FST_H */
