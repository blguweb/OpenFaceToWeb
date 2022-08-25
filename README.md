该项目实现前端调用摄像头识别用户表情，并将表情结果呈现在网页上。
# 功能
1、实现openface监测au，识别基本表情  
2、前端调用摄像头，将图像不断传到后端处理，然后将结果传到前端。 
3、将openface打包成so库，后端python调用openface的so库进行识  别

# 准备

OS: Ubuntu 18.04  
python3.8

安装Openface框架，[安装步骤](https://github.com/TadasBaltrusaitis/OpenFace/wiki/Unix-Installation)

    bash ./download_models.sh
    sudo bash ./install.sh

可以以下代码测试：是否ok:  
for videos:

    ./bin/FaceLandmarkVid -f "../samples/changeLighting.wmv" -f "../samples/2015-10-15-15-14.avi"

for images:

    ./bin/FaceLandmarkImg -fdir "../samples/" -wild

for multiple faces in videos:

    ./bin/FaceLandmarkVidMulti -f ../samples/multi_face.avi

for feature extraction (facial landmarks, head pose, AUs, gaze and HOG and similarity aligned faces):

    ./bin/FeatureExtraction -verbose -f "../samples/default.wmv"

# 代码框架

该项目的根本逻辑在于根据openface的[API](https://github.com/TadasBaltrusaitis/OpenFace/wiki/API-calls)写一个监测人脸au的接口，然后将整个openface打包成库，最后在后端引用库进行调用，识别表情后将结果呈现在前端web。


------------
 - auweb 这个是前后端的代码
 - openFace 这是需要在openface源码中更换的代码
      - exe
          - AuDetection 这是可以独立前后端运行，在ubuntu中实现调用电脑摄像头进行表情监测，结果在终端打印出来。
          - Interface 这是 接口代码，安装成库后要调用的函数
      - interlib 这里面是打包好的文件，但是对user来说没用，所以不复制过去也行。
      - lib
          - local 修改了cmakelist和代码文件，需要更换。
              - LandmarkDetector
              - FaceAnalyser
      - CMakeLists.txt  

-------------

# 运行

1、openFace文件中是按照openface 源码的文件层进行排布，因此按照对应的文件层 添加或者更换里面的代码文件。  
2、修改里面调用model和文件的路径，基本在我更换的文件中修改即可。注意如果我用的是绝对路径的话，都必须用绝对路径！  
3、需要对库进行重新编译，不然会报 没有fpic 的错误。  
&emsp;&emsp;3.1 这就是为什么LandmarkDetector 的CMakeLists.txt 更改的原因，里面都添加了  

    add_compile_options(-fPIC)

&emsp;&emsp;3.2 重新编译 在openface-master中

    wget http://dlib.net/files/dlib-19.13.tar.bz2
    tar xvf dlib-19.13.tar.bz2
    cd dlib-19.13
    mkdir -p build
    cd build
    cmake -DBUILD_SHARED_LIBS=ON -DDLIB_USE_LAPACK=1 ..
    cmake --build . --config Release
    sudo make install
    sudo ldconfig
    cd ..
    rm -r dlib-19.13.tar.bz2

&emsp;&emsp;3.3 opencv也是 查看install.sh的代码和网上的解决方案自行重新编译  
<br>
4、在./auWeb中运行：

    python3 app.py
    
5、输入终端中出现的网址，http://127.0.0.1:5000/

