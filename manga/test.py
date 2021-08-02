import os
from PIL import Image, ImageTk

img = Image.open("test/2.jpg")

img.convert("L").save("test/5.jpg")