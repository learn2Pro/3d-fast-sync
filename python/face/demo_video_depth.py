# coding: utf-8

__author__ = 'cleardusk'

from os import error
import sys
import argparse
import cv2
import yaml
import requests
from FaceBoxes import FaceBoxes
from TDDFA import TDDFA
from utils.render import render
# from utils.render_ctypes import render  # faster
from utils.depth import depth
from utils.pncc import pncc
from utils.uv import uv_tex
from utils.pose import viz_pose
from utils.serialization import ser_to_ply, ser_to_obj
from utils.functions import draw_landmarks, get_suffix
from utils.tddfa_util import str2bool
import time
# def img_read():
#     url = 'rtsp://admin:123qwe123@192.168.254.5:554/Streaming/Channels/101'
#     cap = cv2.VideoCapture(url)
#     return cap
import socket
import json
import six

from flask import Flask
import cv2
import numpy as np
import gzip
from flask import make_response, json
import base64
import threading
import request_buffer


# app = Flask(__name__)

# global buffer
# buffer = []
# @app.route("/recent/image", methods=['POST', 'GET'])

savepath = './examples/results/img/'
# if not os.path.exists('/data/5gmsg_Share/pic'):
#     os.makedirs('/data/5gmsg_Share/pic')


def main(args):

    cfg = yaml.load(open(args.config), Loader=yaml.SafeLoader)

    # Init FaceBoxes and TDDFA, recommend using onnx flag
    if args.onnx:
        import os
        os.environ['KMP_DUPLICATE_LIB_OK'] = 'True'
        os.environ['OMP_NUM_THREADS'] = '12'

        from FaceBoxes.FaceBoxes_ONNX import FaceBoxes_ONNX
        from TDDFA_ONNX import TDDFA_ONNX

        face_boxes = FaceBoxes_ONNX()
        tddfa = TDDFA_ONNX(**cfg)
    else:
        gpu_mode = args.mode == 'gpu'
        tddfa = TDDFA(gpu_mode=gpu_mode, **cfg)
        face_boxes = FaceBoxes()

    # Given a still image path and load to BGR channel
    url = 'rtsp://admin:scuxxxs123@192.168.1.64:554/Streaming/Channels/101'
    #url = 'rtsp://admin:123qwe123@192.168.1.168:554/Streaming/Channels/101'
    #url = args.img_fp
   # t1 = time.time()
    cap = cv2.VideoCapture(url)
    num = 0
    print('''start record with camera url:%s,state:%s,time:%s''' %
          (url, cap.isOpened(), time.time()))
    while(cap.isOpened()):
        ret, img = cap.read()

        #  img = cv2.imread(args.img_fp)

        # Detect faces, get 3DMM params and roi boxes
        boxes = face_boxes(img)
        n = len(boxes)
        if n != 0:
            # print(f'Detect {n} faces')
            param_lst, roi_box_lst = tddfa(img, boxes)
            # Visualization and serialization
            dense_flag = args.opt in (
                '2d_dense', '3d', 'depth', 'pncc', 'uv_tex', 'ply', 'obj')
            # old_suffix = get_suffix(args.img_fp)
            new_suffix = f'.{args.opt}' if args.opt in (
                'ply', 'obj') else '.jpg'

            wfp = f'examples/results/video_depth/{num}_{args.opt}' + new_suffix
        #  new_suffix = f'.jpg'
            wfp_input = f'examples/results/video_depth/{num}' + new_suffix
            ver_lst = tddfa.recon_vers(
                param_lst, roi_box_lst, dense_flag=dense_flag)

            if args.opt == 'depth':
                # if `with_bf_flag` is False, the background is black
                dep = depth(img, ver_lst, tddfa.tri,
                            show_flag=args.show_flag, wfp=wfp, with_bg_flag=False)

            elif args.opt == 'uv_tex':
                uv_tex(img, ver_lst, tddfa.tri,
                    show_flag=args.show_flag, wfp=wfp)

            elif args.opt == 'obj':
                ser_to_obj(img, ver_lst, tddfa.tri,
                        height=img.shape[0], wfp=wfp)
            else:
                raise ValueError(f'Unknown opt {args.opt}')

            if 0xFF == ord('q'):
                break
            # path1 = os.path.join(savepath, str(num)+'_rgb.jpg')
            # path2 = os.path.join(savepath, str(num) + '_depth.jpg')
            # cv2.imwrite(path1, img)
            # cv2.imwrite(path2, dep)
            request_buffer.append(img, dep, time.time()*1000)
            num = num + 1

        # result = {}

        else:
            print(f'==================>No face detected, skip==================>')
            continue
        

    cv2.destroyAllWindows()
    cap.release()


if __name__ == '__main__':
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

    args = parser.parse_args()
    main(args)
   # app.run(host='0.0.0.0', port=8080)

  #
