# nyymanni ErgoDox EZ layout

External QMK userspace: maintain this small repository, not a firmware fork.
Includes the four-layer C layout, tapping settings, indicator hooks, and a
release-pinned GitHub Actions build. Migrated to QMK **0.34.8** on 2026-11-12.

Repository: https://github.com/nyyManni/nyymanni-qmk-userspace

See [the migration study and layout guide](keyboards/ergodox_ez/keymaps/nyymanni/readme.md)
for upstream fixes/features, existing layout caveats, verification, and recovery
points in the original firmware repository.

## Build locally

Requires the AVR toolchain, Python, and a working QMK CLI >=1.1.7.
Local builds were verified with QMK CLI 1.2.0 and AVR GCC 15.2.0.

Use an independent, unmodified upstream checkout alongside this repository:

```sh
# Run in the directory containing this layout repository:
git clone --branch 0.34.8 https://github.com/qmk/qmk_firmware.git qmk-upstream
git -C qmk-upstream checkout --detach ec8a1b6549a059cac1d284ec8b6377f63af75d05
git -C qmk-upstream submodule update --init lib/lufa
cd nyymanni-qmk-userspace

export QMK_HOME="$(realpath ../qmk-upstream)"
export QMK_USERSPACE="$PWD"
qmk --config-file /dev/null userspace-doctor
qmk --config-file /dev/null userspace-compile
```

Output: `ergodox_ez_base_nyymanni.hex` in this directory. The external build and
54 native typing scenarios passed; the user reported successful typing and
Emacs window-movement shortcuts after the Shift corrections.

The migration workstation already has a release worktree at
`../qmk_firmware-upstream-0.34.8`; you can keep using it by setting `QMK_HOME`
to that path instead. It shares Git storage with the old firmware fork, so
do not delete that fork directory while relying on the worktree. The independent
clone above avoids that dependency.

The firmware revision is pinned by the checkout, not by `qmk.json`.

## Flash deliberately

Use the same environment as above, then:

```sh
qmk --config-file /dev/null flash -kb ergodox_ez/base -km nyymanni
```

Enter the keyboard's HalfKay bootloader using its physical reset when requested.
Nothing has been flashed automatically. Test typing, mod-taps, media, LEDs, and
layers afterward. **Gaming exits via the right inner-top toggle; the left one is
`6` while gaming.** This is the AVR/base target, not Shine/Glow/EZ ST.

## Maintain / publish

Edit `keyboards/ergodox_ez/keymaps/nyymanni/` here. The copy in the old firmware
fork is a migration snapshot; do not maintain both copies in parallel.

The public layout-only repository is
https://github.com/nyyManni/nyymanni-qmk-userspace. Commit layout changes here and
push `main`; `.gitignore` excludes build outputs.

GitHub Actions builds every push and uploads a `Firmware` artifact containing
the HEX file. Download it from the successful run's Actions page. The workflow
can also be started manually using **Run workflow**.

The workflow pins the firmware SHA for 0.34.8 and the official reusable-workflow
revision. Its upstream container still floats, so it is not a fully pinned
compiler environment. For future updates, change the clean firmware checkout and
workflow `qmk_ref` together, read the intervening breaking-change notes, and
rebuild. No firmware rebases are needed.
