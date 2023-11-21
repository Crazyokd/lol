#!/usr/bin/env bash

# assign arguments to variables
FORMAT_TOOL="${1}" && shift
SOURCES_DIR="${1}" && shift

SOURCES=$(find ${SOURCES_DIR} \
    \( -path "*build*" -o -path "*install*" -o -path ./include/color.h \) -prune \
    -o \( -name "*.c" -o -name "*.h" \) \
    -print)


if [[ ${FORMAT_TOOL} == *"clang-format"* ]];
then
    OPTS="${OPTS} -i"
    ${FORMAT_TOOL} ${OPTS} ${SOURCES} 2>/dev/null

else
    echo "Unknown static format tool: ${FORMAT_TOOL}"
    exit 1
fi
