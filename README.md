# flipper-fap-nvim-template

Scaffold a new Flipper Zero application (FAP) with a real, layered architecture
and configure it for Neovim development — clangd LSP and nvim-dap debugging — in
a single command.

## Install

```sh
git clone https://github.com/iHarnastaeu/flipper-fap-nvim-template.git ~/flipper-fap-nvim-template
```

The examples below assume that location; adjust the path if you clone elsewhere.

## Usage

Create and configure a new project (architected template, the default):

```sh
~/flipper-fap-nvim-template/setup.sh my_app
cd my_app && nvim .
```

Create a minimal single-file project instead:

```sh
~/flipper-fap-nvim-template/setup.sh --minimal my_app
```

Configure an **existing** project (run from its root, no argument — does not
touch your source):

```sh
cd existing-fap
~/flipper-fap-nvim-template/setup.sh
```

## What the script does

1. `ufbt create APPID=my_app` — scaffolds the FAP (and a valid app icon)
2. Overlays the chosen template (architected or `--minimal`), replacing
   `__APPID__` / `__APP_NAME__` tokens
3. `ufbt` — builds once so generated headers exist
4. `ufbt vscode_dist` — generates `.vscode/compile_commands.json`
5. Symlinks `compile_commands.json` to the project root
6. Writes a `.clangd` with ARM cross-compile flags so clangd resolves the
   Flipper SDK headers (auto-detects `arm64-darwin` / `x86_64-darwin` toolchain)

## Architecture (default template)

A hexagonal / clean layout with a minimal `main.c` and everything under `src/`,
wired with Flipper's **ViewDispatcher + SceneManager** so adding screens later is
a drop-in. The shipped example is a counter (Up/Down change it, value persists,
Back exits) rendered under a "Hello world" title.

```
my_app/
├── application.fam          # sources=["*.c"] — globs recursively, no edits needed
├── main.c                   # entry point only: alloc → run → free
├── Makefile                 # host unit tests (make test)
├── include/                 # mirrors src/, app-root-relative includes
│   ├── app/                 #   app.h, scenes.h, views.h
│   ├── domain/              #   counter.h
│   ├── ports/               #   storage_port.h  (the abstraction seam)
│   └── persistence/         #   counter_store.h
├── src/
│   ├── app/                 # app.c (lifecycle), scenes.c, scene_main.c, main_view.c
│   ├── domain/              # counter.c   — PURE C, no furi.h → unit-testable
│   ├── platform/            # storage_port.c — furi storage adapter (implements port)
│   └── persistence/         # counter_store.c — what to persist, via the port
└── tests/
    └── test_counter.c       # host test of the domain
```

Layer rules:

- **domain/** — pure logic, no `furi.h`. The only layer with unit tests.
- **ports/** — interfaces (function-pointer structs) the inner layers depend on.
- **platform/** — concrete adapters that implement ports using the Flipper SDK.
- **persistence/** — decides *what* to store, talks only to ports.
- **app/** — GUI: ViewDispatcher/SceneManager lifecycle, scenes, custom views.

### Includes

Every file uses app-root-relative includes, e.g. `#include "include/domain/counter.h"`.
This resolves both in the fap build (ufbt puts the app root on the include path)
and in host tests (`make` compiles with `-I.`) — no compiler config needed.

### Adding a scene

1. Add an entry to `AppScene` in `include/app/scenes.h` (before `SceneCount`).
2. Add `scene_x_on_enter/on_event/on_exit` declarations there.
3. Add a row to each of the three handler tables in `src/app/scenes.c`.
4. Create `src/app/scene_x.c`. No `application.fam` change — sources glob picks it up.

### Host tests

```sh
make test     # compiles src/domain + tests with the system compiler, runs them
```

This is the payoff of keeping `domain/` free of `furi.h`: logic is testable on
your machine without a device.

## Requirements

- [`ufbt`](https://github.com/flipperdevices/flipperzero-ufbt) in `PATH`
- macOS (Apple Silicon or Intel) — the `.clangd` step only detects the
  `arm64-darwin` / `x86_64-darwin` ufbt toolchains
- Neovim with `clangd` (e.g. via Mason) for LSP
- For debugging: `nvim-dap` configured for Flipper in your own Neovim config
  (not provided by this repo)

## License

[MIT](LICENSE)

## Notes

- If you add image assets later or the build dir is cleaned, run `ufbt` again to
  regenerate `*_icons.h` so clangd stops reporting it as missing.
