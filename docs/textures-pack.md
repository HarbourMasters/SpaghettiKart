# Texture Pack
To replace textures you should only create a zip file or a folder with textures to replace at the same path as mk64.o2r. Only png, jpg and bmp files are supported.
## Example:
You want to change textures/other_textures/logo_mario_kart_64 then you just need to create a zip file or folder with the following structure:
```
textures_pack.zip
└── textures
    └── other_textures
        └── logo_mario_kart_64.png
```

## Tools To Help
To extract textures from the game you can use [convert-texture-o2r](https://github.com/coco875/convert-texture-o2r/releases). Just place the `convert-texture-o2r` executable in the same folder as `mk64.o2r` and drag and drop the `mk64.o2r` file on it or run it through command line:
```bash
convert-texture-o2r mk64.o2r
```
This will create a `textures` folder with all the textures extracted from the game.

## Future plans
* Make name more meaningful and uniform. (example: change `texture_69B140`). You can suggest name or make a PR to help rename them/organise them.
* Support dds files.
* move texture in a subfolder.