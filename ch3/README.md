# ch3：创建 ROS 消息和服务

## 1. msg 和 srv 介绍
- msg（消息）：msg 文件就是文本文件，用于描述 ROS 消息的字段。它们用于为不同编程语言编写的消息生成源代码。
- srv（服务）：一个 srv 文件描述一个服务。它由两部分组成：请求（request）和响应（response）。


msg 文件存放在软件包的 msg 目录下，srv 文件则存放在 srv 目录下。

msg 文件就是简单的文本文件，每行都有一个字段类型和字段名称。面是一个 msg 文件的样例，它使用了 Header，string，和其他另外两个消息的类型：
```
  Header header
  string child_frame_id
  geometry_msgs/PoseWithCovariance pose
  geometry_msgs/TwistWithCovariance twist
```


srv 文件和 msg 文件一样，只是它们包含两个部分：请求和响应。这两部分用一条 --- 线隔开。下面是一个 srv 文件的示例：
```
int64 A
int64 B
---
int64 Sum
```
在上面的例子中，A 和 B 是请求，Sum 是响应。

## 2. 使用 msg
在之前创建的软件包里定义一个新的消息。
```bash
roscd ch3
mkdir msg
echo "int64 num" > msg/Num.msg
```

不过还有关键的一步：我们要确保 msg 文件能被转换为 C++、Python 和其他语言的源代码。
打开 package.xml, 确保它包含以下两行且没有被注释。如果没有，添加进去：
```xml
  <build_depend>message_generation</build_depend>
  <exec_depend>message_runtime</exec_depend>
```

在 CMakeLists.txt 文件中，为已经存在里面的 find_package 调用添加 message_generation 依赖项，这样就能生成消息了。直接将 message_generation 添加到 COMPONENTS 列表中即可，如下所示：
```
find_package(catkin REQUIRED COMPONENTS
   roscpp
   rospy
   std_msgs
   message_generation
)
```
还要确保导出消息的运行时依赖关系：
```
catkin_package(
  ...
  CATKIN_DEPENDS message_runtime ...
  ...)
  ```
找到如下代码块：

```
# add_message_files(
#   FILES
#   Message1.msg
#   Message2.msg
# )
```
删除#符号来取消注释，然后将 Message*.msg 替换为你的.msg 文件名。

手动添加.msg 文件后，我们要确保 CMake 知道何时需要重新配置项目。

现在必须确保 generate_messages() 函数被调用，取消下面几行的注释：

```
# generate_messages(
#   DEPENDENCIES
#   std_msgs
# )
```


## 3. 使用 srv
使用之前创建的包再来创建服务：

```bash
roscd ch3
mkdir srv
```

我们可以从 rospy_tutorials 包中复制一个服务：
```bash
roscp rospy_tutorials AddTwoInts.srv srv/AddTwoInts.srv
```
如前所述，在构建时，对package.xml和CMakeLists.txt进行修改。
此外，你也需要像之前对消息那样在 package.xml 中修改服务字段，因此请看上面描述的所需附加依赖项。
