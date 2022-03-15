from PIL import Image
import math
#打開圖片

'''
#取得圖片的長寬
im_width, im_height = im.size
#讀取圖片
pixels = im.load()
#取得圖片的每個Pixel

for i in range(im_width):
    for j in range(im_height):
        # Get pixel RGB information
        r, g, b = pixels[i, j]
        print(r, g, b)
# Close image file
im.close()
'''

def RGBtoGRAY(im):
    im_width, im_height = im.size
    pixels = im.load()
    for i in range(im_width):
        for j in range(im_height):
            r, g, b = pixels[i, j]
            gray = math.floor((r*30 + g*59 + b*11 + 50)/100)
            pixels[i,j] = (gray, gray, gray)
    im.show()
    im.save("gray.jpg")
#RGBtoGRAY(im)

def softlightFilter(im):
    im_width, im_height = im.size
    pixels = im.load()
    for i in range(im_width):
        for j in range(im_height):
            r, g, b = pixels[i, j]
            r = 250 if r+30>250 else r+45
            g = 250 if g+30>250 else g+45
            b = 250 if b+30>250 else b+45
            pixels[i,j] = (r, g, b)
    im.show()
    im.save("softlightFilter.jpg")
#softlightFilter(im)

def blur(im):
    im_width, im_height = im.size
    pixels = im.load()
    pixels_new = im.load()
    for i in range(im_width):
        for j in range(im_height):
            if (i > 0 and j > 0 and i < im_width - 1 and j < im_height - 1):
                tmp_pixel = (0, 0, 0)
                for idx in range(-1, 2):
                    for idy in range(-1, 2):
                        tmp_pixel = (
                        tmp_pixel[0] + pixels[i + idx, j + idy][0], tmp_pixel[1] + pixels[i + idx, j + idy][1],
                        tmp_pixel[2] + pixels[i + idx, j + idy][2])
                pixels_new[i, j] = tuple(ele1 // ele2 for ele1, ele2 in zip(tmp_pixel, (9, 9, 9)))
            else:
                pixels_new[i, j] = pixels[i, j]
    im.show()
    im.save("blur.jpg")
#blur(im)

def rmRED(im):
    im_width, im_height = im.size
    pixels = im.load()
    for i in range(im_width):
        for j in range(im_height):

            r, g, b = pixels[i, j]
            r = 0 if (g < 60 and b < 60) else r
            g = 250 if g+30>250 else g+45
            b = 250 if b+30>250 else b+45
            pixels[i,j] = (r, g, b)
    im.show()
    im.save("removeRED.jpg")
#rmRED(im)

def rmGreen(im):
    im_width, im_height = im.size
    pixels = im.load()
    for i in range(im_width):
        for j in range(im_height):

            r, g, b = pixels[i, j]
            g = 0 if (g < 60 and b < 60) else r
            r = 250 if g+30>250 else g+45
            b = 250 if b+30>250 else b+45
            pixels[i,j] = (r, g, b)
    im.show()
    im.save("removeGREEN.jpg")


im = Image.open("1.jpg")
rmGreen(im)
