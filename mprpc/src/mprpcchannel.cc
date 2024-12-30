#include "mprpcchannel.h"
#include "mprpcapplication.h"
#include "rpcheader.pb.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string>
/*header_size+service_name method_name args_size + args*/
void MprpcChannel::CallMethod(const google::protobuf::MethodDescriptor* method,google::protobuf::RpcController* controller, const google::protobuf::Message* request,
                          google::protobuf::Message* response, google::protobuf::Closure* done)
{
    const google::protobuf::ServiceDescriptor* sd = method->service();
    std::string service_name = sd->name();
    std::string method_name = method->name();

    //获取参数的序列化字符串 args_size
    uint32_t request_str_size = 0;
    std::string request_str;
    if(request->SerializeToString(&request_str)==0){
        std::cout<<"serialize request error!"<<std::endl;
        return;
    }
    request_str_size = request_str.size();

    //定义rpc的请求header
    mprpc::RpcHeader rpcHeader;
    rpcHeader.set_service_name(service_name);
    rpcHeader.set_method_name(method_name);
    rpcHeader.set_args_len(request_str_size);

    uint32_t rpc_header_str_size = 0;
    std::string rpc_header_str;
    if(rpcHeader.SerializeToString(&rpc_header_str)==0){
        std::cout<<"serialize rpcheader error!"<<std::endl;
        return;
    }
    rpc_header_str_size = rpc_header_str.size();

    //组织待发送的rpc请求的字符串
    std::string send_rpc_str;
    send_rpc_str+=std::string((char*)&rpc_header_str_size,4);//不能把字符串的数字存进去，比如 111存成"111",应该存储111的32位（4字节）数据
    send_rpc_str+=rpc_header_str;
    send_rpc_str+=request_str;

        //打印调试信息
    std::cout<<"===================================="<<std::endl;
    std::cout<<"header_size: "<<rpc_header_str_size<<std::endl;
    std::cout<<"rpc_header_str:"<<rpc_header_str<<std::endl;
    std::cout<<"service_name:"<<service_name<<std::endl;
    std::cout<<"method_name"<<method_name<<std::endl;
    std::cout<<"args_str:"<<request_str<<std::endl;
    std::cout<<"===================================="<<std::endl;

    //使用TPC编程，完成rpc方法的远程调用并获取回复
    int client_fd = socket(AF_INET,SOCK_STREAM,0);
    if(client_fd==-1){
        std::cout<<"create socket errno :"<<errno<<std::endl;
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    std::string ip = MprpcApplication::GetInstance().GetConfig().Load("rpcserverip");
    uint16_t port = atoi(MprpcApplication::GetInstance().GetConfig().Load("rpcserverport").c_str());
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET,ip.c_str(),&server_addr.sin_addr);

    //连接rpc服务节点
    if(connect(client_fd,(sockaddr*)&server_addr,sizeof(sockaddr))==-1){
        std::cout<<"connect errno :"<<errno<<std::endl;
        close(client_fd);
        exit(EXIT_FAILURE);
    }
    //发送rpc请求
    if(send(client_fd,send_rpc_str.c_str(),send_rpc_str.length(),0)==-1){
        std::cout<<"send msg errno :"<<errno<<std::endl;
        close(client_fd);
        return;
    }
    //接收rpc请求的响应
    char buf[1024];
    int recv_size=0;
    if((recv_size=recv(client_fd,buf,sizeof(buf),0))==-1){
        std::cout<<"recv msg errno :"<<errno<<std::endl;
        close(client_fd);
        return;
    }
    //解析回复
    if(!response->ParseFromArray(buf,recv_size)){
        std::cout<<"response parse error!"<<std::endl;
    }

    close(client_fd);
}

                          