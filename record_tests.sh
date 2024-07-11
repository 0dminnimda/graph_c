#!/usr/bin/env bash

TO_RUN=$@

if [[ -z $TO_RUN ]]; then
    echo "Error: \$MAIN not set."
    exit 1
fi

for file in tests/*.input; do
    base=$(basename -- "$file")
    base="${base%.*}"

    # First run without modifications
    echo "Recording '$file'"

    out_file="tests/${base}.output"

    $TO_RUN <"$file" >"$out_file" 2>&1

    if [[ $? -ne 0 ]]; then
        echo "Error: program exited abnormally $file"
    else
        echo "Completed $file"
    fi

    # Second run with "DEBUG\n" appended to the input
    echo "Recording '$file' (debug)"

    debug_out_file="tests/${base}.output_debug"

    $TO_RUN <<< "$(echo 'DEBUG'; cat $file)" >"$debug_out_file" 2>&1

    if [[ $? -ne 0 ]]; then
        echo "Error: program exited abnormally '$file'"
    else
        echo "Completed '$file' (debug)"
    fi
done

