# 定时拉取http请求服务

- 拉取图像src/request_image.cpp
  - prerequirements
    - `cmake`
    - `opencv`
      1. git clone https://github.com/opencv/opencv.git
      2. mkdir build && cd build/ 
      3. cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_EXAMPLES=ON -D OPENCV_GENERATE_PKGCONFIG=YES ../ && make
    - `SFML(TODO)`
      1. git clone https://github.com/SFML/SFML.git
      2. mkdir build && cd build/ 
      3. cmake ../ && make
  - 打包命令
    1. `mac` - /usr/bin/clang++ -std=c++11 -stdlib=libc++ -g ${workdir}/3d-fast-sync/src/request_image.cpp -o ${workdir}/3d-fast-sync/src/request_image
    2. `linux` - /usr/bin/g++ -std=c++11 -stdlib=libc++ -g ${workdir}/3d-fast-sync/src/request_image.cpp -o ${workdir}/3d-fast-sync/src/request_image
    3. `windows` - C:/msys64/mingw64/bin/g++.exe -std=c++11 -stdlib=libc++ -g ${workdir}/3d-fast-sync/src/request_image.cpp -o ${workdir}/3d-fast-sync/src/
        - more @see this doc https://code.visualstudio.com/docs/cpp/config-mingw
  - 输出
    1. 打包完成后会输出一个request_image的可执行文件，执行即可
  - 配置项
    1. iterval_in_millis 拉取间隔（ms），1000标识1秒
    2. running_time_in_minutes 程序运行时间(minutes), 10标识10分钟
    3. request_url 请求对应的地址，可以修改为http://ip:port格式代表对方的ip和端口
    4. running_threads_num 拉取并发线程数量，默认为16，如果画面卡，提升这个数量
- 部署服务器
  - 安装Python环境(python3.8)
    1. mac `brew install python3`
    2. linux `yum install python3`
    3. window `https://www.python.org/downloads/`
  - 进入python/ 文件夹
    1. 安装python虚拟环境 `python3 -m venv flask`
    2. 运行虚拟环境 `. flask/bin/activate`
    3. 按照numpy,flask,cv2 `pip install numpy flask opencv-python`
    4. 运行flask服务器环境 `export FLASK_APP=hello && flask run`
  - 替换图片
    1. rgb 图片为 apple2.jpg, 可以替换为自己要的图片，名字一样即可
    2. depth 图片为 apple.jpg, 可以替换为自己要的图片，名字一样即可
- serving端采集图像并提供http服务
  - 启动
    1. docker run -it -v /c/Users/admin/Desktop/3d-fast-sync:/usr/local/export/3d-fast-sync -p 192.168.1.65:15000:5000 -p 127.0.0.1:15000:5000 3ddfa:v2
    2. cd /usr/local/export/3d-fast-sync/python/face
    3. 差机器：python3 app.py
    4. 好机器： gunicorn -w 4 -b 0.0.0.0:5000 app:app
  - 配置参数
    1. 核心http服务文件[python/face/request_buffer.py,python/face/app.py]
    1. python/face/request_buffer.py max_len=5 , 数字越大延迟越高（最大20），数字越小延迟越小（最小1），但是会跳帧