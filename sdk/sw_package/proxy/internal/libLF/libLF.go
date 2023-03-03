package libLF

/*
#cgo CFLAGS: -I../../../../sw_logic/
#cgo arm LDFLAGS: -L../../../../sw_logic/ -lLF -Wl,-rpath=/usr/lib
#include "lf.h"
*/
import "C"
import (
	"crypto"
	"crypto/ecdsa"
	"crypto/ed25519"
	"crypto/rsa"
	"crypto/tls"
	"crypto/x509"
	"encoding/pem"
	"errors"
	"log"
	"os"
	"unsafe"
)

// from tls.go
func parsePrivateKey(der []byte) (crypto.PrivateKey, error) {
	if key, err := x509.ParsePKCS1PrivateKey(der); err == nil {
		return key, nil
	}
	if key, err := x509.ParsePKCS8PrivateKey(der); err == nil {
		switch key := key.(type) {
		case *rsa.PrivateKey, *ecdsa.PrivateKey, ed25519.PrivateKey:
			return key, nil
		default:
			return nil, errors.New("tls: found unknown private key type in PKCS#8 wrapping")
		}
	}
	if key, err := x509.ParseECPrivateKey(der); err == nil {
		return key, nil
	}

	return nil, errors.New("tls: failed to parse private key")
}

func Passphrase() ([]byte, error) {
	passphrase := make([]byte, 32*2+16) // SHA256_DIGEST_LENGTH*2+BLOCK_SIZE
	myCharPtr := (*C.char)(unsafe.Pointer(&passphrase[0]))
	var len C.int

	C.lf_rsa_load_passphrase(myCharPtr, &len)
	passphrase = passphrase[:len]

	return passphrase, nil
}

func LoadKeyPairs(certFile, keyFile string) (*tls.Certificate, error) {
	passphrase, _ := Passphrase()
	tlsCert := tls.Certificate{}

	// load pems
	log.Printf("load cert,pem files : %v, %v", certFile, keyFile)
	certPEMBlock, err := os.ReadFile(certFile)
	if err != nil {
		return nil, err
	}
	keyPEMBlock, err := os.ReadFile(keyFile)
	if err != nil {
		return nil, err
	}

	decodedPriPEM, _ := pem.Decode([]byte(keyPEMBlock))
	decryptedPriPEM, err := x509.DecryptPEMBlock(decodedPriPEM, passphrase)
	if err != nil {
		return nil, err
	}
	log.Printf("LF: the private key has ben decrypted.")

	var certDERBlock *pem.Block
	certDERBlock, certPEMBlock = pem.Decode(certPEMBlock)
	if certDERBlock == nil {
		return nil, errors.New("wrong cert file")
	}

	tlsCert.Certificate = append(tlsCert.Certificate, certDERBlock.Bytes)
	tlsCert.PrivateKey, err = parsePrivateKey(decryptedPriPEM)
	if err != nil {
		return nil, err
	}

	return &tlsCert, nil
}
