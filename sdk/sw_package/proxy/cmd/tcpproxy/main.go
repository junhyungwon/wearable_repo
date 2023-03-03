package main

import (
	"crypto/tls"
	"crypto/x509"
	"flag"
	"io/ioutil"
	"log"

	"lfproxy/pkg/proxy"
	"lfproxy/pkg/tempkey"
)

var (
	// addresses
	localAddr  = flag.String("lhost", ":4444", "proxy local address")
	targetAddr = flag.String("rhost", ":80", "proxy remote address")

	// tls configuration for proxy as a server (listen)
	localTLS  = flag.Bool("ltls", false, "tls/ssl between client and proxy, you must set 'lcert' and 'lkey'")
	localCert = flag.String("lcert", "", "certificate file for proxy server side")
	localKey  = flag.String("lkey", "", "key x509 file for proxy server side")

	// tls configuration for proxy as a client (connection to target)
	targetTLS  = flag.Bool("rtls", false, "tls/ssl between proxy and target, you must set 'rcert' and 'rkey'")
	targetCert = flag.String("rcert", "", "certificate file for proxy client side")
	// targetKey  = flag.String("rkey", "", "key x509 file for proxy client side")

	// 임시키 생성을 위한 플래그
	generateTempkey = flag.Bool("generate-tempkey", false, "generate a temporal encrypted private key in /tmp/tempkey.pem")
)

func main() {
	flag.Parse()

	if *generateTempkey {
		tempkey.GenerateOpenssl() // openssl 이 좀더 빠름.
		return
	}

	p := proxy.Server{
		Addr:   *localAddr,
		Target: *targetAddr,
	}

	if *targetTLS {
		rootCAs, _ := x509.SystemCertPool()

		severCert, err := ioutil.ReadFile(*targetCert)
		if err != nil {
			log.Fatal("Could not load server certificate!")
		}
		rootCAs.AppendCertsFromPEM(severCert)

		p.TLSConfigTarget = &tls.Config{
			InsecureSkipVerify: false,
			RootCAs:            rootCAs,
		}
	}

	log.Println("Proxying from " + p.Addr + " to " + p.Target)
	if *localTLS {
		err := p.ListenAndServeTLS(*localCert, *localKey)
		if err != nil {
			log.Fatal(err)
		}
	} else {
		p.ListenAndServe()
	}
}
