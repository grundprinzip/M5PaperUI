import sys
from PIL import Image, ImageOps
import os.path

GRAYSCALE_MAP = [0 for i in range(256)]

x = -1
for i in range(0, 256):
    if i % 16 == 0:
        x += 1
    GRAYSCALE_MAP[i] = 15 - x


def getBitmap(img):
    buffer = []
    byte = 0
    img = img.convert('L')
    for y in range(img.height):
        for x in range(0, img.width, 2):
            byte = GRAYSCALE_MAP[img.getpixel((x, y))] << 4
            if x + 1 < img.width:
                byte |= GRAYSCALE_MAP[img.getpixel((x + 1, y))]
            buffer.append(byte)
    return bytes(buffer)


def main():
    num_args = len(sys.argv) - 1
    dstf = open('image_resources.h', 'w+')
    dstf.write("#pragma once\n\n")

    for x in range(num_args):
        file_name = sys.argv[x + 1]
        final = Image.open(file_name)

        ident = "_".join(os.path.basename(file_name).split(".")[:-1]).upper()
        dstf.write("const unsigned char %s [] PROGMEM = {\n" % (ident))
        buffer = getBitmap(final)
        cnt = 0
        for byte in buffer:
            dstf.write('0x%02X, ' % byte)
            cnt += 1
            if cnt == 15:
                cnt = 0
                dstf.write('\n\t')

        dstf.write('};\n\n')


if __name__ == "__main__":
    main()
