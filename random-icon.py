# Generate a random file.ico

from struct import pack
from random import randint

def random_icon(width, height):
    buff = []

    # Icon information
    buff += pack('<H',  0) # reserved
    buff += pack('<H',  1) # image type (icon=1 / cursor=2)
    buff += pack('<H',  1) # number of images

    buff += pack('<B', width) # width
    buff += pack('<B', height) # height
    buff += pack('<B',  0) # color palette
    buff += pack('<B',  0) # reserved
    buff += pack('<H',  1) # color planes
    buff += pack('<H', 32) # bits per pixel
    buff += pack('<L', width*height*2*4) # image size in bytes
    buff += pack('<L', len(buff)+4) # offset of bmp

    # Bitmap file header
    buff += pack('<L',  40) # BITMAPINFOHEADER
    buff += pack('<i',  width) # width in pixels
    buff += pack('<i',  height*2) # height in pixels
    buff += pack('<H',   1) # color planes
    buff += pack('<H',  32) # bits per pixel
    buff += pack('<L',   0) # compression = BI_RGB
    buff += pack('<L',   0) # image size (0 is acceptable for BI_RGB)
    buff += pack('<i',   1) # horizontal resolution (pixel per meter)
    buff += pack('<i',   1) # vertical resolution (pixel per meter)
    buff += pack('<L',   0) # number of colors in the color palette (default = 0)
    buff += pack('<L',   0) # number of important colors

    # Image
    for i in range(width*height*2):
        buff += pack('<L',  randint(0, 1<<32)) # pixel

    return buff

img_data = bytearray(random_icon(64,64))
f = open('icon.ico', 'wb')
f.write(img_data)
f.close()