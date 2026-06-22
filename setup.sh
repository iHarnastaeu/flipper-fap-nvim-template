#!/usr/bin/env bash
set -euo pipefail

UFBT_DIR="$HOME/.ufbt"

if ! command -v ufbt &>/dev/null; then
    echo "Error: ufbt not found in PATH. Install from https://github.com/flipperdevices/flipperzero-ufbt"
    exit 1
fi

if [ ! -f "application.fam" ]; then
    echo "Error: no application.fam found. Run this script from your FAP project root."
    echo "To create a new project first: mkdir myapp && cd myapp && ufbt create APPID=myapp"
    exit 1
fi

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
echo "Done. Open the project in Neovim — clangd LSP and nvim-dap debugger are ready."
