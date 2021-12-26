from flask import Flask
import cv2
import numpy as np
import gzip
from flask import make_response, json
import base64
import time

app = Flask(__name__)


@app.route("/")
def hello_world():
    return "<p>Hello, World!</p>"


@app.route("/recent/image", methods=["POST", "GET"])
def recent_image():

    # content = gzip.compress(json.dumps(very_long_content).encode('utf8'), 5)
    # response = make_response(content)
    # response.headers['Content-length'] = len(content)
    # response.headers['Content-Type'] = 'application/json; charset=utf-8'
    # response.headers['Content-Encoding'] = 'gzip'
    img = cv2.imread("../../src/cv/apple.jpg")
    depth_img = cv2.imread("../../src/cv/apple.jpg")
    ser_image = base64.b64encode(cv2.imencode(".jpg", img)[1]).decode()
    ser_depth_image = base64.b64encode(cv2.imencode(".jpg", depth_img)[1]).decode()
    return """{"rgb":"%s","depth":"%s","time":%d}""" % (
        ser_image,
        ser_depth_image,
        time.time() * 1000,
    )


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)
