# 简易RPC通信框架
## 简介


**服务提供者**：编写proto文件并编译生成pb.h和pb.cpp文件，编写服务类继承proto生成的服务类，重写虚函数即可,之后使用RpcProvider对象并调用NotifyService来发布服务。  
**服务消费者**：使用proto生成的Service_Stub对象并传入MprpcChannel，之后用stub对象传入Request和Response调用需要执行的方法，等待返回结果。  


**具体使用例子位于/mprpc/example文件夹下**


## 使用方法:
**mprpc文件夹下执行sudo ./autobuild.sh**
