#!/usr/bin/env bash

#
# detect framework path
#
DIR_SCRIPTS="$(dirname "${BASH_SOURCE[0]}")"
DIR_FRAMEWORK_ROOT="$(dirname "${DIR_SCRIPTS}")"

#
# find protobuild binary
#
DIR_BIN="${DIR_FRAMEWORK_ROOT}/bin"
PROTOBUILD_BIN="${DIR_BIN}/protobuild"

#
# prepare configuration
#
DIR_CONFIG="${DIR_FRAMEWORK_ROOT}/config"
DIR_DATA="${DIR_FRAMEWORK_ROOT}/data"

FILE_CONFIG="${DIR_CONFIG}/${EXAMPLE}.prototxt"

eval "cat <<EOF
$(<${FILE_CONFIG})
EOF
" > .tmp_pipeline.prototxt
echo "temporary config file: .tmp_pipeline.prototxt"

#
# library path
#
export LD_LIBRARY_PATH="${DIR_FRAMEWORK_ROOT}/lib"

#
# run pipeline processor
#
echo "run protobuild: \$${PROTOBUILD_BIN} -c .tmp_pipeline.prototxt "$@""
${PROTOBUILD_BIN} -c .tmp_pipeline.prototxt "$@"