all: build

build:
	@echo "Building and running......"
	gcc shell.c -o shell

binaries:
	@echo "Building ls..."
	gcc ls.c -o ls

clean:
	@echo "Cleaning....."
	rm shell
