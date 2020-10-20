default: driver.c token_stream.c token.c
	gcc driver.c token.c token_stream.c -g -o driver
