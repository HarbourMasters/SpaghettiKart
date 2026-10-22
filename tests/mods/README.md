# Mod metadata regression tests

These standalone tests use the production `ModMetadata.h`, the game's shared
semver v1.0.1 pin and toml++ v3.4.0. No game assets or test framework are required.
The first configure downloads the two header-only dependencies.

From the repository root:

```sh
cmake -S tests/mods -B build/mod-metadata-tests
cmake --build build/mod-metadata-tests --config Release
ctest --test-dir build/mod-metadata-tests -C Release --output-on-failure
```

On Windows, the Visual Studio 2022 generator can be selected with
`-G "Visual Studio 17 2022" -T v143 -A x64`, as in `docs/BUILDING.md`.

Coverage includes invalid and partially parsed version strings, invalid ranges,
malformed TOML, diagnostics, valid versions, the core asset prerelease ranges,
AND/OR membership, trailing range whitespace, and stable semver's additional
range syntax. Checks remain enabled in Release builds.

These tests exercise metadata parsing, not `FindAndLoadMods`, archive I/O, game
startup or the full game build.
