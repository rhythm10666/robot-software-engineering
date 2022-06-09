

# 机器人软件工程学大作业——TurtleBot Simulation

<center>1911717 第5组 郑可欣</center>

系统版本： Ubuntu18.04
仿真平台： ROS（Melodic） + Gazebo + Moveit! + Rviz

## 1. 任务要求

重点考察在运用仿真、语音、导航、图像、机械臂（创新加分项）的能力，以及对以上基础模块融合运用的创新能力。

## 2. 任务设计

- 建图：在Gazebo下搭建仿真场景，通过键盘操控TurtleBot机器人运动，通过GMapping实现进行机器人的定位与建图。
- 导航：在起始位置，通过语音启动RViz模块，告知机器人前往目标点抓取某物，随后使用AMCL进行导航，机器人从起始位置移动至目标点。
- 物体识别：到达目标点后使用YOLO对待抓取物体进行物体识别，通过坐标变换定位目标物体。
- 抓取：Gazebo与Moveit! 结合，利用获得的物体位姿控制机械臂完成对指定物体的抓取。


## 3. 安装依赖

- turtlebot_gazebo

```bash
sudo apt-get install ros-melodic-gazebo-ros-pkgs ros-melodic-gazebo-ros-control
sudo apt-get install ros-melodic-turtlebot*
```

- turtlebot_arm

```bash
git clone https://github.com/turtlebot/turtlebot_arm.git
```

- YOLO_v3

```bash
git clone https://github.com/leggedrobotics/darknet_ros.git
```

## 4. 语音控制

- PocketSphinx：

```bash
roslaunch pocketsphinx kws.launch dict:=/home/longway/turtlebot_ws/src/pocketsphinx/demo/voice_cmd.dic kws:=/home/longway/turtlebot_ws/src/pocketsphinx/demo/voice_cmd.kwlist
rostopic echo /kws_data
rosrun pocketsphinx voice_control_example.py
```

## 5. Gazebo 场景构建与建图

- 打开 Gazebo 后，按 Ctrl+B 绘制新地图，或者直接insert现有的模型：

```bash
roslaunch turtlebot_gazebo turtlebot_world.launch
```
- 机器人定位建图：

```bash
roslaunch turtlebot_teleop keyboard_teleop.launch
roslaunch turtlebot_gazebo gmapping_demo.launch
roslaunch turtlebot_rviz_launchers view_navigation.launch
rosrun image_view image_view image:=/camera/rgb/image_raw
```

- 保存地图：

```bash
rosrun map_server map_saver -f /home/longway/turtlebot_ws/src/turtlebot_simulator/turtlebot_gazebo/maps/coke
```


## 6. 机器人定点导航

```bash
roslaunch turtlebot_gazebo turtlebot_world.launch
roslaunch turtlebot_gazebo amcl_demo.launch map_file:=/home/longway/turtlebot_ws/src/turtlebot_simulator/turtlebot_gazebo/maps/coke.yaml
roslaunch turtlebot_rviz_launchers view_navigation.launch
# rosrun my_pkg navigation.py
```

## 7. 物体识别与机械臂抓取

- 将turtlebot_arm连接到底盘上。在~/turtlebot_ws/src/turtlebot/turtlebot_description/robots/kobuki_hexagons_kinect.urdf.xacro中添加：

```xml
<xacro:include filename="$(find turtlebot_arm_description)/urdf/turtlebot_arm.xacro"/>
```

- 利用YOLO算法完成图像中的物体检测（设定目标物体）：

```bash
roslaunch darknet_ros yolo_v3.launch
```

- 机器人在三维空间中定位目标物体：

  物体检测框 -- 物体像素坐标 (u, v) -- 对应的点云坐标 (xc, yc, zc)

```bash
rosrun darknet_ros test_darknet.py  
```

- Moveit! 与 Gazebo 联合仿真：

```bash
roslaunch turtlebot_arm_moveit_config moveit_planning_execution.launch 
roslaunch turtlebot_gazebo turtlebot_world_moveit.launch 
```

- 调用moveit中配置好的ik求解器和控制器完成抓取操作：


```bash
rosrun turtlebot_arm_moveit_demos pick_and_place.py   
```

## 8. 分工

陈世达：物体检测、PPT

胥润：建图、文档

陈涛：语音识别、导航

关子旭：建图、场景搭建

郑可欣：导航、物体检测、机械臂

## 9. 参考资料

<https://github.com/HilbertXu/ros_task_frame>

<https://www.guyuehome.com/7639>

<https://www.ncnynl.com/category/Turtlebot-arm-code/>

