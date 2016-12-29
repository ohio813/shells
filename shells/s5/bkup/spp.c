/**
  Copyright © 2016 Odzhan. All Rights Reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are
  met:

  1. Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.

  3. The name of the author may not be used to endorse or promote products
  derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY AUTHORS "AS IS" AND ANY EXPRESS OR
  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE. */

#include "spp.h"

/**F*********************************************
 *
 * entrypoint of PIC
 *
 ************************************************/
BIGNUM *BN_lebin2bn(const uint8_t *s, int len, BIGNUM *ret)
{
  unsigned int i, m;
  unsigned int n;
  BN_ULONG l;
  BIGNUM *bn = NULL;

  if (ret == NULL)
      ret = bn = BN_new();
  if (ret == NULL)
      return (NULL);
  bn_check_top(ret);
  s += len - 1;
  /* Skip trailing zeroes. */
  for ( ; len > 0 && *s == 0; s--, len--)
      continue;
  n = len;
  if (n == 0) {
      ret->top = 0;
      return ret;
  }
  i = ((n - 1) / BN_BYTES) + 1;
  m = ((n - 1) % (BN_BYTES));
  if (bn_wexpand(ret, (int)i) == NULL) {
      BN_free(bn);
      return NULL;
  }
  ret->top = i;
  ret->neg = 0;
  l = 0;
  while (n--) {
      l = (l << 8L) | *(s--);
      if (m-- == 0) {
          ret->d[--i] = l;
          l = 0;
          m = BN_BYTES - 1;
      }
  }
  /*
   * need to call this due to clear byte at top if avoiding having the top
   * bit set (-ve number)
   */
  bn_correct_top(ret);
  return ret;
}

/**F*********************************************
 *
 * entrypoint of PIC
 *
 ************************************************/
void spp_showkeys(spp_ctx *c)
{
  int i;
  for (i=0; i<64; i++) {
    printf ("%02x", ((uint8_t*)&c->tek)[i]);
  }
}

void spp_dump(void *tek) {
#if defined(DEBUG) && DEBUG > 0
  int i;
  for (i=0; i<64; i++) {
    printf ("%02x", ((uint8_t*)tek)[i]);
  }
#endif
}

/**F*********************************************
 *
 * entrypoint of PIC
 *
 ************************************************/
int spp_rand (spp_ctx *c, void *out, uint32_t outlen)
{
  BIGNUM *r = BN_new();
  
  // create random bits
  BN_rand(r, outlen*8, -1, 0);
  BN_bn2bin(r, out);
  BN_free(r);
  
  return 1;
}

/**F*********************************************
 *
 * entrypoint of PIC
 *
 ************************************************/
void spp_pad (spp_ctx *c, spp_blk *in)
{
  in->padlen = (in->buflen & (SPP_BLK_LEN-1));
  
  // if not zero
  if (in->padlen) {
    // calculate how much padding bytes required
    in->padlen = (SPP_BLK_LEN - in->padlen);
    // generate random bytes
    spp_rand (c, &in->buf[in->buflen], in->padlen);
    // update block len to include padding
    in->buflen += in->padlen;
  }
}

/**F*********************************************
 *
 * TODO: sign keys
 *
 ************************************************/
int spp_sign (spp_ctx *c, spp_blk *in)
{
  // 
  return 1;
}

/**F*********************************************
 *
 * TODO: verify keys
 *
 ************************************************/
int spp_verify (spp_ctx *c, spp_blk *in)
{
  //
  return 1;
}

/**F*********************************************
 *
 * Generate MAC of encrypted packet
 *
 ************************************************/
int spp_mac (spp_ctx *c, void *in, uint32_t inlen, void *mac)
{
  SHA3_CTX ctx;
  uint8_t  m[SHA3_256];
  
  SHA3_Init(&ctx, SHA3_256);
  SHA3_Update(&ctx, c->tek.mkey, SPP_MKEY_LEN);
  SHA3_Update(&ctx, in, inlen);
  SHA3_Final(m, &ctx);
  
  memcpy(mac, m, SPP_MAC_LEN);
  return 1;
}

