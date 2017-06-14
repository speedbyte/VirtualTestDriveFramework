#!/usr/bin/env bash

#
# detect framework path
#
DIR_SCRIPTS="$(dirname "${BASH_SOURCE[0]}")"
DIR_FRAMEWORK_ROOT="$(dirname "${DIR_SCRIPTS}")"

echo "detected framework root: ${DIR_FRAMEWORK_ROOT}"

#
# prepare configuration
#
DIR_CONFIG="${DIR_FRAMEWORK_ROOT}/config"
DIR_DATA="${DIR_FRAMEWORK_ROOT}/data"

FILE_CONFIG="${1}"

echo "detected template: ${FILE_CONFIG}"

eval "cat <<EOF
$(<${FILE_CONFIG})
EOF
" > /tmp/pipeline.prototxt
echo "temporary config file: /tmp/pipeline.prototxt"