#!/usr/bin/env bash

# assign arguments to variables
FORMAT_TOOL="${1}" && shift
BUILD_DIR="${1}" && shift
SOURCES_DIR="${1}" && shift

SOURCES=$(find ${SOURCES_DIR}/src ${SOURCES_DIR}/module ${SOURCES_DIR}/lib \
    -not -path *build* \
    -name *.cpp -o -name *.cc)


if [[ ${FORMAT_TOOL} == *"clang-format"* ]];
then
    OPTS="${OPTS} -i"
    ${FORMAT_TOOL} ${OPTS} ${SOURCES} 2>/dev/null

else
    echo "Unknown static format tool: ${FORMAT_TOOL}"
    exit 1
fi