/**F*********************************************
 *
 * xor block of plaintext with ciphertext
 *
 ************************************************/
uint32_t memxor (uint8_t *pt, uint8_t *ct, uint32_t len)
{
  uint32_t i;
  
  len=(len>SPP_BLK_LEN) ? SPP_BLK_LEN:len;
  
  for (i=0; i<len; i++) {
    pt[i] ^= ct[i];
  }
  return len;
}

/**F*********************************************
 *
 * increase the IV
 *
 ************************************************/
void update_iv (uint8_t *iv)
{
  int i;
  
  for (i=SPP_IV_LEN-1; i>=0; i--) {
    iv[i]++;
    if (iv[i]) {
      break;
    }
  }
}

/**F*********************************************
 *
 * Encrypt packet, use CTR mode
 *
 ************************************************/
int spp_crypt (spp_ctx *c, void *in, uint32_t len)
{
  uint32_t r, inlen=len;
  uint8_t  *p=(uint8_t*)in;
  uint8_t  iv[SPP_IV_LEN];
  
  while (inlen)
  {
    // copy iv into temp space
    memcpy (iv, c->tek.iv, SPP_IV_LEN);
    
    // encrypt with AES-256 in ECB mode
    aes_encrypt (&c->ctx, iv, AES_ENCRYPT);
      
    // xor against plaintext
    r=memxor (p, iv, inlen);
    
    // update iv
    update_iv(c->tek.iv);
    
    // advance plaintext position
    p += r;
    
    // decrease length of input
    inlen -= r;
  }
  return r;
}

/**F*********************************************
 *
 * send packet, fragmented if required
 *
 ************************************************/
int send_pkt (int s, void *in, uint32_t inlen)
{
  int      len;
  uint32_t sum;
  uint8_t  *p=(uint8_t*)in;
  
  for (sum=0; sum<inlen; sum += len) {
    len=send (s, &p[sum], inlen - sum, 0);
    if (len<=0) return -1;
  }
  return sum;
}

/**F*********************************************
 *
 * send packets
 *
 ************************************************/
int spp_send (spp_ctx *c, spp_blk *in)
{
  int      len;
  uint32_t inlen;

  // pad message if required
  spp_pad (c, in);
  
  inlen=in->buflen;
  
  if (c->secure) {
    // add mac only if data present
    if (inlen!=0) {
      in->buflen += SPP_MAC_LEN;
    }
    // encrypt the length first
    // no authentication of length
    spp_crypt (c, &in->len, sizeof(uint32_t));
    
    // then remainder of data
    if (inlen!=0) {
      spp_crypt (c, &in->buf, inlen);
      
      // add mac of encrypted data
      spp_mac (c, in->buf, inlen, &in->buf[inlen]);
      inlen += SPP_MAC_LEN;
    }
  }

  if (c->secure) {
    DEBUG_PRINT("encrypted length = %08X", in->len);
  }
  
  // send data + len
  len=send_pkt (c->s, &in->len, inlen + sizeof(uint32_t));
  if (len<=0) return SPP_ERR_SCK;
  
  return SPP_ERR_OK;
}

/**F*********************************************
 *
 * receive packet, fragmented if required
 *
 ************************************************/
int recv_pkt (int s, void *out, uint32_t outlen) 
{
  int      len;
  uint32_t sum;
  uint8_t  *p=(uint8_t*)out;

  for (sum=0; sum<outlen; sum += len) {
    len=recv (s, &p[sum], outlen - sum, 0);
    if (len<=0) return -1;
  }
  return sum;
}

/**F*********************************************
 *
 * entrypoint of PIC
 *
 ************************************************/
