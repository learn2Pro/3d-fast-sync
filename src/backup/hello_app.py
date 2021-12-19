# from flask import Flask
# import cv2
# import numpy as np
# import gzip
# from flask import make_response, json
# import base64
# import time
# import threading
# import demo_video_depth

# from python.request_buffer import popHead

# app = Flask(__name__)
# lst = []

# @app.before_first_request
# def activate_job():
#     def run_job():
#         ## 这里写下video要输入的参数
#         demo_video_depth.main()
#     thread = threading.Thread(target=run_job)
#     thread.start()

# @app.route("/")
# def hello_world():
#     return "<p>Hello, World!</p>"

# @app.route("/recent/image",methods=['POST','GET'])
# def recent_image():
       
#     # content = gzip.compress(json.dumps(very_long_content).encode('utf8'), 5)
#     # response = make_response(content)
#     # response.headers['Content-length'] = len(content)
#     # response.headers['Content-Type'] = 'application/json; charset=utf-8'
#     # response.headers['Content-Encoding'] = 'gzip'
#     img,depth_img,ts = popHead()
#     # img = cv2.imread('../src/cv/apple2.jpg')
#     # depth_img = cv2.imread('../src/cv/apple.jpg')
#     ser_image = base64.b64encode(cv2.imencode('.jpg', img)[1]).decode()
#     ser_depth_image = base64.b64encode(cv2.imencode('.jpg', depth_img)[1]).decode()
#     return '''{"rgb":"%s","depth":"%s","time":%d}''' % (ser_image,ser_depth_image,ts)

