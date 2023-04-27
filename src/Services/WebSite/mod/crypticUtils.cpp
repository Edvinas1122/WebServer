#include "contentUtils.hpp"

#include <openssl/sha.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/bio.h>
#include <openssl/evp.h>

std::string generateWebSocketAccept(const std::string& key)
{
    const std::string magic = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
    std::string combined = key + magic;
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1(reinterpret_cast<const unsigned char*>(combined.c_str()), combined.length(), hash);
    BIO *b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    BIO *bmem = BIO_new(BIO_s_mem());
    BIO_push(b64, bmem);
    BIO_write(b64, hash, SHA_DIGEST_LENGTH);
    BIO_flush(b64);
    BUF_MEM *bptr;
    BIO_get_mem_ptr(b64, &bptr);
    std::string result(bptr->data, bptr->length);
    BIO_free_all(b64);
    return result;
}

// std::string decryptAES(const std::string &encrypted, const char *aesKey) {
//     std::string decrypted;

//     // Create AES decryption context
//     AES_KEY aesKey;
//     AES_set_decrypt_key(aes_key, 256, &aesKey);

//     // Allocate memory for decrypted data
//     int len = encrypted.size();
//     unsigned char *decryptedData = new unsigned char[len];

//     // Decrypt the data
//     AES_cbc_encrypt((unsigned char *)encrypted.data(), decryptedData, len, &aesKey, aes_iv, AES_DECRYPT);

//     // Copy decrypted data to string
//     decrypted.assign((const char *)decryptedData, len);

//     // Free memory
//     delete[] decryptedData;

//     return decrypted;
// }

#include <cstdlib>
#include <ctime>

std::string generate_random_string() {
  std::string result;
  const std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  std::srand(std::time(NULL));

  for (int i = 0; i < 4; i++) {
    result += charset[std::rand() % charset.size()];
  }

  return result;
}