int spp_recv (spp_ctx *c, spp_blk *out)
{
  int     len;
  uint8_t mac[SPP_MAC_LEN];
  
  memset (out, 0, sizeof(spp_blk));
  
  // receive the length first
  len=recv_pkt (c->s, &out->len, sizeof(uint32_t));

  if (len<=0) {
    return SPP_ERR_SCK;
  }
  
  if (c->secure) {
    // decrypt length
    spp_crypt (c, &out->len, sizeof(uint32_t));
  }
  
  // zero buffer is okay
  if (out->buflen==0) {
    return SPP_ERR_OK;
  }

  // does buffer len exceed what's acceptable?
  if (out->buflen > SPP_DATA_LEN+SPP_MAC_LEN) {
    return SPP_ERR_LEN;
  }

  // padding shouldn't equal or exceed block length
  if (out->padlen >= SPP_BLK_LEN) {
    return SPP_ERR_LEN;
  }
  
  // receive the data
  len=recv_pkt (c->s, &out->buf, out->buflen);
  if (len<=0) return SPP_ERR_SCK;

  if (c->secure) {
    // subtract mac len to get data len
    out->buflen -= SPP_MAC_LEN;
    // generate mac
    spp_mac (c, out->buf, out->buflen, mac);
    // verify they match
    if (memcmp (mac, &out->buf[out->buflen], SPP_MAC_LEN)!=0) {
      return SPP_ERR_MAC;
    }
    // decrypt
    spp_crypt (c, &out->buf, out->buflen);
  }
  // remove any padding
  out->buflen -= out->padlen;
  out->padlen = 0;
  
  out->buf[out->buflen]=0;
  
  return (len<=0) ? SPP_ERR_SCK : SPP_ERR_OK;
}

/**F*********************************************
 *
 * entrypoint of PIC
 *
 ************************************************/
int spp_readkey (spp_ctx *c, spp_blk *key)
{
  //
  return 1;
}

/**F*********************************************
 *
 * initialize spp
 *
 ************************************************/
int spp_init (spp_ctx *c, int mode)
{
  RSA     *rsa;
  int     r = 1;
  c->secure = 0;
  c->mode   = mode;
  
  c->e      = BN_new();
  c->d      = BN_new();
  c->n      = BN_new();
  c->bn_ctx = BN_CTX_new();
  
  BN_dec2bn(&c->e, "65537");
  
  // server mode?
  if (c->mode==SPP_SERVER) 
  {
    rsa = RSA_new();
    // generate RSA key pair    
    // set public exponent and generate key pair
    RSA_generate_key_ex(rsa, SPP_RSA_LEN, c->e, NULL);
    
    BN_copy(c->d, rsa->d);
    BN_copy(c->n, rsa->n);
    
    RSA_free(rsa);
  }
  return r;
}

/**F*********************************************
 *
 * entrypoint of PIC
 *
 ************************************************/
int spp_genkey (spp_ctx *c, spp_blk *out)
{
  return 1;
}

/**F*********************************************
 *
 * entrypoint of PIC
 *
 ************************************************/
int spp_getkey(spp_ctx *c, spp_blk *out)
{
  uint8_t  rnd[32];
  int      i, rs;
  SHA3_CTX ctx;
  BIGNUM   *keys, *enc;
  
  if (c->mode==SPP_SERVER)
  {
    // get length of modulus in bytes
    out->buflen=BN_num_bytes(c->n);
    
    // copy public key to buffer
    //BN_bn2bin(c->n, out->buf);
    memcpy (out->buf, c->n->d, out->buflen);
    
    DEBUG_PRINT ("\nModulus is %s", BN_bn2hex(c->n));
    
    // attach signature for client to verify
    rs=spp_sign (c, out);
  } else {
    // get 256-bits of pseudo random bytes
    spp_rand (c, rnd, sizeof(rnd));
    memset (&c->tek, 0, sizeof(spp_tek));
    // get 512-bit hash
    SHA3_Init(&ctx, SHA3_512);
    SHA3_Update(&ctx, rnd, sizeof(rnd));
    SHA3_Final(&c->tek, &ctx);
    // set 256-bit key for aes
    aes_setkey(&c->ctx, c->tek.ekey);
    // encrypt with public key
    keys = BN_new();
    enc  = BN_new();
    
    BN_bin2bn((uint8_t*)&c->tek, 64, keys);
    DEBUG_PRINT ("\nSession keys are %s", BN_bn2hex(keys));
    
    BN_mod_exp(enc, keys, c->e, c->n, c->bn_ctx);
    DEBUG_PRINT ("\nEncrypted keys are %s", BN_bn2hex(enc));
    
    BN_bn2bin(enc, out->buf);
    out->buflen = BN_num_bytes(enc);
    
    BN_free(keys);
    BN_free(enc);
    rs=1;
  }
  return rs;
}

