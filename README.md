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