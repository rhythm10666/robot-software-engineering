# 1. 语音控制

## 1.1 sound_play相关指令
[教程](http://wiki.ros.org/sound_play/Tutorials)
依赖：
```bash
sudo apt-get install ros-melodic-audio-common
sudo apt-get install libasound2
```
运行：
```bash
roscore
rosrun sound_play soundplay_node.py
rosrun sound_play say.py “Hello world”
```

## 1.2 Sphinx相关指令
[教程](http://wiki.ros.org/pocketsphinx)

依赖：
```bash
sudo apt-get install python-pip python-dev build-essential
sudo pip install --upgrade pip
sudo apt-get install libasound-dev
sudo apt-get install python-pyaudio
sudo pip install pyaudio
sudo apt-get install swig
sudo apt-get install libpulse-dev
sudo pip install pocketsphinx
cd ~/catkin_ws/src
git clone https://github.com/Pankaj-Baranwal/pocketsphinx​
cd ~/catkin_ws
cd ..
catkin_make
```
运行：
实例1
```bash
roslaunch pocketsphinx kws.launch dict:=/home/longway/turtlebot_ws/src/pocketsphinx/demo/voice_cmd.dic kws:=/home/longway/turtlebot_ws/src/pocketsphinx/demo/voice_cmd.kwlist
rostopic echo /kws_data
```
实例2
```bash
roslaunch pocketsphinx lm.launch dict:=/home/longway/turtlebot_ws/src/pocketsphinx/demo/robocup/robocup.dic lm:=/home/longway/turtlebot_ws/src/pocketsphinx/demo/robocup/robocup.lm
rostopic echo /lm_data
```

# 2. 模板匹配
Kinect 相关依赖安装:
```bash
sudo apt-get install ros-melodic-openni-* ros-melodic-openni2-* ros-melodic-freenect-*
echo $TURTLEBOT_3D_SENSOR
echo "export TURTLEBOT_3D_SENSOR=kinect" )) .bashrc
```
启动相机:
```bash
roslaunch turtlebot_bringup minimal.launch
roslaunch freenect_launch freenect-registered-xyzrgb.launch
```
测试相机:
```bash
rosrun image_view image_view image:=/camera/rgb/image_raw 
rosrun image_view image_view image:=/camera/depth_registered/image_raw
```

# 3. 建图导航

## 3.1 安装依赖
### 3.1.1 Kinect 依赖
OpenNI 库、freenect 驱动：
```bash
sudo apt-get install ros-melodic-openni-* ros-melodic-openni2-* ros-melodic-freenect-*
```
检查默认的3D传感器：
```bash
echo $TURTLEBOT_3D_SENSOR
#Output: kinect
```
如果不是 kinect，设置环境变量, 在.bashrc最后添加一下语句：
```bash
export TURTLEBOT_3D_SENSOR=kinect
```
### 3.1.2 SLAM与导航依赖
```bash
#安装gmapping功能包，创建并输出基于概率的二维栅格地图
sudo apt-get install ros-melodic-gmapping
# 安装move_base功能包，ROS导航的配置、运行、交互接口
sudo apt-get install ros-melodic-move-base
#安装DWA local planner 功能包，该包使用DWA(Dynamic Window Approach)算法实现平面上移动机器人局部导航功能
sudo apt-get install ros-melodic-dwa-local-planner
#安装amcl功能包，是移动机器人二维环境下的概率定位系统
sudo apt-get install ros-melodic-navigation
```
```bash
# 安装 turtlebot_viz，导航相关的可视化工具
cd catkin_ws/src
git clone https://github.com/ncnynl/turtlebot_viz
cd ~/catkin_ws
catkin_make
#安装map_server功能包，它提供了map_server的ROS节点、地图 数据、实用程序，允许动态生成的映射保存到文件
sudo apt-get install ros-melodic-map-server
```

## 3.2 建图
创建地图：
```bash
roslaunch turtlebot_bringup minimal.launch
roslaunch turtlebot_navigation gmapping_demo.launch
roslaunch turtlebot_rviz_launchers view_navigation.launch
```
保存地图：
```bash
rosrun map_server map_saver -f /tmp/my_map
```

## 3.3 机器人定点导航
设置地图文件：
```bash
roslaunch turtlebot_navigation amcl_demo.launch map_file:=/tmp/my_map
roslaunch turtlebot_rviz_launchers view_navigation.launch --screen
```
源码控制命令：
```bash
roslaunch turtlebot_bringup minimal.launch
roslaunch turtlebot_navigation amcl_demo.launch map_file:=/tmp/my_map.yaml
roslaunch turtlebot_rviz_launchers view_navigation.launch --screen 
rosrun my_pkg navigation.py
```

# 4. 机器人手臂抓取

## 4.1 参考资料
[TurtleBot Arm](http://wiki.ros.org/turtlebot_arm/)
[Hardware](https://makezine.com/projects/build-an-arm-for-your-turtlebot/ )
[ROS dynamixel_motor](http://wiki.ros.org/dynamixel_motor)

## 4.2 依赖安装
### 4.2.1 ros-melodic-dynamixel
```bash
sudo apt-get install ros-melodic-dynamixel-*
```
### 4.2.2. ros-project
```bash
cd ~/catkin_ws/src
git clone https://github.com/arebgun/dynamixel_motor.git 
cd ../
catkin_make
```

## 4.3 检查硬件连接
### 4.3.1 电源模块指示灯
```bash
ls /dev/ttyUSB0
```
### 4.3.2 USB 权限
```bash
sudo dmesg -c
sudo chmod 666 /dev/ttyUSB0
```

## 4.4 启动控制器管理器
```bash
cd ~/catkin_ws/src
catkin_create_pkg my_dynamixel dynamixel_controllers std_msgs rospy roscpp
cd ~/catkin_ws
catkin_make
cd src/my_dynamixel/ 
mkdir launch
cd launch
touch controller_manager.launch
```

## 4.5 启动controller
```bash
roslaunch my_dynamixel controller_manager.launch
roslaunch my_dynamixel start_tilt_controller.launch
```

## 4.6 发布机械臂话题
```bash
rostopic pub -1 /tilt_controller/command std_msgs/Float64 -- 0.5
```