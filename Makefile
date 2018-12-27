### Reminder -------------------------------------------------------------------
#
#	$@ 	target
#	$^	all prerequisites (without | order-only prerequisites)
#	$<	first prerequisite

### Variables ------------------------------------------------------------------

B :=			bin
S :=			src
L :=			src/libds1302
T :=			tmp

NODE_BIN :=		node_modules/.bin
MD_TO_MAN :=	./${NODE_BIN}/marked-man --gfm --breaks

CCFLAGS :=		"-iquote$L" -lwiringPi


### Tasks ----------------------------------------------------------------------

.PHONY: default
default: build

.PHONY: all
all: build lib docs

.PHONY: build
build: $B/ds1302

.PHONY: lib
lib: $T/libds1302.so

.PHONY: run
run: $B/ds1302
	sudo ./$B/ds1302

.PHONY: docs
docs: $T/ds1302.1

.PHONY: man
man: docs

.PHONY: clean
clean:
	rm -rf "$B" "$T"


### Setup ----------------------------------------------------------------------

$B:
	mkdir -p "$@"

$T:
	mkdir -p "$@"


### Binary Targets -------------------------------------------------------------

$B/ds1302: $T/ds1302.o $T/libds1302.o | $B
	$(CC) ${CCFLAGS} -o "$@" $^

$T/libds1302.so: $T/libds1302.o | $T
	$(CC) ${CCFLAGS} -shared -o "$@" $^

$T/ds1302.o: $S/ds1302.c | $T
	$(CC) ${CCFLAGS} -o "$@" -c "$<"

$T/libds1302.o: $L/libds1302.c $L/libds1302.h | $T
	$(CC) ${CCFLAGS} -Wall -Werror -fPIC -o "$@" -c "$<"


### Documentation targets ------------------------------------------------------

$T/ds1302.1: README.md | $T
	${MD_TO_MAN} \
		--version "`cat VERSION`" \
		--manual "Command-Line access to DS1302 Real-time-clock" \
		"$<" > "$@"
