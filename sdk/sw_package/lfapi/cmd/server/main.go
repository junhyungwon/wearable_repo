package main

import (
	"fmt"
	"log"
	"os"

	"lfapi/pkg/cmd"
)

func main() {
	// enable line numbers
	log.SetFlags(log.LstdFlags | log.Lshortfile)

	if err := cmd.RunServer(); err != nil {
		fmt.Fprintf(os.Stderr, "%v\n", err)
		os.Exit(1)
	}
}