/**F*********************************************
 *
 * entrypoint of PIC
 *
 ************************************************/
int spp_setkey(spp_ctx *c, spp_blk *in)
{
  int    r=1, i;
  BIGNUM *keys, *enc;

  if (c->mode==SPP_SERVER)
  {
    // import session keys
    enc  = BN_new();
    keys = BN_new();
    
    DEBUG_PRINT ("\nreceived %i bytes", in->buflen);
    // use little endian function for PIC
    BN_lebin2bn (in->buf, in->buflen, enc);
    DEBUG_PRINT ("\nEncrypted keys are %s", BN_bn2hex(enc));
    
    // decrypt using private key
    BN_mod_exp (keys, enc, c->d, c->n, c->bn_ctx);
    
    DEBUG_PRINT ("\nSession keys are %s", BN_bn2hex(keys));
    // copy 512-bits of key
    memcpy ((uint8_t*)&c->tek, keys->d, 64);
    //BN_bn2bin (keys, (uint8_t*)&c->tek);
    aes_setkey (&c->ctx, c->tek.ekey);
    
    BN_free(enc);
    BN_free(keys);
  } else {
    // verify signature
    r=spp_verify (c, in);
    if (r)
    {
      // import public key
      BN_bin2bn (in->buf, in->buflen, c->n);
      DEBUG_PRINT ("\nmodulus is %s", BN_bn2hex(c->n));
      r=1;
    }
  }
  return r;
}

/**F*********************************************
 *
 * entrypoint of PIC
 *
 ************************************************/
int spp_handshake (spp_ctx *c)
{
  int     r=0;
  spp_blk pubkey, seskey, iv;
  
  if (c->mode==SPP_SERVER)
  {
    // get the public key
    printf ("[ getting public key\n");
    if (spp_getkey(c, &pubkey))
    {
      // send to client
      printf ("[ sending public key\n");
      if (spp_send(c, &pubkey)==SPP_ERR_OK)
      {
        // wait for session key
        printf ("[ receiving session keys\n");
        if (spp_recv(c, &seskey)==SPP_ERR_OK)
        {
          // import it
          printf ("[ importing session keys\n");
          r=spp_setkey(c, &seskey);
        }
      }
    }
  } else {
    // wait for public key
    printf ("[ waiting for public key\n");
    if (spp_recv(c, &pubkey)==SPP_ERR_OK)
    {
      // import it
      printf ("[ importing public key\n");
      if (spp_setkey(c, &pubkey))
      {
        // generate session key
        printf ("[ generating session key\n");
        if (spp_getkey(c, &seskey))
        {
          // send session key
          printf ("[ sending session keys\n");
          if (spp_send(c, &seskey)==SPP_ERR_OK)
          {
            r=1;
          }
        }
      }
    }
  }
  // if no error, set secure flag
  if (r) { 
    //spp_showkeys(c);
    c->secure=1; 
  }
  return r;
}

/**F*********************************************
 *
 * entrypoint of PIC
 *
 ************************************************/
void spp_end (spp_ctx *c)
{
  DEBUG_PRINT ("\nfreeing d");
  BN_free(c->d);
    
  DEBUG_PRINT ("\nfreeing e");
  BN_free(c->e);
    
  DEBUG_PRINT ("\nfreeing ctx");
  BN_CTX_free(c->bn_ctx);
}

