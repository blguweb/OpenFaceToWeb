#! python3
# -*- encoding: utf-8 -*-
import ctypes
from flask import Flask, render_template, request,Response,jsonify
import json
import numpy
import base64
import cv2

'''
@File    :   app.py
@Time    :   2022/08/04 14:23:08
@Author  :   kdiominox
@Version :   1.0
'''
app = Flask(__name__)
global i
au_detection = ctypes.CDLL('../OpenFace-master/interlib/libinterface.so')
@app.route('/')
def video_sample():
    return render_template('camera.html')

 
@app.route('/receiveImage/', methods=["POST"])
def receive_image():
    global video_img,i
    if request.method == "POST":
        data = request.data.decode('utf-8')
        json_data = json.loads(data)
        video_img = json_data.get("imgData")
        img_np = numpy.fromstring(base64.b64decode(video_img), dtype='uint8')
        new_img = cv2.imdecode(img_np, 1)
        print(new_img.shape[0],new_img.shape[1])
        ## process img
        au_detection.AuInerface.restype = ctypes.c_char_p
        result = au_detection.AuInerface(new_img.shape[0],new_img.shape[1],new_img.ctypes.data_as(ctypes.c_char_p))
        i = i+ 1
        result = result.decode()
        # print("data:",result)
    return result



if __name__ == '__main__':
    i = 0
    au_detection.Init()
    app.run(debug=True,host='0.0.0.0',port=5000)