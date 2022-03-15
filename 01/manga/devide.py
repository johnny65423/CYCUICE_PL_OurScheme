from PIL import Image
import glob

path = r"E:\manga\origin\10"  #test data image
paths = glob.glob(path + "/*.jpg")


i = 0



for page in paths:
    im = Image.open(page)
    left = 0
    top = 0
    right = im.width/2
    bottom = im.height
    im1 = im.crop((left, top, right, bottom))
    im1.save("origin/B10/" + str(i) + ".jpg" )
    i += 1
    im2 = im.crop((right, top, im.width, bottom))
    im2.save("origin/B10/" + str(i) + ".jpg" )
    i += 1




