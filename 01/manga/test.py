import cv2
import numpy as np

img = cv2.imread("1.jpg")

lsc = cv2.ximgproc.createSuperpixelLSC(img)
lsc.iterate(10)
mask_lsc = lsc.getLabelContourMask()
label_lsc = lsc.getLabels()
number_lsc = lsc.getNumberOfSuperpixels()
mask_inv_lsc =cv2.bitwise_not(mask_lsc)
img_slic = cv2.bitwise_and(img,img,mask = mask_inv_lsc)
cv2.imshow("img", img_slic)
cv2.waitKey(0)
cv2.destroyWindow()