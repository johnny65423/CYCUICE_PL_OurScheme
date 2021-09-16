import PIL
from PIL import Image, ImageFilter
if __name__ == '__main__':
    image1 = Image.open(r"D:\PythonProject\SLIC\img0903\temp\1_grayscale.jpg").convert("RGB") # 生成的灰階圖片
    pix1 = image1.load()
    image2 = Image.open(r"D:\PythonProject\SLIC\img0903\temp\1_sketch.jpg").convert("RGB") # 轉換的線稿圖片
    pix2 = image2.load()
    image3 = Image.open(r"D:\PythonProject\SLIC\img0903\temp\1_grayscale_superpixel.jpg").convert("RGB") # 生成的灰階圖片轉的超像素圖片
    image3 = image3.filter(ImageFilter.SMOOTH)
    pix3 = image3.load()

    for x in range(256):
        for y in range(394):
            r1, g1, b1 = pix1[x, y]
            r2, g2, b2 = pix2[x, y]
            r3, g3, b3 = pix3[x, y]
            if r2 > 250 and g2 > 250 and b2 > 250: # 是框線或是文字
                pix1[x, y] = r3, g3, b3
    image1.show()

    #new_img = image1.filter(ImageFilter.MedianFilter(3))  # 使用軍職濾波器
    #new_img = image1.filter(ImageFilter.SMOOTH)

    image1.save("fake.jpg")