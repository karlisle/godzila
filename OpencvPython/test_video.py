#!/usr/bin/python2.7

#######################################
# Author: 	Kharl                     #
# Licence: 	Creative commons          #
# WebPage: 	karlislee.com             #
#                                     #
#######################################
import sys
import cv2 as cv
import numpy as np 


class Captura:
	def __init__(self):

		print("\t\nProcesamiento de imagenes con OpenCV")


		pass
	
	def imagen(self):
		img = cv.imread('saitama.jpg',0)
		cv.namedWindow('imagen', cv.WINDOW_NORMAL)
		cv.imshow('imagen', img)
		cv.waitKey(0)
		cv.destroyAllWindows()

		pass

	def video(self):
		capt = cv.VideoCapture(0)

		while True:
			ret, frame = capt.read()

			gray = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)

			hsv = cv.cvtColor(frame, cv.COLOR_BGR2HSV)
			lower_blue = np.array([110, 50, 50])
			upper_blue = np.array([130,255,255])

			mask = cv.inRange(hsv, lower_blue, upper_blue)

			res = cv.bitwise_and(frame, frame, mask = mask)

			#cv.imshow('Frame', frame)
			#cv.imshow('Mask', mask)
			#cv.imshow('Res', res)
			

			cv.imshow('frame', gray)
			if cv.waitKey(1) & 0xFF == ord('q'):
				break
			pass
		capt.release()
		cv.destroyAllWindows()
		pass

	def face(self):
		face_cascade = cv.CascadeClassifier('/home/kharl/Documents/OpencvPython/haarcascade_frontalface_alt2.xml')
		cap = cv.VideoCapture(0)

		while True:
			ret, img = cap.read()

			gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
			#cv.equalizeHist(gray, gray)

			hsv = cv.cvtColor(img, cv.COLOR_BGR2HSV)

			verdes_bajos = np.array([49,50,50], dtype=np.uint8)
			verdes_altos = np.array([80,255,255], dtype=np.uint8)

			mask = cv.inRange(hsv, verdes_bajos, verdes_altos)
			
			kernel = np.ones((3,3), np.uint8)
			transform = cv.erode(mask, kernel, iterations = 2)

			moments = cv.moments(mask)
			area = moments['m00']
			#print(area)

			if (area > 2000000):
				#Buscamos centros
				x = int(moments['m10']/moments['m00'])
				y = int(moments['m01']/moments['m00'])

				#Escribimos el valor de los centros
				print("x = ", x, ": y = ", y)
				
				# dibujar el centro con un rectangulo
				cv.rectangle( img, (x, y), (x+3, y+3),(0, 0, 255),2 ) 
				pass
			#cv.imshow('mask', mask)
			cv.imshow('cap', img)
			#cv.imshow('erode', transform)

			cv.imshow('img', img)

			faces = face_cascade.detectMultiScale(gray, 1.3, 5)
			
			for (x, y, w, h) in faces:
				cv.rectangle(img,(x,y),(x+w, y+h), (255, 0, 0), 2)
				pass
			cv.imshow('img', mask)
			pass
			
			if cv.waitKey(1) & 0xFF == ord('q'):
				break
			pass
		pass

	def figuras(self):
		capt = cv.VideoCapture(0)

		while True:
			# Capturar una imagen y convertirla a hsv
			ret, imagen = capt.read()
			hsv = cv.cvtColor(imagen, cv.COLOR_BGR2HSV)

			# Guardamos los colores hsv (azules)
			bajos = np.array([49,50,50], dtype=np.uint8)
			altos = np.array([80,255,255], dtype=np.uint8)

			# Creamos la mascara que detecta los colores
			mask = cv.inRange(hsv, bajos, altos)

			# Filtrar el ruido con close/open
			kernel = np.ones((6,6), np.uint8)

			mask = cv.morphologyEx(mask, cv.MORPH_CLOSE, kernel)
			mask = cv.morphologyEx(mask, cv.MORPH_OPEN, kernel)

			#Difuminar la mascara para suavisar los contornos y aplicar filtro canny
			blur = cv.GaussianBlur(mask, (5,5), 0)
			edges = cv.Canny(mask, 1, 2)

			# Detectar y guardar los contornos y sus áreas
			_, contours, hier = cv.findContours(edges,cv.RETR_TREE,cv.CHAIN_APPROX_SIMPLE)
			areas = [cv.contourArea(c) for c in contours]
			i = 0
			for extension in areas:
				if extension > 600:
					actual = contours[i]
					approx = cv.approxPolyDP(actual, 0.05*cv.arcLength(actual, True), True)
					if len(approx) == 4:
						cv.drawContours(imagen,[actual], 0, (0,0,255),2)
						cv.drawContours(mask, [actual],0,(0,0,255),2)
						pass
					pass
				i += 1
				cv.imshow('mask', mask)
				cv.imshow('Camara', imagen)
				pass
			if cv.waitKey(1) & 0xFF == ord('q'):
				break
				pass
			pass
		pass

	def strokeEdges(self, src, dst, blurKsize = 7, edgeKsize = 5):

		if blurKsize > 3:
			blurredSrc = cv.medianBlur(src, blurKsize)
			graySrc = cv.cvtColor(blurredSrc, cv.COLOR_BGR2GRAY)
			pass
		else:
			graySrc = cv.cvtColor(src, cv.COLOR_BGR2GRAY)
			pass
		cv.Laplacian(graySrc, cv.cv.CV_8U, graySrc, ksize = edgeKsize)
		normalizedInversealpha = (1.0 / 255) * (255 - graySrc)
		chanels = cv.split(src)
		for chanel in chanels:
			chanel1[:] = chanel * normalizedInversealpha
			pass
		cv.merge(chanels, dst)

		pass
	pass

app = Captura()
app.imagen()
#app.figuras()
#app.video()
#app.face()
