package proxy

/*
#cgo CFLAGS: -I../../sw_logic/
#cgo arm LDFLAGS: -L../../sw_logic/ -lLF -Wl,-rpath=/usr/lib
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
	"net"
	"os"
	"unsafe"
)

// Server is a TCP server that takes an incoming request and sends it to another
// server, proxying the response back to the client.
type Server struct {
	// TCP address to listen on
	Addr string

	// TCP address of target server
	Target string

	// ModifyRequest is an optional function that modifies the request from a client to the target server.
	ModifyRequest func(b *[]byte)

	// ModifyResponse is an optional function that modifies the response from the target server.
	ModifyResponse func(b *[]byte)

	// TLS configuration to listen on.
	TLSConfig *tls.Config

	// TLS configuration for the proxy if needed to connect to the target server with TLS protocol.
	// If nil, TCP protocol is used.
	TLSConfigTarget *tls.Config
}

// ListenAndServe listens on the TCP network address laddr and then handle packets
// on incoming connections.
func (s *Server) ListenAndServe() error {
	listener, err := net.Listen("tcp", s.Addr)
	if err != nil {
		return err
	}
	return s.serve(listener)
}

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

// ListenAndServeTLS acts identically to ListenAndServe, except that it uses TLS
// protocol. Additionally, files containing a certificate and matching private key
// for the server must be provided if neither the Server's TLSConfig.Certificates nor
// TLSConfig.GetCertificate are populated.
func (s *Server) ListenAndServeTLS(certFile, keyFile string) error {
	s.TLSConfig = &tls.Config{
		//InsecureSkipVerify: true,
	}
	configHasCert := len(s.TLSConfig.Certificates) > 0 || s.TLSConfig.GetCertificate != nil
	if !configHasCert || certFile != "" || keyFile != "" {
		var err error

		passphrase := make([]byte, 128)
		myCharPtr := (*C.char)(unsafe.Pointer(&passphrase[0]))
		var len C.int

		C.lf_rsa_load_passphrase(myCharPtr, &len)
		passphrase = passphrase[:len]

		// load pems
		log.Printf("load cert,pem files : %v, %v", certFile, keyFile)
		certPEMBlock, err := os.ReadFile(certFile)
		if err != nil {
			return err
		}
		keyPEMBlock, err := os.ReadFile(keyFile)
		if err != nil {
			return err
		}

		decodedPriPEM, _ := pem.Decode([]byte(keyPEMBlock))
		decryptedPriPEM, err := x509.DecryptPEMBlock(decodedPriPEM, passphrase)
		if err != nil {
			return err
		}
		log.Printf("LF: the private key has ben decrypted.")

		tlsCert := tls.Certificate{}

		var certDERBlock *pem.Block
		certDERBlock, certPEMBlock = pem.Decode(certPEMBlock)
		if certDERBlock == nil {
			return errors.New("wrong cert file")
		}

		tlsCert.Certificate = append(tlsCert.Certificate, certDERBlock.Bytes)
		tlsCert.PrivateKey, err = parsePrivateKey(decryptedPriPEM)
		if err != nil {
			return err
		}

		s.TLSConfig.Certificates = make([]tls.Certificate, 1)
		s.TLSConfig.Certificates[0] = tlsCert

		log.Println("LF: cert,key loaded")
	}
	listener, err := tls.Listen("tcp", s.Addr, s.TLSConfig)
	if err != nil {
		return err
	}
	return s.serve(listener)
}

func (s *Server) serve(ln net.Listener) error {
	for {
		conn, err := ln.Accept()
		if err != nil {
			log.Println(err)
			continue
		}
		go s.handleConn(conn)
	}
}

func (s *Server) handleConn(conn net.Conn) {
	// connects to target server
	var rconn net.Conn
	var err error
	if s.TLSConfigTarget == nil {
		rconn, err = net.Dial("tcp", s.Target)
	} else {
		rconn, err = tls.Dial("tcp", s.Target, s.TLSConfigTarget)
	}
	if err != nil {
		log.Printf("%v", err)
		return
	}

	// write to dst what it reads from src
	var pipe = func(src, dst net.Conn, filter func(b *[]byte)) {
		defer func() {
			conn.Close()
			rconn.Close()
		}()

		buff := make([]byte, 65535)
		for {
			n, err := src.Read(buff)
			if err != nil {
				log.Println(err)
				return
			}
			b := buff[:n]

			if filter != nil {
				filter(&b)
			}

			_, err = dst.Write(b)
			if err != nil {
				log.Println(err)
				return
			}
		}
	}

	go pipe(conn, rconn, s.ModifyRequest)
	go pipe(rconn, conn, s.ModifyResponse)
}
