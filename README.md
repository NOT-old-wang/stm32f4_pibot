# stm32f4_pibot
pibot 的嵌入式代码

## 环境
```
开发环境：ubuntu 16.04
硬件平台：stm32F4
```

## 代码下载
```
$ git clone https://github.com/NOT-old-wang/stm32f4_pibot.git
```

## 依赖
```
# 安装交叉编译器 
$ sudo apt-get install gcc-arm-none-eabi

# 安装openocd -- 用来支持jlink烧写
$ sudo apt-get install openocd
```

## 选择车体模型
```
4 种模型： 2轮差速、4轮差速、三轮全向、麦克纳母轮
在parma.mk 中选择 相应`model`，打开宏定义
```
## 编译
```
$ make
```

## 程序烧录
```
# 连接jlink
$ make burn
```