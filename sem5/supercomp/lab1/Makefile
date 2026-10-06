BINARY := build/bf16

.PHONY: all test clean

all: $(BINARY)

$(BINARY): $(shell find cmd internal -name '*.go') go.mod go.sum
	mkdir -p build
	go build -o $(BINARY) ./cmd/bf16

test:
	go test ./...

clean:
	rm -rf build
