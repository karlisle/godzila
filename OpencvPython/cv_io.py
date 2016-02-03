#!/usr/bin/python

#######################################
# Author: 	Kharl                     #
# Licence: 	Creative commons          #
# WebPage: 	karlislee.com             #
#                                     #
#######################################


from collections import deque
import numpy as np 
import argparse
import imutils
import cv2


# construir el detector de argumento sy parametros
ap = argparse.ArgumentParser()
ap.add_argument("-v", "--video",help = "path to the (optional) video file")
ap.add_argument("-b", "--buffer", type=int, default=64,help="max buffer size")
args = vars(ap.parse_args())

# Definir los valores maximos y minimos del color 
# en el espacio de colo HSV, luego inicalizar la lsita de punto
# a rastrear

greenLower = (29,86,6)
greenUpper = (64,255,255)
pts = deque(maxlen=args["buffer"]) 

# Si la ruta del video no fue proporcionada, redirige a la camara web
if not args.get("video", False):
	camara = cv2.VideoCapture(0)
	pass
else:
	camara = cv2.VideoCapture(args["video"])
	pass

# Mantener el ciclo
while True:

	(grabbed, frame) = camara.read()
	
	# Si no obtenemos ningun  cuadrod el vide actual 
	# entonces terminamos el video
	if args.get("video") and not grabbed:
		break
		pass

	# redimencinar el cuadro, aplicar blur y convertir
	# al espacio de color HSV
	frame = imutils.resize(frame, width=600)
	blurred = cv2.GaussianBlur( frame, (11,11), 0 )
	hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
	



	#Si estamos 
	pass