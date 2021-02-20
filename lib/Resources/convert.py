import sys
from PIL import Image, ImageOps
import os.path
import fs

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

  this_dir = os.path.dirname(__file__)
  src_dir = os.path.join(this_dir, "..", "..", "images")

  dstf = open('image_resources.h', 'w+')
  dstf.write("#pragma once\n")


  for f in fs.list(path=src_dir):
    final = Image.open(f)
    ident = "_".join(os.path.basename(f).split(".")[:-1]).upper()
    dstf.write("\nconst unsigned char %s[] PROGMEM = {\n" % (ident))
    buffer = getBitmap(final)
    cnt = 0
    for byte in buffer:
      cnt += 1
      if cnt == 1:
        dstf.write("    ")
        dstf.write('0x%02X' % byte)
      elif cnt == 12:
        cnt = 0
        dstf.write(', 0x%02X,' % byte)
        dstf.write('\n')
      else:
        dstf.write(', 0x%02X' % byte)
    dstf.write('};\n')



if __name__ == "__main__":
    main()
