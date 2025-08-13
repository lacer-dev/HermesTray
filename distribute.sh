#!/usr/bin/env bash

function log {
  echo "$(basename "$0"): $*"
}

# replace-symlink <link>
function replace-symlink {
  local original_link="$1"
  local target_file
  local target_dir
  local temp_link
  
  if [[ ! -L "$original_link" ]]; then
    log "error: $original_link is not a symlink" >&2
    return 1;
  fi

  target_file="$(readlink "$original_link")"
  if [[ ! -e "$target_file" ]]; then
    log "error: no such file or directory: $target_file" >&2
    return 1;
  fi
  
  temp_link="$(mktemp -t "$(basename "$original_link").XXXXXXXXXX")"
  mv -f "$original_link" "$temp_link"
  target_dir="$(dirname "$original_link")"

  if ! cp -fr "$target_file" "$target_dir"; then
    mv -f "$temp_link" "$original_link"
    return 1;
  fi
  
  rm "$temp_link"
}

# build target in release mode
OUT=dist

log "running build.sh..."
if ! ./build.sh -c release -o "$OUT"; then
  log "error: build failed" >&2
  exit 1
fi

# copy resources to dist
log "copying resources to '$OUT'..."
RESOURCES=("hermes32.png")
for name in "${RESOURCES[@]}"; do
  if [[ -L "$name" ]]; then
    if ! replace-symlink "$OUT/$name"; then
      log "error: failed to copy resource '$name'" >&2
      exit 1
    fi
  else
    if ! diff -q "resources/$name" "$OUT/$name"; then
      log "error: probelm with resource that i dont feel like describing rn"
      exit 1
    fi
  fi
done
log "successfully copied resources"

log "distribution completed successfully"
exit 0
