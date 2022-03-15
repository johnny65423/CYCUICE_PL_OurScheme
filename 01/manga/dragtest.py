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
import windnd

print("done.")
if os.path.exists(".imagetemp") :
	shutil.rmtree(".imagetemp")
os.mkdir(".imagetemp")

#testpath = r"E:\manga\test"  #test data image
#imgindex = 0
#testpaths = glob.glob(testpath + "/*.jpg")
#testsize = len(testpaths)

import torch


SIZE = 256

root = tk.Tk()



root.geometry('1044x440')
root.title("ShowImgTest")


def change_img(imagepath):
	print("Painting...")

	inputimage = Image.open(imagepath)
	tkimg1 = ImageTk.PhotoImage(inputimage.resize((256, 394)))
	label1.configure(image=tkimg1)
	label1.image = tkimg1
	label1['borderwidth'] = 2
	label1['relief'] = "solid"

	sketchimage = Image.fromarray(ps.pictosketch(inputimage))
	#sketchimage = Image.fromarray(sk.get(testpaths[imgindex]))
	tkimg2 = ImageTk.PhotoImage(sketchimage.resize((256, 394)))
	label2.configure(image=tkimg2)
	label2.image = tkimg2
	label2['borderwidth'] = 2
	label2['relief'] = "solid"

	grayimage = gc.sketchtograyscale(sketchimage)
	tkimg3 = ImageTk.PhotoImage(grayimage.resize((256, 394)))
	label3.configure(image=tkimg3)
	label3.image = tkimg3
	label3['borderwidth'] = 2
	label3['relief'] = "solid"

	colorimage = ci.graytocolor(grayimage)
	tkimg4 = ImageTk.PhotoImage(colorimage.resize((256, 394)))
	label4.configure(image=tkimg4)
	label4.image = tkimg4
	label4['borderwidth'] = 2
	label4['relief'] = "solid"

	#infostr = str(imgindex + 1) + "/" + str(testsize)
	#indexlabel['text'] = infostr
	button1['state'] = tk.DISABLED

def save_img(dirpath):
	testpaths = glob.glob(dirpath + "/*.jpg")
	testsize = len(testpaths)
	for imgpath in testpaths :
		inputimage = Image.open(imgpath)
		#tkimg1 = ImageTk.PhotoImage(inputimage.resize((256, 394)))
		#label1.configure(image=tkimg1)
		#label1.image = tkimg1
		#label1['borderwidth'] = 2
		#label1['relief'] = "solid"

		sketchimage = Image.fromarray(ps.pictosketch(inputimage))
		# sketchimage = Image.fromarray(sk.get(testpaths[imgindex]))
		#tkimg2 = ImageTk.PhotoImage(sketchimage.resize((256, 394)))
		#label2.configure(image=tkimg2)
		#label2.image = tkimg2
		#label2['borderwidth'] = 2
		#label2['relief'] = "solid"

		grayimage = gc.sketchtograyscale(sketchimage)
		#tkimg3 = ImageTk.PhotoImage(grayimage.resize((256, 394)))
		#label3.configure(image=tkimg3)
		#label3.image = tkimg3
		#label3['borderwidth'] = 2
		#label3['relief'] = "solid"

		colorimage = ci.graytocolor(grayimage)
		#tkimg4 = ImageTk.PhotoImage(colorimage.resize((256, 394)))
		#label4.configure(image=tkimg4)
		#label4.image = tkimg4
		#label4['borderwidth'] = 2
		#label4['relief'] = "solid"
		colorimage.resize((256, 394)).save("result/" + os.path.basename(imgpath))
		print("result" + os.path.basename(imgpath))
	#button2['state'] = tk.DISABLED

def imgnext( mod ) :
	global imgindex
	global imgpath
	if imgindex == -1 :
		imgindex = 0
	elif mod == "+" :
		imgindex += 1
	elif mod == "-" :
		imgindex -= 1

	if imgindex < 0 :
		imgindex = len( imgpath ) - 1

	if imgindex == len( imgpath ) :
		imgindex = 0

	change_img(imgpath[imgindex])

#Create a Label widget
label1= tk.Label(root)
label1.grid(row=1,column=0,padx=4, pady=4, sticky="nw")
label2= tk.Label(root)
label2.grid(row=1,column=0,padx=264, pady=4, sticky="nw")
label3= tk.Label(root)
label3.grid(row=1,column=0,padx=524, pady=4, sticky="nw")
label4= tk.Label(root)
label4.grid(row=1,column=0,padx=784, pady=4, sticky="nw")

#infostr = str(imgindex + 1) + "/" + str(testsize)

#indexlabel = tk.Label(root, font= ('Helvetica 13 bold'), text= infostr )
#indexlabel.grid(row=1, column=0, padx=50, pady=410, sticky="nw")

imgpath = ""
imgindex = 0
button1= tk.Button(root, text= "Change", font= ('Helvetica 13 bold'),command= lambda: change_img(imgpath))
button1.grid(row=1, column=0, padx=220, pady=405, sticky="nw")
button1['state'] = tk.DISABLED
#button2= tk.Button(root, text= "Save All", font= ('Helvetica 13 bold'),command= lambda: save_img(imgpath))
#button2.grid(row=1, column=0, padx=330, pady=405, sticky="nw")
#button2['state'] = tk.DISABLED

button3= tk.Button(root, text= "<", font= ('Helvetica 13 bold'),command= lambda: imgnext("-"))
button3.grid(row=1, column=0, padx=450, pady=405, sticky="nw")
button3['state'] = tk.DISABLED
button4= tk.Button(root, text= ">", font= ('Helvetica 13 bold'),command= lambda: imgnext("+"))
button4.grid(row=1, column=0, padx=500, pady=405, sticky="nw")
button4['state'] = tk.DISABLED
dragspace = tk.Label(root, text= "Drag Image Here.", bg='gray', font=('Arial', 12), width=50, height=2)


def func(ls):

	global imgpath
	global imgindex
	if len(ls) == 1 :
		print(ls[0].decode("utf-8"))
		path = ls[0].decode("utf-8")
		imgpath = path
		dragspace['text'] = path
		if os.path.isdir(path) :
			button1['state'] = tk.DISABLED
			#button2['state'] = tk.NORMAL
			button3['state'] = tk.NORMAL
			button4['state'] = tk.NORMAL
			imgindex = -1
			imgpath = glob.glob(path + "/*.jpg")
		else :
			button1['state'] = tk.NORMAL
			button3['state'] = tk.DISABLED
			button4['state'] = tk.DISABLED

windnd.hook_dropfiles(dragspace.winfo_id(),func)
dragspace.grid(row=1,column=0,padx=590, pady=400, sticky="nw")

print('good')
root.bind("<Return>", change_img)

root.mainloop()

shutil.rmtree(".imagetemp")