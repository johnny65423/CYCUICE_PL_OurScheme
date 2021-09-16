import tkinter as tk
import os, shutil
import glob
import numpy as np
from PIL import Image, ImageTk
#import sketch as sk

print("load model...")
import Pic2Sketch as ps
#import sketch as sk
import grayscale2 as gc
import colorimage2 as ci
import matplotlib.pyplot as plt
import matplotlib
from skimage.color import rgb2lab, lab2rgb

print("done.")
if os.path.exists(".imagetemp") :
	shutil.rmtree(".imagetemp")
os.mkdir(".imagetemp")

testpath = r"E:\manga\test"  #test data image
imgindex = 0
testpaths = glob.glob(testpath + "/*.jpg")
testsize = len(testpaths)


SIZE = 256

root = tk.Tk()



root.geometry('1040x440')
root.title("ShowImgTest")

def change_img():
	print("Painting...")
	global imgindex
	imgindex += 1
	if imgindex == testsize :
			imgindex = 0
	inputimage = Image.open(testpaths[imgindex])
	tkimg1 = ImageTk.PhotoImage(inputimage.resize((256, 394)))
	label1.configure(image=tkimg1)
	label1.image = tkimg1

	sketchimage = Image.fromarray(ps.pictosketch(inputimage))
	#sketchimage = Image.fromarray(sk.get(testpaths[imgindex]))
	tkimg2 = ImageTk.PhotoImage(sketchimage.resize((256, 394)))
	label2.configure(image=tkimg2)
	label2.image = tkimg2

	grayimage = gc.sketchtograyscale(sketchimage)
	tkimg3 = ImageTk.PhotoImage(grayimage.resize((256, 394)))
	label3.configure(image=tkimg3)
	label3.image = tkimg3

	colorimage = ci.graytocolor(grayimage)
	tkimg4 = ImageTk.PhotoImage(colorimage.resize((256, 394)))
	label4.configure(image=tkimg4)
	label4.image = tkimg4
	infostr = str(imgindex + 1) + "/" + str(testsize)
	indexlabel['text'] = infostr

def save_img():
	button2['state'] = 'disable'

print("Painting...")
inputimage = Image.open(testpaths[imgindex])
tkimg1 = ImageTk.PhotoImage( inputimage.resize((256, 394) ))
sketchimage = Image.fromarray(ps.pictosketch(inputimage))
#sketchimage = Image.fromarray(sk.get(testpaths[imgindex]))
tkimg2= ImageTk.PhotoImage( sketchimage.resize((256, 394) ))
grayimage = gc.sketchtograyscale(sketchimage)
#grayimage =sketchimage
tkimg3= ImageTk.PhotoImage( grayimage.resize((256, 394) ))
colorimage = ci.graytocolor(grayimage)
tkimg4= ImageTk.PhotoImage( colorimage.resize((256, 394) ))


#Create a Label widget
label1= tk.Label(root, image=tkimg1)
label1.grid(row=1,column=0,padx=3, pady=3, sticky="nw")
label2= tk.Label(root, image=tkimg2)
label2.grid(row=1,column=0,padx=260, pady=3, sticky="nw")
label3= tk.Label(root, image=tkimg3)
label3.grid(row=1,column=0,padx=517, pady=3, sticky="nw")
label4= tk.Label(root, image=tkimg4)
label4.grid(row=1,column=0,padx=777, pady=3, sticky="nw")
infostr = str(imgindex + 1) + "/" + str(testsize)

indexlabel = tk.Label(root, font= ('Helvetica 13 bold'), text= infostr )
indexlabel.grid(row=1, column=0, padx=50, pady=410, sticky="nw")

button1= tk.Button(root, text= "Change", font= ('Helvetica 13 bold'),command= change_img)
button1.grid(row=1, column=0, padx=220, pady=405, sticky="nw")

button2= tk.Button(root, text= "Save All", font= ('Helvetica 13 bold'),command= save_img)
button2.grid(row=1, column=0, padx=360, pady=405, sticky="nw")

root.bind("<Return>", change_img)

root.mainloop()

shutil.rmtree(".imagetemp")