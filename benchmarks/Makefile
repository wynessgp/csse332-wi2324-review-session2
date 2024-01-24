all: with_unlocks.bin without_unlocks.bin

# no dependencies here

with_unlocks.bin: with_unlocks.c
	gcc with_unlocks.c -o with_unlocks.bin

without_unlocks.bin: without_unlocks.c
	gcc without_unlocks.c -o without_unlocks.bin

# no need to run this, but it'll clear out your local directory of .bin files
clean:
	rm -rf *.bin
