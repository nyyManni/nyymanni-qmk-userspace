# nyymanni — ErgoDox EZ

## Upgrade record — 2026-11-12

Rebased the two personal layout commits onto **QMK 0.34.8**, commit
`ec8a1b6549a059cac1d284ec8b6377f63af75d05`, the newest upstream numeric release
tag returned on the study date. This is a stable tag, not the tip of `develop`.

The previous upstream baseline was `36c3f4deba7e4c670736cc95db144b25156e90f1`
(July 23, 2025; nearest ancestor tag `0.29.11`). Contrary to the initial estimate,
the checked-out fork was about 16 months old, not several years old. Only
`cf4ec88816` and `19764f726a` were personal commits; all changes were under this
keymap directory. **No custom firmware patches needed carrying forward.**
The uncommitted FINNISH-to-GAMING overlay changes were preserved too.

Recovery points in the original firmware repository:

- Branch `backup/nyymanni-before-0.34.8` preserves the original committed tip,
  `19764f726a79ec509223e11e07db22deea124d06`.
- Stash commit `e03bae4434adffdca1c53941263fbced19e94ed3` preserves the original
  uncommitted gaming changes. It was applied and retained, not dropped.
- The two layout commits were replayed onto the release. Compatibility cleanup,
  this report, and the original gaming edits remain uncommitted. Nothing was
  pushed; no remote URL or global QMK configuration was changed.

### Applied compatibility cleanup

- Kept `TAPPING_TERM 100`, `PERMISSIVE_HOLD`, and `RETRO_TAPPING`.
- Replaced ambiguous mod-tap aliases with equivalent `LALT_T`, `LSFT_T`, and
  `LGUI_T`; these remain **left** modifiers even on the right half.
- Removed ineffective `TAPPING_FORCE_HOLD` and `RETRO_TAPPING_LIMIT 650`.
  Neither controlled the old baseline's tap-hold implementation. In particular,
  retro tapping did **not** previously stop after 650 ms.
