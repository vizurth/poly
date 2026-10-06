package main

import "fmt"

func main() {
	if err := newRootCmd().Execute(); err != nil {
		fmt.Println(err)
	}
}
