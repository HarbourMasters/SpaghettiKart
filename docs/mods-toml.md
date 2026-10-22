# mods.toml File Structure

The `mods.toml` file is a metadata file required for all mods and resource packs in SpaghettiKart. It uses the [TOML](https://toml.io/) format and defines important information about your mod, including its name, version, and dependencies.

## Location

The `mods.toml` file must be placed at the **root** of your mod archive (`.o2r`, `.zip`) or folder.

```
MyMod/
├── mods.toml          <- Required at root level
├── textures/
│   └── ...
└── ...
```

## Basic Structure

Here's a minimal `mods.toml` file:

```toml
[mod]
name = "my-awesome-mod"
version = "1.0.0"
```

## Complete Structure

Here's a complete example with all supported fields:

```toml
[mod]
name = "my-awesome-mod"
version = "1.0.0"

[dependencies]
spaghettikart-assets = "=1.0.0-alpha1"
another-mod = ">=2.0.0"
```

## Fields Reference

### [mod] Section

| Field | Type | Required | Description |
|-------|------|----------|-------------|
| `name` | String | Yes | Unique identifier for your mod. Use lowercase letters, numbers, and hyphens. |
| `version` | String | Yes | Version of your mod following [Semantic Versioning](https://semver.org/) (e.g., `1.0.0`, `1.0.0-alpha1`). |

### [dependencies] Section

The `[dependencies]` section is optional and allows you to specify other mods that your mod requires.

Each dependency is defined as:
```toml
[dependencies]
mod-name = "version-requirement"
```

#### Version Requirements

Version requirements follow semantic versioning ranges:

| Format | Description | Example |
|--------|-------------|---------|
| `1.0.0` | Exact version | Only version 1.0.0 |
| `>=1.0.0` | Greater than or equal | Version 1.0.0 or higher |
| `>=1.0.0 <2.0.0` | Range | Between 1.0.0 (inclusive) and 2.0.0 (exclusive) |
| `>=1.0.0-alpha1` | With prerelease | Version 1.0.0-alpha1 or higher |
| `^1.2.3` | Compatible major version | At least 1.2.3 and below 2.0.0 |
| `~1.2.3` | Compatible minor version | At least 1.2.3 and below 1.3.0 |
| `1.2.*` | Wildcard | Any release in the 1.2 series |
| `>=1.0.0 <2.0.0 \|\| >=3.0.0` | Alternative ranges | Either range may match |

Ranges use [Neargye/semver v1.0.1 syntax](https://github.com/Neargye/semver/blob/v1.0.1/doc/reference.md#ranges).
A prerelease only matches a branch that explicitly names a prerelease with the same major, minor and patch.
Build metadata is accepted in a mod's version, but not in dependency range bounds.

## Core Dependencies

SpaghettiKart defines three core packages that are always available:

- `mk64-assets` - The base game resources (version `1.0.0-alpha1`) (`mk64.o2r`)
- `extended-assets` - SpaghettiKart additional assets (version `1.0.0-alpha1`) (`spaghetti.o2r`)
- `spaghettikart-core` - SpaghettiKart core engine (For verifying that mods support your game version)

If your mod depends on touching base game assets or SpaghettiKart assets, you should declare a dependency on these packages. For example:

```toml
[dependencies]
mk64-assets = "=1.0.0-alpha1"
```

## Validation

When SpaghettiKart loads mods, it performs the following validations:

1. **Missing mods.toml**: A warning is shown if the file is missing. The mod may still load but is considered incompatible.
2. **Invalid metadata**: Invalid TOML, version strings and dependency ranges cause the mod to be skipped. The log identifies the archive or folder and the parsing error; invalid version strings and ranges include the field and original value. Trailing junk is rejected.
3. **Cyclic dependencies**: If mod A depends on mod B and mod B depends on mod A, an error is shown.
4. **Missing dependencies**: If a required dependency is not found or has an incompatible version, an error is shown.

## Load Order

Mods are automatically sorted based on their dependencies:
- Mods without dependencies are loaded first
- Mods are loaded after their dependencies

## Best Practices

1. **Follow semantic versioning**: Use proper version numbers (MAJOR.MINOR.PATCH)
2. **Specify minimal dependencies**: Only declare dependencies you actually need
3. **Use version ranges**: Prefer `=1.0.0` because assets may change between versions

## Migration Script Support

When using the migration script (`migrations.py`), a `mods.toml` file can be automatically generated for your migrated mod. See [Migration Guide](migrations.md) for details.

## See Also

- [Modding Guide](modding.md) - General modding information
- [Migration Guide](migrations.md) - How to migrate existing mods
- [Texture Pack Guide](textures-pack.md) - Creating texture packs
