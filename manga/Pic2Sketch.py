import cv2
import os
import math
import numpy as np

def pictosketch(image):
	img = np.array(image)
	gray_image = img

	#cv2.imshow("1", gray_image)
	inverted = 255 - gray_image

	blurred = cv2.GaussianBlur(inverted, (25, 25), 0)
	blurred = cv2.GaussianBlur(blurred, (25, 25), 0)
	blurred = cv2.GaussianBlur(blurred, (25, 25), 0)
	invertedblur = 255 - blurred

	sketch = cv2.divide(gray_image, invertedblur, scale = 256.0)
	#cv2.imshow("2", sketch)
	#cv2.imwrite("sketch.jpg", sketch)
	return sketch

'''
def modify_contrast_and_brightness2(img, brightness=0, contrast=100):


	brightness = 0
	contrast = +50  # - 減少對比度/+ 增加對比度

	B = brightness / 255.0
	c = contrast / 255.0
	k = math.tan((45 + 44 * c) / 180 * math.pi)

	img = (img - 127.5 * (1 - B)) * k + 127.5 * (1 + B)

	# 所有值必須介於 0~255 之間，超過255 = 255，小於 0 = 0
	img = np.clip(img, 0, 255).astype(np.uint8)
	return img

'''