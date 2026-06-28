#!/usr/bin/env bash
set -e

repo_dir="$(cd "$(dirname "$0")/.." && pwd)"
cmd_file="$repo_dir/tests/mybash_runtime_cmd.sh"

cat > "$cmd_file" <<EOF
myfile $repo_dir/exercises/20_mybash/bin/mybash
myfile $repo_dir/exercises/20_mybash/obj/mybash/main.o

mysed s/unix/linux/ "unix is opensource. unix is free os."

mytrans $repo_dir/exercises/20_mybash/src/mytrans/text.txt

mywc $repo_dir/exercises/20_mybash/src/mytrans/text.txt
EOF

"$repo_dir/exercises/20_mybash/bin/mybash" "$cmd_file"
