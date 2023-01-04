# vidio video format

> its video, but simple

# why

its basically like ppm but video, the format is literally
raw bytes, nothing else:

    (2 bytes)   unsigned char magic[2]         (should usually be 'vi', but its ignored)
    (4 bytes)   unsigned fps                   (frames per second)
    (4 bytes)   unsigned width                 (width of the canvas)
    (4 bytes)   unsigned height                (height of the canvas)
    (whn bytes) unsigned char frames[n][w][h]  (all of your framas)
     ^^^ w = width; h = height; n = frames_count
         all data in this array is a continuous  arrat of rgb values:
         e.g. {
                 255, 255, 255,
                 0,   0,   0,
                 255, 0,   0,
              }

and its very easily convertable to other simple media formats, for example
PPM P6 format, renderer/converter comes already implemented and the default
lib is fully compatible with C89, also, this format is very extensible,
at first i thought i might add support for `alpha` values but then thought
theyre quite useless in this case, if you will just add `alpha` field to
the `VidioPixel` struct:

```c
typedef struct {
    unsigned char red, green, blue, alpha;
} VidioPixel;
```

thats all you need to add support for alpha, just make sure to also have
`alpha` in your data array when generating:

```c
{
    255, 255, 255, a,
    0,   0,   0,   a,
    255, 0,   0,   a,
}
```

or, well, in the pyhton test script:

```py
bytearray((255, 255, 255, a) * 100 * 100 * 60)  # 60 100x100 frames
```

but if youll use `vidio_render_frame_to_ppm`, you should adapt it
to use your new format

# why not ?

its not a widely supported, established format of video, i only
added `fps` option for future when ill actually be implementing
viewing video on some viewer, maybe `mpv` ? well see :)

# lib

see [include](/include) for the source code of the library :)

# example usage

example usage can be found in the [src](/src) folder, you can generate
your own blank, fully white vidio file using the [test.py](/scripts/test.py)
script, it will generate a `test.vi` file which you can then pass
to `vidio.elf` after compiling with `./build.sh` :

```sh
$ python3 scripts/test.py

$ rm -rf frames && ./build.sh -r test.vi

$ mpv frames/
```

make sure `frames` directory is never present, im too lazy to
add checking for it in the example

the example python script also implements another view on the
format, if you want something to reference very quickly, take
a look at the test script :)
