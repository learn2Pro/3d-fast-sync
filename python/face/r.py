#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
Created on Fri Jun 12 18:52:42 2020

@author: ansonwan
"""
from flask import Flask, request, jsonify
import json, requests

app = Flask(__name__)
app.debug = True


@app.route('/test', methods=['post'])
def post_http():
    if not request.data:  # 检测是否有数据
        return ('fail')
    params = request.data.decode('utf-8')
    # 获取到POST过来的数据，因为我这里传过来的数据需要转换一下编码。根据晶具体情况而定
    prams = json.loads(params)
    #params["time"] = json.loads(params["time"])
    print(prams)
    return {"info":"sucess"}


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=3458)
