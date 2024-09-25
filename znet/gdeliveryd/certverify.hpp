#ifndef __GNET_CERTVERIFY_HPP
#define __GNET_CERTVERIFY_HPP

#include <openssl/x509.h>

namespace GNET
{

class CertVerify
{
public:
	CertVerify(unsigned char* cert, int len);
	~CertVerify();
	
	int VerifyCert();
	int VerifyCertByCA();

	EVP_PKEY * GetPublicKey(const char *filename);
	EVP_PKEY * GetPrivateKey(const char *filename, const char *password);

	int GetBufferSize(){
		return EVP_PKEY_size(cert_pkey);
	}
	
	int Encrypt(int key_len, unsigned char* key, unsigned char* buf);
	int Decrypt(int buf_len, unsigned char* buf, unsigned char* key);   


	void GenerateRandNum(unsigned char *buf, int size);
private:
	EVP_PKEY *cert_pkey;
	EVP_PKEY *ca_pkey;
	
	X509 *x509_cert;
	X509 *x509_ca;
};

};

#endif
