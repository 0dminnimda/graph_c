#!/usr/bin/env bash

MAIN=$@

if [[ -z "$MAIN" ]]; then
    echo "Error: \$MAIN not set."
    exit 1
fi

for file in tests/*.input; do
    echo "Recording $file"

    base=$(basename -- "$file")
    base="${base%.*}"

    out_file="tests/${base}.output"

    $MAIN <"$file" >"$out_file" 2>&1

    if [[ $? -ne 0 ]]; then
        echo "Error: program exited abnormally $file"
    else
        echo "Completed $file"
    fi
done

