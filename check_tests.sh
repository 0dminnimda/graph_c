#!/usr/bin/env bash

diff_output_normal_output_debug() {
    diff "$1" "$2"
}

TO_RUN=$@

if [[ -z $TO_RUN ]]; then
    echo "Error: \$MAIN not set."
    exit 1
fi

for file in tests/*.input; do

    base=$(basename -- "$file")
    base="${base%.*}"

    # First run without modifications
    echo "Checking '$file'"
    output_normal=$(mktemp)
    out_file="tests/${base}.output"
    $TO_RUN <"$file" >"$output_normal" 2>&1

    if [[ $? -ne 0 ]]; then
        echo "Error: program exited abnormally $file"
    else
        echo "Completed '$file'"
    fi

    diff_output_normal_output_debug "$output_normal" "$out_file"

    # Second run with "DEBUG\n" appended to the input
    echo "Checking '$file' (debug)"
    output_debug=$(mktemp)
    debug_out_file="tests/${base}.output_debug"
    $TO_RUN <<< "$(echo 'DEBUG'; cat $file)" >"$output_debug" 2>&1

    if [[ $? -ne 0 ]]; then
        echo "Error: program exited abnormally '$file'"
    else
        echo "Completed '$file' (debug)"
    fi

    # Compare the outputs
    diff_output_normal_output_debug "$output_debug" "$debug_out_file"

    # Cleanup temporary files
    rm "$output_normal" "$output_debug"
done

