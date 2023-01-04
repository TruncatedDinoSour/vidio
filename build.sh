#!/usr/bin/env bash

set -xe

GENERIC_FLAGS="-std=c89 -Wall -Wextra -Wpedantic -Wshadow -Werror -pedantic -march=native -mtune=native -pipe -o ../vidio.elf main.c"

main() {
    CC="${CC:-clang}"

    cd src

    if [ "$DEBUG" ]; then
        $CC -g $GENERIC_FLAGS
    else
        $CC -flto -Ofast -ffunction-sections -fdata-sections -s $GENERIC_FLAGS
        strip --strip-all --remove-section=.note --remove-section=.gnu.version --remove-section=.comment --strip-debug --strip-unneeded ../vidio.elf -o ../vidio.elf
    fi

    cd ..

    if [ "$1" = '-r' ]; then
        ./vidio.elf "${@:2}"
    fi
}

main "$@"
