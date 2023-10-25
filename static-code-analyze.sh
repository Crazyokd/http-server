#!/usr/bin/env bash

# assign arguments to variables
ANALYZE_TOOL="${1}" && shift
BUILD_DIR="${1}" && shift
SOURCES_DIR="${1}" && shift

SOURCES=$(find ${SOURCES_DIR}/src ${SOURCES_DIR}/module ${SOURCES_DIR}/lib \
    -not -path *build* \
    -name *.c -o -name *.cpp -o -name *.cc)


if [[ ${ANALYZE_TOOL} == *"clang-tidy"* ]];
then
    if [[ ${ANALYZE_TOOL} == *"run-clang-tidy"* ]]; then
        OPTS="${OPTS} -j $(nproc)"
    fi

    OPTS="${OPTS} -quiet"
    OPTS="${OPTS} -p=${BUILD_DIR}"

    ${ANALYZE_TOOL} ${OPTS} ${SOURCES} 2>/dev/null

elif [[ ${ANALYZE_TOOL} == *"cppcheck"* ]];
then
    OPTS="${OPTS} -j $(nproc)"
    OPTS="${OPTS} --enable=all"
    OPTS="${OPTS} --verbose"
    OPTS="${OPTS} --suppress=variableScope"
    OPTS="${OPTS} --suppress=preprocessorErrorDirective"
    OPTS="${OPTS} --suppress=unusedVariable"
    OPTS="${OPTS} --suppress=unmatchedSuppression"
    OPTS="${OPTS} --output-file=cppchecklog.log"
    OPTS="${OPTS} --project=${BUILD_DIR}/compile_commands.json"

    ${ANALYZE_TOOL} ${OPTS} ${SOURCES}

else
    echo "Unknown static code analysis tool: ${ANALYZE_TOOL}"
    exit 1
fi
