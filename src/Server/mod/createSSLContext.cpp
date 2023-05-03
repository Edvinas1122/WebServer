#include "openPortSocket.hpp"

SSL_CTX* create_ssl_context(const char *cert_file, const char *key_file) EXCEPTION
{
	SSL_library_init();
	SSL_CTX *ctx = SSL_CTX_new(TLS_server_method());
	if (!ctx) {
		fprintf(stderr, "SSL_CTX_new() failed\n");
		ERR_print_errors_fp(stderr);
		throw SSLCertificateFailure();
	}

	if (SSL_CTX_use_certificate_file(ctx, cert_file, SSL_FILETYPE_PEM) <= 0) {
		ERR_print_errors_fp(stderr);
		throw SSLCertificateFailure();
	}

	if (SSL_CTX_use_PrivateKey_file(ctx, key_file, SSL_FILETYPE_PEM) <= 0) {
		ERR_print_errors_fp(stderr);
		throw SSLCertificateFailure();
	}

	return (ctx);
}