- Did **not** add `QUICK_TAP_TERM 0`: that would disable the tap-then-hold repeat
  behavior actually present at the old baseline. The default quick-tap term
  remains the tapping term, 100 ms. The historical migration dates to
  [February 2023](https://github.com/qmk/qmk_firmware/blob/0.34.8/docs/ChangeLog/20230226.md#quick-tap-term),
  not this release.
- Removed redundant `TAPPING_TOGGLE 1` (already the keyboard default; this map
  uses `MO` and `TG`, not `TT`) and inactive Unicode configuration. Unicode was
  not enabled and its keycodes were commented out.
- Removed inaccurate factory-layout diagrams and image links. They described
  keys, mouse controls, and modifiers that this layout does not have.

### Rebase verification (before Shift tuning below)

Local toolchain: QMK CLI **1.2.0**, AVR GCC **15.2.0**. The installed `qmk`
launcher referenced a missing interpreter under an old home directory. Builds
used an isolated `uv run --no-project --with qmk --with-requirements requirements.txt`
environment instead; the global installation was not repaired or replaced.

| Check | Result |
|---|---|
| Original dirty-change-inclusive `ergodox_ez/base:nyymanni` build | Pass; 19,378 / 32,256 flash bytes |
| Rebased and cleaned-up build | Pass; 18,886 / 32,256 flash bytes; 13,370 free |
| Compiled `keymaps` symbol, before versus after | All **672 bytes identical**, covering four 14×6 matrices, including unused matrix positions |
| External-userspace validation | Valid schema 1.1 manifest; userspace enabled |
| External-userspace manifest build against unmodified upstream 0.34.8 | Pass |

The exact compiled keymap-table comparison proves unchanged keycode assignments,
not identical QMK core behavior across releases. Upstream fixes are intentionally
included. Clangd lacked QMK's generated include/define context and reported missing
headers/types; the actual AVR builds are the compilation evidence.

**Not verified:** physical typing feel, USB host behavior, LEDs on hardware,
GitHub Actions execution, or flashing. No keyboard was flashed and no EEPROM was
reset. After deliberately flashing, check: Z/slash tap versus Shift hold; Escape
and quote versus Alt hold; minus versus GUI hold; tap-then-hold repeat; a long
uninterrupted retro tap; nested chords; independently held Shift during retro
release; all three layer indicators; media controls; and gaming entry/exit.

## Initial Shift usability tuning — 2026-11-12

Historical profile, superseded by the capital-letter correction below. Key
bindings were unchanged.

- Z/Shift and slash/Shift now use **200 ms** tapping terms. Alt/Escape,
  Alt/quote, and GUI/minus retain **100 ms**.
- `CHORDAL_HOLD` is enabled, but `get_chordal_hold()` applies the opposite-hand
  rule only to those two Shift mod-taps. Other mod-taps remain unrestricted.
- Same-hand key presses before 200 ms settle Z/slash as taps. Opposite-hand
  chords remain eligible for Shift via the existing `PERMISSIVE_HOLD`.
  Hold left Z/Shift and tap right slash for `?`; hold right slash/Shift and tap
  left Z for `Z`.
- After 200 ms, Shift can still be held even for a same-hand chord.
- Handedness explicitly follows the EZ matrix: rows 0–6 left, 7–13 right.
- `RETRO_TAPPING` remains enabled. The quick-tap repeat window stays **100 ms**;
  neither Flow Tap nor Speculative Hold is enabled.

Both the in-firmware and external-userspace layouts were updated and built.
Flash usage: **19,798 / 32,256 bytes** in the fork and **19,782 / 32,256 bytes**
in the clean upstream userspace build (different embedded version strings).
A temporary native harness compiled the actual keymap callbacks and QMK tapping
engine: **10 scenarios passed**, checking same-hand nested Z/slash presses at
150 ms, opposite-hand `?`/`Z`, same-hand Alt/Escape, Alt/quote and GUI/minus
chords, same-hand Shift after 200 ms, long retro taps, and tap-then-hold repeat.
The harness was removed after verification. Indicator hardware was not simulated;
physical typing feel still needs evaluation after deliberate flashing.

## Capital-letter correction — 2026-11-12

Hardware use exposed a missed release-order case in the initial tuning:
`PERMISSIVE_HOLD` alone requires the letter to be released before Shift. A quick
rolled capital chord (Shift down, letter down, Shift up, letter up) could instead
produce `z` or `/` followed by an unshifted letter. The initial 10-scenario
check covered nested capital chords but omitted this release order.

The user chose to retain opposite-hand protection and disable Shift-key
tap-then-hold repeat. The current profile is:

- **200 ms** tapping terms for Z/Shift and slash/Shift; **100 ms** for Alt/GUI.
- `HOLD_ON_OTHER_KEY_PRESS_PER_KEY`, enabled only for the two Shift mod-taps:
  an eligible opposite-hand keydown immediately selects Shift, independent
  of which key is released first.
- `CHORDAL_HOLD` still rejects same-hand ordinary-key Shift chords before 200 ms.
  For quick capitals use right slash/Shift for left-hand D/S/T, and left
  Z/Shift for right-hand H/I/N. Left Z/Shift + left D, or right slash/Shift
  + right H, still produces taps before the term by design.
- `QUICK_TAP_TERM_PER_KEY` returns **0** for those two Shift keys. A recent
  Z/slash tap no longer forces the next attempted Shift hold into auto-repeat.
  Repeated ordinary taps still work. Other mod-taps retain the 100 ms repeat
  window and their previous Permissive Hold behavior.
- `RETRO_TAPPING` remains enabled; Flow Tap and Speculative Hold remain off.

Both layout copies were updated. Both firmware builds passed; the in-firmware
image uses **20,060 / 32,256 flash bytes**. A temporary real-core native harness
passed **20 typing scenarios**, covering both release orders for opposite-hand
capital chords (including `Z` and `?`), rapid successive capitals, a Z/slash tap
immediately followed by Shift, same-hand typing protection, unchanged Alt/GUI
chords, retro taps, and repeated ordinary taps. The harness was removed afterward.
The corrected firmware has not been flashed automatically; physical evaluation
is still needed. Use the existing flash instructions below to rebuild and flash
`ergodox_ez/base:nyymanni`.

## Ctrl+Shift shortcut correction — 2026-11-12

Emacs's live recent-key log showed stray sequences such as `z C-h` and
`z C-l` among the intended `C-S-h` / `C-S-l` window movements. The live
bindings map `C-S-h/j/k/l` to `windmove-left/down/up/right`. In Evil normal
state, `z` is a prefix, and `prefix-help-command` is `which-key-C-h-dispatch`.
This is consistent with the reported prefix-help popup; the popup itself was
not visible during inspection. No Emacs bindings were changed.

The firmware had an identified order-sensitive path: Z/Shift down, left Ctrl
down, then H/J/K/L. The same-hand filter rejected left Ctrl and emitted Z
before the intended shortcut's letter arrived.

`get_chordal_hold()` now exempts ordinary modifier keys (Ctrl/Shift/Alt/GUI)
from the same-hand filter when a Shift mod-tap is pending. This lets Shift
and Ctrl form a modifier chord regardless of which is pressed first.
Ordinary same-hand letters remain protected, and the 200 ms Shift tapping
term, zero Shift quick-tap term, and Alt/GUI behavior are unchanged.

Both layout copies and firmware builds were updated; the in-firmware image
uses **20,070 / 32,256 flash bytes**. **54 native typing scenarios passed**:
the preceding 20 cases; all H/J/K/L window shortcuts with left/right Ctrl,
both modifier press orders and both Shift/letter release orders; repeated
window movements with Ctrl held; and right slash/Shift followed by right Ctrl.
Host reports contained both modifiers and no stray Z/slash for those shortcuts,
and no modifiers remained stuck after release. The temporary harness was removed.
No firmware was flashed automatically; use the flash command below.

## Actual layout behavior

| Layer | Entry | Contents / indicator |
|---|---|---|
| BASE (0) | Default | QWERTY, symbols, media controls; layer LEDs off |
| SYMB (1) | Either inner bottom `MO(SYMB)` | F1–F12, symbols, real keypad keys, previous/next track; LED 1 |
| MOVEMENT (2) | Either outer bottom `MO(MOVEMENT)` | H/J/K/L arrows, GUI+F10 and GUI+F15 host shortcuts; LED 2 |
| GAMING (3) | Either inner top toggle from BASE | Partial left-hand overlay; LED 3 |

Gaming replaces left Delete with left Alt, Alt/Escape with plain Escape, the
left GUI position with left Shift, Shift/Z with plain Z, and left-thumb End with
Enter. The left inner-top gaming toggle becomes `6`; **exit with the right
inner-top toggle**. Right-hand Alt/quote, Shift/slash, and GUI/minus remain
mod-taps. This is not a fully mod-tap-free gaming layout.

Higher layers win. Gaming's explicit bindings override symbols/movement;
transparent positions still fall through. LEDs show the highest active layer,
so holding a lower momentary layer while gaming keeps LED 3 lit.

Symbols assume a **US host layout**. Former FINNISH naming did not switch the
OS layout or implement Nordic Unicode input. Right-side Alt taps still hold
left Alt, not AltGr. Keypad numbers depend on Num Lock and host/application
handling; there is no Num Lock key here. GUI+F10/F15 only start a screensaver if
the host binds them that way. `VRSN`, at the symbol layer's top-left key, types
the keyboard/keymap and compiled QMK version.

## Study: worthwhile upstream changes

Reviewed the five breaking-change cycles after the baseline:
[August 2025](https://github.com/qmk/qmk_firmware/blob/0.34.8/docs/ChangeLog/20250831.md),
[November 2025](https://github.com/qmk/qmk_firmware/blob/0.34.8/docs/ChangeLog/20251130.md),
[February 2026](https://github.com/qmk/qmk_firmware/blob/0.34.8/docs/ChangeLog/20260222.md),
[May 2026](https://github.com/qmk/qmk_firmware/blob/0.34.8/docs/ChangeLog/20260531.md),
and [August 2026](https://github.com/qmk/qmk_firmware/blob/0.34.8/docs/ChangeLog/20260830.md),
plus subsequent changes through the target tag and the ErgoDox EZ path history.

### Included automatically; no new layout setting needed

1. **Retro-tapping modifier preservation** (May 2026,
   [#26127](https://github.com/qmk/qmk_firmware/pull/26127),
   [implementation](https://github.com/qmk/qmk_firmware/commit/d19204824d6229b05a7d41b3e3a82966ff8a79fe)).
   Releasing a retro-tapped key no longer unregisters modifiers independently
   held by another key. Directly relevant to this map's Shift mod-taps and
   dedicated gaming Shift.
2. **Safer extra-key HID descriptor** (August 2026,
   [#26295](https://github.com/qmk/qmk_firmware/blob/0.34.8/docs/ChangeLog/20260830.md#extrakey-gamepad)).
   Narrows System Control usages to avoid some hosts treating the keyboard as a
   gamepad. This board enables extra/media keys. Keep the new default; do not
   add the documented broad-range compatibility override without a real need.
3. **Build-system maintenance.** The old `DEFAULT_FOLDER` setting was replaced
   upstream by keyboard aliases. Continue building the explicit
   `ergodox_ez/base` variant. No board-level fork patch is necessary.

No post-baseline AVR ErgoDox EZ matrix-scanning fix was identified in its path
history. Glow's RGB Matrix layout moved into JSON
([#25561](https://github.com/qmk/qmk_firmware/commit/6aa85699a5626c084a1cfd4ea2ecca3c14783c33));
that is not a change to this base model's three indicator LEDs. FORCE_NKRO removal
in another EZ keymap is not a change to this keymap. ChibiOS/STM32 fixes, including
the target tag's headline change, are not AVR EZ improvements. The verified
hardware target remains ATmega32U4 / HalfKay; Shine, Glow, and EZ ST are not
interchangeable build targets.

### Feature assessment and further experiments

| Feature | Availability | Relevance and tradeoff |
|---|---|---|
| **Speculative Hold**: `#define SPECULATIVE_HOLD` | New since baseline, November 2025; constraints added May 2026 | Not enabled. Sends eligible modifiers immediately for Shift+mouse-click without waiting for tap-hold settlement. Consider only if modifier latency is noticeable; it does not resolve tap-versus-hold ambiguity and can expose standalone modifier side effects. |
| **Flow Tap**: e.g. `#define FLOW_TAP_TERM 150` | Already available May 2025 | Favors immediate taps during fast typing; may reduce accidental Shift on Z/slash. Default eligibility does not cover Escape, quote, or minus. Can suppress intentional fast-following holds. |
| **Chordal Hold**: `#define CHORDAL_HOLD` | Already available February 2025 | Now enabled for the two Shift mod-taps only; same-hand chords before their tapping term favor taps. See the Shift tuning section above. |
| **Per-key tapping term**: `TAPPING_TERM_PER_KEY`, `get_tapping_term(...)` | Already available | Now enabled: Z/Shift and slash/Shift use 200 ms; other mod-taps remain at 100 ms. Physical-use evaluation remains necessary. |
| **Repeat / alternate repeat**: `REPEAT_KEY_ENABLE = yes`, `QK_REP`, `QK_AREP` | Already available May 2023 | Useful for repeated actions; alternate repeat reverses supported directions. Needs deliberate physical key assignments. |
| **Caps Word**: `CAPS_WORD_ENABLE = yes`, `CW_TOGG` | Already available May 2022 | Useful for identifiers. Prefer an explicit activation key: both existing Shift mod-taps are left Shift, not a left/right pair for both-Shifts activation. |
| **One-shot modifiers/layers**: `OSM(MOD_LSFT)`, `OSL(SYMB)` | Already available | Reduces held chords but changes interaction and can surprise in games. Keep present momentary layers for now. |
| **Nordic Unicode**: `UNICODE_ENABLE = yes`, `UNICODE_SELECTED_MODES UNICODE_MODE_LINUX`, `UC(...)` | Already available | Could restore å/ä/ö independently of US punctuation, but requires host support and new bindings. It is not automatic OS-layout switching or portable game input. |

Sources: [tap-hold options](https://github.com/qmk/qmk_firmware/blob/0.34.8/docs/tap_hold.md),
[Repeat](https://github.com/qmk/qmk_firmware/blob/0.34.8/docs/features/repeat_key.md),
[Caps Word](https://github.com/qmk/qmk_firmware/blob/0.34.8/docs/features/caps_word.md),
[one-shots](https://github.com/qmk/qmk_firmware/blob/0.34.8/docs/one_shot_keys.md),
[Unicode and host limitations](https://github.com/qmk/qmk_firmware/blob/0.34.8/docs/features/unicode.md).

Recommendation: evaluate the Shift-only Chordal Hold and per-key tuning above
before adding more features. Flow Tap remains off because it can suppress an
intentional Shift hold immediately after typing.

## Maintain only the layout: external C userspace

**Recommended:** maintain a small independent repository with the existing C
keymap/config and a `qmk.json` build manifest. Build it against an unmodified,
release-pinned upstream checkout. No firmware fork and no recurring Git rebases.
This workflow already existed before the old baseline; it is not new in 0.34.8.

This standalone layout is maintained at
https://github.com/nyyManni/nyymanni-qmk-userspace, with the actual migrated
sources, this study, a root README, schema 1.1 manifest, build-artifact
exclusions, and a release-pinned GitHub Actions workflow. Edit this repository
as the source of truth; the in-firmware copy is the rebased migration snapshot,
not a second layout to keep editing in parallel.

```text
nyymanni-qmk-userspace/
  qmk.json
  README.md
  .gitignore
  .github/workflows/build.yml
  keyboards/ergodox_ez/keymaps/nyymanni/
    keymap.c
    config.h
    readme.md
```

Manifest:

```json
{
    "userspace_version": "1.1",
    "build_targets": [["ergodox_ez/base", "nyymanni"]]
}
```

The manifest does **not** pin the firmware revision. The checkout and CI workflow
pin it separately to `ec8a1b6549a059cac1d284ec8b6377f63af75d05` (0.34.8).

### Local build and flash

From the standalone repository root, with a working QMK CLI and AVR toolchain:

```sh
export QMK_HOME="$(realpath ../qmk_firmware-upstream-0.34.8)"
export QMK_USERSPACE="$PWD"
qmk --config-file /dev/null userspace-doctor
qmk --config-file /dev/null userspace-compile
# Only when deliberately flashing the physical keyboard:
qmk --config-file /dev/null flash -kb ergodox_ez/base -km nyymanni
```

Run from the layout directory, not inside the old firmware fork. Explicit paths
and an empty per-invocation config avoid old global CLI configuration selecting
the wrong firmware/userspace. Use CLI >=1.1.7; this migration used 1.2.0.
`userspace-compile` places the HEX in the userspace root. Flashing uses HalfKay;
activate the board's reset/bootloader when requested. No software reset key was
added to the layout.

The same local commands can use the isolated environment that was verified:

```sh
export QMK_HOME="$(realpath ../qmk_firmware-upstream-0.34.8)"
export QMK_USERSPACE="$PWD"
uv run --no-project --with qmk --with-requirements "$QMK_HOME/requirements.txt" \
  qmk --config-file /dev/null userspace-compile
```

A clean upstream tag worktree, `../qmk_firmware-upstream-0.34.8/`, was created for
the independent smoke build. It has no personal keymap in its source tree and
shares Git storage with the old firmware repository. **Do not delete the original
repository while relying on that worktree.** For an independent long-term setup,
replace it with an ordinary upstream clone, for example at another sibling path:

```sh
git clone --branch 0.34.8 https://github.com/qmk/qmk_firmware.git qmk-upstream
git -C qmk-upstream checkout --detach ec8a1b6549a059cac1d284ec8b6377f63af75d05
git -C qmk-upstream submodule update --init lib/lufa
# Set QMK_HOME to this clone when building from the layout repository.
```

Only LUFA was required for the verified AVR/base build. Other boards and test
suites may need other submodules. Upgrading later means choosing a new stable
firmware tag/SHA, reading intervening breaking-change notes, updating the clean
checkout and CI `qmk_ref`, then building and evaluating the actual keyboard.
There is no personal firmware history to replay.

### Hosted builds and alternatives

The standalone `.github/workflows/build.yml` calls the official reusable userspace
workflow pinned to `01daf5113fa50804558f21cc074ab99ba84ddeaf`, with the firmware
SHA pinned separately. Every push to the public layout-only repository triggers
a build; download the `Firmware` artifact from a successful Actions run.
The reusable workflow uses a floating `ghcr.io/qmk/qmk_cli:latest` container and
version-tagged nested actions: these pins stabilize source selection, **not the
entire toolchain**. No automatic publishing/release job was added.

Configurator JSON alone is not an equivalent migration: it cannot carry this
layout's arbitrary version-string and LED callbacks plus C configuration. JSON
with adjacent C would still need source files and adds conversion work. Submitting
a new personal keymap upstream is not an option under
[QMK's current policy](https://github.com/qmk/qmk_firmware/blob/0.34.8/docs/pr_checklist.md#keymap-prs).
External C userspace is the simplest behavior-preserving option.

Sources: [external userspace guide](https://github.com/qmk/qmk_firmware/blob/0.34.8/docs/newbs_external_userspace.md),
[schema](https://github.com/qmk/qmk_firmware/blob/0.34.8/data/schemas/user_repo_v1_1.jsonschema),
[official template](https://github.com/qmk/qmk_userspace),
[inspected reusable workflow](https://github.com/qmk/.github/blob/01daf5113fa50804558f21cc074ab99ba84ddeaf/.github/workflows/qmk_userspace_build.yml).
