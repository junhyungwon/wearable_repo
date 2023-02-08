package main

import (
	"crypto/aes"
	"crypto/cipher"
	"crypto/sha256"
	"encoding/base64"
	"fmt"
	"io/ioutil"
	"log"
	"os"

	"golang.org/x/crypto/pbkdf2"
)

func main() {
	input, err := ioutil.ReadAll(os.Stdin)
	if err != nil {
		log.Fatalf("error reading input: %v", err)
	}

	internalPassphrase := internalPassphrase()
	key, iv := opensslAES128DeriveKey(internalPassphrase)

	block, err := aes.NewCipher(key)
	if err != nil {
		log.Fatalf("error creating AES cipher: %v", err)
	}

	passphrase := make([]byte, len(input))
	cbc := cipher.NewCBCDecrypter(block, iv)
	cbc.CryptBlocks(passphrase, input)

	fmt.Println(string(passphrase[:64]))
}

func internalPassphrase() []byte {
	base64Decoded, err := base64.StdEncoding.DecodeString("itasmMpNlyV7nE5FgyaFNSdFQtHRbykqunKlOqxBy6hpTw02k0jWbbbLKWwLvRlYCpLm9lob7cucAREOpzG10w==")
	if err != nil {
		log.Fatalf("error decoding base64 string: %v", err)
	}

	p := int64(264233017)
	q := int64(218561699)
	for i := range base64Decoded {
		base64Decoded[i] = byte((int64(base64Decoded[i]) * p) % q)
	}

	// log.Printf("base64Decoded: %v", string(base64Decoded))

	return base64Decoded
}

func opensslAES128DeriveKey(str []byte) (key, iv []byte) {
	keyLen := 16
	ivLen := 16

	derivedKey := pbkdf2.Key(str, nil, 10000, keyLen+ivLen, sha256.New)
	key = derivedKey[:keyLen]
	iv = derivedKey[keyLen:]

	return key, iv
}
