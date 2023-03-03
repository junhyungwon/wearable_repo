package tempkey

import (
	"crypto/rand"
	"crypto/rsa"
	"crypto/x509"
	"encoding/pem"
	"fmt"
	"log"
	"os"
	"os/exec"

	"lfproxy/internal/libLF"
)

// 주의 : S50lighttpd 와 cgi.h 위치 같아야 함.
const PRIVATE_KEY string = "/tmp/tempkey.pem"
const PUBLIC_KEY string = "/tmp/tempkey.pub"
const RSA_BIT string = "2048"

func GenerateOpenssl() {
	log.Println("[tempkey] generating a temporal private key by openssl...")
	passphrase, _ := libLF.Passphrase()

	args := []string{"genrsa", "-aes128", "-passout", "pass:" + string(passphrase), "-out", PRIVATE_KEY, RSA_BIT}
	cmd := exec.Command("/usr/bin/openssl", args...)
	output, err := cmd.Output()
	if err != nil {
		log.Println("Error:", err)
		return
	}
	log.Println(string(output))

	log.Println("[tempkey] generating a paired public key by openssl...")
	file, err := os.Create(PUBLIC_KEY)
	if err != nil {
		fmt.Println("Error:", err)
		return
	}
	defer file.Close()

	args = []string{"rsa", "-in", PRIVATE_KEY, "-passin", "pass:" + string(passphrase), "-pubout"}
	cmd = exec.Command("/usr/bin/openssl", args...)
	cmd.Stdout = file

	err = cmd.Run()
	if err != nil {
		fmt.Println("Error:", err)
		return
	}

	log.Println("[tempkey] Private key and public key generated successfully")
}

func Generate() {
	log.Println("[tempkey] generating a temporal private key...")
	passphrase, _ := libLF.Passphrase()

	// Generate RSA key pair
	privateKey, err := rsa.GenerateKey(rand.Reader, 2048)
	if err != nil {
		panic(err)
	}

	// Create PEM block for encrypted private key
	privateKeyPEMBlock, err := x509.EncryptPEMBlock(
		rand.Reader,
		"RSA PRIVATE KEY",
		x509.MarshalPKCS1PrivateKey(privateKey),
		passphrase,
		x509.PEMCipherAES128,
	)
	if err != nil {
		panic(err)
	}

	// Save encrypted private key to file
	privateKeyFile, err := os.Create(PRIVATE_KEY)
	if err != nil {
		panic(err)
	}
	defer privateKeyFile.Close()
	if err := pem.Encode(privateKeyFile, privateKeyPEMBlock); err != nil {
		panic(err)
	}

	// Create PEM block for public key
	publicKeyPEMBlock := pem.Block{
		Type:  "RSA PUBLIC KEY",
		Bytes: x509.MarshalPKCS1PublicKey(&privateKey.PublicKey),
	}

	// Save public key to file
	publicKeyFile, err := os.Create(PUBLIC_KEY)
	if err != nil {
		panic(err)
	}
	defer publicKeyFile.Close()
	if err := pem.Encode(publicKeyFile, &publicKeyPEMBlock); err != nil {
		panic(err)
	}

	log.Println("[tempkey] Private key and public key generated successfully")
}
