# openGL-FPSdemo

一个基于openGL各类开源库的第一人称视角简易图形化引擎(?)


## 使用模板(CMake)
+ https://github.com/ArthurSonzogni/OpenGL_CMake_Skeleton
+ https://github.com/cmmw/imgui-glfw-glad-glm

## 构建方式

需递归拉取lib\下的[glew](https://github.com/Perlmint/glew-cmake/tree/8260c05ee0121094a0dc9a53f610f514659763d6)、[glfw](https://github.com/glfw/glfw/tree/dd8a678a66f1967372e5a5e3deac41ebf65ee127)与[glm](https://github.com/g-truc/glm/tree/cc98465e3508535ba8c7f6208df934c156a018dc)库，CMake将递归构建并链接到项目内

之后正常构建即可

## 目前完成的功能

+ 第一人称摄像机（平地移动视角/自由视角，跳跃，重力自由落体）

+ 贴图、基本形状加载

+ 简单冯氏光照模型（点光源、聚光灯源、材质贴图）

+ 矩形碰撞箱与AABB碰撞检测

+ .OBJ模型加载（有一些bug，正在完善）

  

