#include "rpcprovider.h"
#include "rpcheader.pb.h"
void RpcProvider::NotifyService(google::protobuf::Service *service)
{
    ServiceInfo service_info;
    service_info.m_service = service;

    //获取了服务对象的描述器
    const google::protobuf::ServiceDescriptor *pserviceDesc = service->GetDescriptor();
    //获取服务对象的名字
    const std::string& service_name = pserviceDesc->name();

    //获取服务对象的方法数量(也就是类里面的成员函数)
    int methodCount = pserviceDesc->method_count();

    std::cout<<"service_name:"<<service_name<<std::endl;

    //添加服务方法名及其对应的描述器到服务信息中
    for(int i=0;i<methodCount;i++){
        //获取服务内第i个服务方法的描述
        const google::protobuf::MethodDescriptor* pmethodDesc = pserviceDesc->method(i);
        std::string method_name = pmethodDesc->name();
        service_info.m_methodMap.insert({method_name,pmethodDesc});
        std::cout<<"method"<<i<<" name: "<<method_name<<std::endl;
    }
    //把这个服务及其信息添加到map中
    m_serviceMap.insert({service_name,service_info});
}

void RpcProvider::Run()
{
    std::string ip = MprpcApplication::GetInstance().GetConfig().Load("rpcserverip");
    uint16_t port = atoi(MprpcApplication::GetInstance().GetConfig().Load("rpcserverport").c_str());

    muduo::net::InetAddress address(ip,port);
    //创建TcpServer
    muduo::net::TcpServer server(&m_eventloop,address,"RpcServerProvider");
    //绑定回调，连接回调，消息读写回调,分离了网络代码和业务代码
    server.setConnectionCallback(std::bind(&RpcProvider::OnConnection,this,std::placeholders::_1));
    server.setMessageCallback(std::bind(&RpcProvider::OnMessage,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
    //设置muduo库线程数
    server.setThreadNum(4);

    std::cout<<"RpcProvider start service at ip:"<<ip<<" port: "<<port<<std::endl;
    //启动网络服务
    server.start();
    m_eventloop.loop();
}

void RpcProvider::OnConnection(const muduo::net::TcpConnectionPtr & connptr)
{
    if(!connptr->connected()){
        //rpc client连接诶断开了
        connptr->shutdown();

    }
}

/**
 * 框架内部，RPCProvider和RPCConsumer协商好通信用的protobuf数据类型
 * service_name method_name 
 * 定义protobuf的message类型，定义数据头的序列化和反序列化 service_name method_name args_len
 * header_size(4字节) + header_str + args_str
 */
void RpcProvider::OnMessage(const muduo::net::TcpConnectionPtr & connptr, muduo::net::Buffer * buffer, muduo::Timestamp timestamp)
{
    //网络上接收的远程rpc调用请求的字符流
    std::string receive_buf = buffer->retrieveAllAsString();
    //从字符流中读取前四个字节的内容
    uint32_t header_size = 0;
    receive_buf.copy((char*)&header_size,4,0);
    
    //读取header_str,也就是定义的message
    std::string rpc_header_str = receive_buf.substr(4,header_size);
    //反序列化数据，得到rpc请求的详细信息
    mprpc::RpcHeader rpcHeader;
    std::string service_name;
    std::string method_name;
    uint32_t args_size;
    if(rpcHeader.ParseFromString(rpc_header_str)){
        //数据头反序列化成功
        service_name = rpcHeader.service_name();
        method_name = rpcHeader.method_name();
        args_size = rpcHeader.args_len();     
    }else{
        //数据头反序列化失败
        std::cout<<"rpc_header_str"<<rpc_header_str<<" parse error~"<<std::endl;
    }

    //获取rpc方法的参数的字符流数据
    std::string args_str = receive_buf.substr(4+header_size,args_size);

    //打印调试信息
    std::cout<<"===================================="<<std::endl;
    std::cout<<"header_size: "<<header_size<<std::endl;
    std::cout<<"rpc_header_str:"<<rpc_header_str<<std::endl;
    std::cout<<"service_name:"<<service_name<<std::endl;
    std::cout<<"method_name:"<<method_name<<std::endl;
    std::cout<<"args_str:"<<args_str<<std::endl;
    std::cout<<"===================================="<<std::endl;

    //获取service对象和method对象
    auto service_iter = m_serviceMap.find(service_name);
    if(service_iter==m_serviceMap.end()){
        std::cout<<service_name<< "is not exist!"<<std::endl;
        return;
    }
    
    auto method_iter = service_iter->second.m_methodMap.find(method_name);
    if(method_iter==service_iter->second.m_methodMap.end()){
         std::cout<<"service:"<<service_name<< " -> method:"<<method_name<<" is not exist!"<<std::endl;
    }

    google::protobuf::Service *service = service_iter->second.m_service;//获取service对象
    const google::protobuf::MethodDescriptor *method = method_iter->second;//获取method对象

    //生成rpc方法需要调用的请求request和响应response
    google::protobuf::Message* request = service->GetRequestPrototype(method).New();//获取某个服务某个方法的request类型
    if(!request->ParseFromString(args_str)){
        std::cout<<"request parse error! content:"<<args_str<<std::endl;
    }
    google::protobuf::Message* response = service->GetResponsePrototype(method).New();//获取某个服务某个方法的response类型
    
    //给下面的method方法的调用，绑定一个Closure回调函数
    google::protobuf::Closure* closure =  
        google::protobuf::NewCallback<RpcProvider,const muduo::net::TcpConnectionPtr &, 
                google::protobuf::Message *>(this,&RpcProvider::SendRpcResponse,connptr,response);

    //在框架上根据远端rpc请求调用当前rpc节点上发布的rpc方法
    service->CallMethod(method,nullptr,request,response,closure);
     
}

void RpcProvider::SendRpcResponse(const muduo::net::TcpConnectionPtr & connptr, google::protobuf::Message *response)
{
    std::string response_str;
    //进行序列化
    if(!response->SerializeToString(&response_str)){
        std::cout<<"serialize response_str error"<<std::endl;
        connptr->shutdown();
        return;
    }
    //序列化成功，通过网络，把rpc方法执行的结果发送回rpc方法的调用方
    connptr->send(response_str);
    connptr->shutdown();//模拟http服务的短连接服务，由rpcprovider主动断开连接
}
