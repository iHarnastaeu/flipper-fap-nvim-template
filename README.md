# flipper-fap-nvim-template

Scaffold a new Flipper Zero application (FAP) and configure it for Neovim
development — clangd LSP and nvim-dap debugging — in a single command.

## Usage

Create and configure a new project:

```sh
~/flipper-fap-nvim-template/setup.sh my_app
```

This creates `my_app/`, then:

1. `ufbt create APPID=my_app` — scaffolds the FAP
2. Replaces the default hello-world stub with a **runloop + UI template**:
   renders "Hello world" on screen via a draw callback, reads input from a
   message queue, and exits on the Back button
3. `ufbt` — builds once so generated headers (`my_app_icons.h`, etc.) exist
4. `ufbt vscode_dist` — generates `.vscode/compile_commands.json`
5. Symlinks `compile_commands.json` to the project root
6. Writes a `.clangd` with ARM cross-compile flags so clangd resolves the
   Flipper SDK headers (auto-detects `arm64-darwin` / `x86_64-darwin` toolchain)

Then:

```sh
cd my_app && nvim .
```

Configure an **existing** project (run from its root, no argument):

```sh
cd existing-fap
~/flipper-fap-nvim-template/setup.sh
```

This skips scaffolding and only does steps 3–6 (does not touch your source).

## Requirements

- [`ufbt`](https://github.com/flipperdevices/flipperzero-ufbt) in `PATH`
- Neovim with `clangd` (via Mason) and `nvim-dap` configured for Flipper
  (see `~/.config/nvim/lua/plugins/dap.lua`)

## Notes

- If you add image assets later or the build dir is cleaned, run `ufbt` again
  to regenerate `*_icons.h` so clangd stops reporting it as missing.
