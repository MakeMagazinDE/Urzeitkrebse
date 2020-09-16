#!/usr/bin/python3
import picamera, time
cam = picamera.PiCamera()
cam.resolution = (2592,1944)
t = time.strftime("%Y%m%d_%H%M")
cam.capture('/home/pi/bilder/foto_%s.jpg' %t)
time.sleep(2)
cam.resolution = (1024,768)
cam.start_recording('/home/pi/bilder/video_%s.h264' %t)
time.sleep(10)
cam.stop_recording()
