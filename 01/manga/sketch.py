from keras.models import load_model
import cv2
import numpy as np
import os
from helper import *
from PIL import Image, ImageTk

mod = load_model('mod.h5')
def get(path):
    from_mat = cv2.imread(path)
    width = float(from_mat.shape[1])
    height = float(from_mat.shape[0])
    new_width = 0
    new_height = 0
    if (width > height):
        from_mat = cv2.resize(from_mat, (1024, int(1024 / width * height)), interpolation=cv2.INTER_AREA)
        new_width = 1024
        new_height = int(1024 / width * height)
    else:
        from_mat = cv2.resize(from_mat, (int(1024 / height * width), 1024), interpolation=cv2.INTER_AREA)
        new_width = int(1024 / height * width)
        new_height = 1024

    from_mat = from_mat.transpose((2, 0, 1))
    print( from_mat.shape )
    light_map = np.zeros(from_mat.shape, dtype=np.float)
    for channel in range(3):
        light_map[channel] = get_light_map_single(from_mat[channel])
    light_map = normalize_pic(light_map)
    light_map = resize_img_1024_3d(light_map)
    print(light_map.shape)
    line_mat = mod.predict(light_map, batch_size=1)

    line_mat = line_mat.transpose((3, 1, 2, 0))[0]
    line_mat = line_mat[0:int(new_height), 0:int(new_width), :]
    #show_active_img_and_save('sketchKeras_colored', line_mat, 'sketchKeras_colored.jpg')
    line_mat = np.amax(line_mat, 2)
    #show_active_img_and_save_denoise_filter2('sketchKeras_enhanced', line_mat, 'sketchKeras_enhanced.jpg')
    #img = show_active_img_and_save_denoise_filter('sketchKeras_pured', line_mat, 'sketchKeras_pured.jpg')
    img = show_active_img_and_save_denoise('sketchKeras', line_mat, 'sketchKeras.jpg')

    return img

