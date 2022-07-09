# Lab3 渲染 使用说明

本项目为北京大学可视计算概论课程的第二个作业，并扩展应用为计算影像生成技术课程第三个大项目。框架代码为PKU VCL Lab编写。

## 一、如何运行程序

本项目为C/C++项目，未使用OpenGL等其他渲染器。目前支持 Windows 和 MacOS，不支持 Linux。

*（安装运行代码方式参照可视计算概论课程作业2 README.md）*

### 1.安装依赖

本项目需要以下工具构建：

* [git](http://git-scm.com/)：版本管理工具
* C++编译器：Windows 下推荐使用 Visual Studio（MSVC），MacOS 使用 XCode（Apple Clang）
* [xmake](https://xmake.io/#/)：项目构建工具，参考[官方文档](https://xmake.io/#/guide/installation)安装

### 2.编译

解压文件夹，如下方式编译。

编译命令：
```
xmake f -m release
xmake
```
* 重复编译出错时可以使用 `xmake clean` 清空缓存
* `xmake f -m debug` 可以切换到 debug 模式，切换之后需要运行 `xmake -r` 重新编译
* xmake 会自动下载所需要的第三方库文件并链接到项目中，如果下载过程中遇到网络问题，有如下解决方式：
  - 可运行 `xmake g --proxy_pac=github_mirror.lua` 将 github.com 重定向到 hub.fastgit.xyz
  - 可运行 `xmake g --pkg_searchdirs=<download-dir>` 并根据报错提示手动下载
  - 打开本地代理，使用命令行设置好环境变量 `HTTPS_PROXY="127.0.0.1:<port>"`，之后在命令行中运行 `xmake`

### 3.运行

运行命令：
```
xmake run
```
编译生成的二进制程序在 `bin` 目录下，也可以进入 `bin` 目录下手动执行

### 4.可以在windows下运行的可执行文件

主文件夹下有以下两个程序，是预先生成的可执行文件，第一个是采用witted style ray tracing渲染的场景，第一个是采用monte carlo path tracing渲染的场景。

```
 SoftRender_ray_tracing.exe
 SoftRender_path_tracing.exe
```

## 二、实现了哪些效果

### 1) 基础要求

基本实现了所需要的基础要求，未使用OpenGL等其他渲染器。



搭建了一个场景如下，

**场景：**包含深蓝色的天花板和碧绿色的地板，及四种不同颜色的墙面，四个墙角各放置一面镜子。

**物体：**中间有四个物体，分别是金属材质和蓝色实木的立方体，以及金属材质和黄色实木的立方体。

**光源：**顶上有一个面光源和四个吊灯状的点光源，面光源一直开着，四个点光源的开灯状态随机。



场景每次创建时自动生成，

随机一种**光照状态**，四个吊灯的开关状态，每个灯开or关，一共有$2^4=16$种开关灯状态。

随机一种**相机位姿**，位姿有从前、后、左、右、左前到右后、右前到左后、左后到右前、右后到左前八种状态。可以看出，任何一种场景下，都存在一组遮挡关系。



本项目分别使用witted style ray tracing和monte carlo path tracing两种方式渲染场景，witted style ray tracing方法渲染速度快，但是效果不真实；monte carlo path tracing方法渲染速度很慢，但是效果真实。

可以调节src\renderer\renderer.h中Renderer类下monte_carlo的值来设定渲染方法，为false是witted style ray tracing，为true是monte carlo path tracing。

以下例图分别为witted style ray tracing和monte carlo path tracing得到的不同光照和视角的四张例图。

#### witted style ray tracing

<img src="images\image-20220626180802783.png" alt="image-20220626180802783" style="zoom:25%;" /><img src="images\image-20220626180510978.png" alt="image-20220626180510978" style="zoom:25%;" />

#### monte carlo path tracing

<img src="images\image-20220626174206942.png" alt="image-20220626174206942" style="zoom: 25%;" /><img src="images\image-20220626175328567.png" alt="image-20220626175328567" style="zoom: 25%;" />

### 2）升级要求

本项目实现了

**阴影**：在whitted style ray tracing物体旁存在硬阴影，在monte carlo path tracing物体旁存在软阴影，

**镜面反射**：四角存在的镜子展现了较为真实的镜面效果。

**软阴影**：在monte carlo path tracing物体旁存在软阴影，

**面光源**：天花板正中间有一个用多个点光源模拟的面光源。

**间接光照**：从镜面中也可以看出光线有多次反弹。可调节src\renderer\renderer.h中Renderer类下bounce_num的值来控制最大反弹次数。同时反弹多次后，光线强度减弱，到光强衰减到一定程度时，不再反弹。

调换视角（效果不好）：**在窗口中操作**鼠标，相机视角会有（按住鼠标左键拖动）旋转，（按住鼠标右键拖动）平移，（滚动鼠标滚轮）缩放，窗口及时清空上一帧buffer里的值，生成新视角的图片。





