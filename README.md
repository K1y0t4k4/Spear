<h1 align=center><b>Spear</b></h1>
<h6 align=center>C2 framework</h3>
<div align=center>
    <img src="https://img.shields.io/badge/Github-K1y0t4k4-yellow?style=plastic&logo=github" />
    <img src="https://img.shields.io/badge/Python-yellow?style=plastic&logo=python" />
    <img src="https://img.shields.io/badge/C/C++-yellow?style=plastic&logo=c&logoColor=blue" />
</div>

<h1 align=center><b>In development</b></h1>

<div align=center>
    <img src="https://github.com/K1y0t4k4/Spear/blob/main/img/banner.png" />
</div>

---

# 简介
一个基于https的简易c2框架
## 特性
- 使用https通讯
- 自动重连到服务端
- 伪终端
- 字符串混淆
- 隐藏导入函数，一定程度上的静态免杀

---

# 支持的命令
|命令|描述|
|---|---|
|所有非交互的powershell命令|如cd ls ipconfig ...|
|bsod|蓝屏|
|close|与agent断开连接，但不关闭agent|
|exit|与agent断开连接，并关闭agent|
---

# 使用方法
## 配置agent
- 打开Spear/Agent/src/SpearAgent.cpp
```cpp
Instance.Config.Address.server = std::wstring(WOBF(L"127.0.0.1"));
Instance.Config.Address.port   = 8000;
```
- 将**127.0.0.1**修改成你服务器地址，端口默认**8000**
## 编译agent.exe
```shell
cd Spear/Agent/build
cmake -DCMAKE_BUILD_TYPE:STRING=Release -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_C_COMPILER:FILEPATH=gcc.exe -DCMAKE_CXX_COMPILER:FILEPATH=g++.exe --no-warn-unused-cli -S../ -G "MinGW Makefiles" ..
make
```

## 打开服务端
```shell
cd Spear/TeamServer
python TeamServer.py
```