#!/usr/bin/env bash
set -euo pipefail

UFBT_DIR="$HOME/.ufbt"

if ! command -v ufbt &>/dev/null; then
    echo "Error: ufbt not found in PATH. Install from https://github.com/flipperdevices/flipperzero-ufbt"
    exit 1
fi

APPID="${1:-}"

# Two modes:
#   setup.sh <appid>   -> create a new project dir named <appid>, scaffold it, then configure
#   setup.sh           -> configure the existing project in the current directory
if [ -n "$APPID" ]; then
    if [ -e "$APPID" ]; then
        echo "Error: '$APPID' already exists. Pick a different appid or remove it first."
        exit 1
    fi
    echo "==> creating new project '$APPID'"
    mkdir "$APPID"
    cd "$APPID"
    ufbt create "APPID=$APPID"
else
    if [ ! -f "application.fam" ]; then
        echo "Usage: setup.sh <appid>            # create + configure a new FAP project"
        echo "       setup.sh                     # configure existing project (run from its root)"
        exit 1
    fi
fi

echo "==> ufbt build (generates *_icons.h and other build-time headers)"
ufbt

echo "==> ufbt vscode_dist (generates .vscode/compile_commands.json)"
ufbt vscode_dist

echo "==> symlinking compile_commands.json"
ln -sf .vscode/compile_commands.json compile_commands.json

# Detect native toolchain arch; prefer arm64-darwin on Apple Silicon
if [ -d "$UFBT_DIR/toolchain/arm64-darwin/arm-none-eabi/include" ]; then
    ARM_INCLUDE="$UFBT_DIR/toolchain/arm64-darwin/arm-none-eabi/include"
elif [ -d "$UFBT_DIR/toolchain/x86_64-darwin/arm-none-eabi/include" ]; then
    ARM_INCLUDE="$UFBT_DIR/toolchain/x86_64-darwin/arm-none-eabi/include"
else
    echo "Warning: arm-none-eabi/include not found under $UFBT_DIR/toolchain — .clangd isystem path left empty"
    ARM_INCLUDE=""
fi

echo "==> writing .clangd (toolchain: $ARM_INCLUDE)"
cat > .clangd << EOF
CompileFlags:
  Add:
    - --target=arm-none-eabi
$([ -n "$ARM_INCLUDE" ] && echo "    - -isystem$ARM_INCLUDE")
    - -Wno-unknown-warning-option
  Remove:
    - -mword-relocations
    - -mlong-calls
    - -mfloat-abi=*
    - -mfpu=*
    - -Wa,*
Diagnostics:
  UnusedIncludes: None
EOF

echo ""
echo "Done. cd $([ -n "$APPID" ] && echo "$APPID " || echo "")&& nvim . — clangd LSP and nvim-dap debugger are ready."
