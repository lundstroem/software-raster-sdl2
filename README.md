# software-raster-sdl2
This example will present a virtual screen in form of a software raster at
the resolution of 320x180 (16:9). In windowed mode it will use that resolution
scaled up to 2 (640x360), and in fullscreen it will present black bars on top and
bottom to prevent stretching if a ratio not as wide as 16:9 is provided.

Ratios wider than 16:9 will not be handled at the moment and thus stretched.
