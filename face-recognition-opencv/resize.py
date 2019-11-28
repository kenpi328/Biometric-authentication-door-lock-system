import os, sys
from PIL import Image
import glob
filenames = glob.glob("dataset/jj/*.jpg")

size = 800, 800

for file in filenames:
    #outfile = os.path.splitext(infile)[0] + ".thumbnail"
    im = Image.open(file)
    im.thumbnail(size, Image.ANTIALIAS)
    im.save(file, "JPEG")
