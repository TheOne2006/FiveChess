## 介绍
    这是2024秋季PKU计算概论课程的大作业，由长岛冰茶顿顿编写。
## 项目结构
  项目分为四个大部分：AI部分、核心数据维护部分、UI部分、IO部分。
## 编译方式
   整体采用makefile管理整个项目，需使用TDM-GCC在Windows平台上进行编译(mingw-win64编译器无法编译)。 在项目目录内使用make指令即可完成编译。最后会在output文件下生成exe文件，和项目目录下的res文件夹放在同一路径下即可运行。
## AI部分
  AI部分有两个核心头文件，AI.h与Value.h，整体采取Alpha-Beta剪枝的方法，使用启发式搜索加深搜索层数。Value.h里面对各种棋型进行评分，采用Hash算法简化一些重复搜索，加快检索速度。并且在一些特殊棋型中使用了特判算法。
## UI部分
  使用EasyX库进行UI的编写，库文件和头文件已经包含于项目中，UI效果见演示
## 核心数据维护
  这部分有两个核心头文件:judge.h，point.h，里面暴露了各种对外的接口，具有多种记录下棋状态的变量。
## IO部分
  这部分包含的一个核心头文件:my_io.h，暴露了各种IO接口。负责整体程序对外的文件管理。
