from flask import Flask
import cv2
import numpy as np
import gzip
from flask import make_response, json
import base64
import time
import threading
import demo_video_depth
import request_buffer
import argparse
from utils.tddfa_util import str2bool

app = Flask(__name__)


@app.before_first_request
def activate_job():
    def run_job():
        # 这里写下video要输入的参数
        parser = argparse.ArgumentParser(
            description='The demo of still image of 3DDFA_V2')
        parser.add_argument('-c', '--config', type=str,
                            default='configs/mb05_120x120.yml')
        parser.add_argument('-f', '--img_fp', type=str,
                            default='examples/inputs/trump_hillary.jpg')
        parser.add_argument('-m', '--mode', type=str,
                            default='cpu', help='gpu or cpu mode')
        parser.add_argument('-o', '--opt', type=str, default='depth',
                            choices=['2d_sparse', '2d_dense', '3d', 'depth', 'pncc', 'uv_tex', 'pose', 'ply', 'obj'])
        parser.add_argument('--show_flag', type=str2bool, default='false',
                            help='whether to show the visualization result')
        parser.add_argument('--onnx', action='store_true', default=True)

        args = parser.parse_args(["--onnx", "-o", "depth"])
        demo_video_depth.main(args)
    thread = threading.Thread(target=run_job)
    thread.start()


@app.route("/")
def hello_world():
    return "<p>Hello, World!</p >"


@app.route("/recent/image", methods=['POST', 'GET'])
def recent_image():

    # content = gzip.compress(json.dumps(very_long_content).encode('utf8'), 5)
    # response = make_response(content)
    # response.headers['Content-length'] = len(content)
    # response.headers['Content-Type'] = 'application/json; charset=utf-8'
    # response.headers['Content-Encoding'] = 'gzip'
    img, depth_img, ts = request_buffer.popHead()
    if(ts == 0):
        return '''{"rgb":"%s","depth":"%s","time":%d}''' % ("", "", ts)
    else:
        ser_image = base64.b64encode(cv2.imencode('.jpg', img)[1]).decode()
        ser_depth_image = base64.b64encode(
            cv2.imencode('.jpg', depth_img)[1]).decode()
        return '''{"rgb":"%s","depth":"%s","time":%d}''' % (ser_image, ser_depth_image, ts)

if __name__ == '__main__':
    app.run(host = "0.0.0.0", port = 5000)