.PHONY: *

build:
	clang -O2 -Wall -framework ApplicationServices -o ctrl2esc main.c
	./build_app.sh
