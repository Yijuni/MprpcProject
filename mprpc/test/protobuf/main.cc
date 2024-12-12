#include "test.pb.h"
#include <iostream>

using namespace fixbug;

int main(){

    LoginResponse loginrsp;
    //如果一个message里的成员包含另一个message可以通过以下方法修改内部message里的值
    ResultCode *resc = loginrsp.mutable_result();//返回一个ResultCode指针，可以通过该指针修改ResultCode里的值
    resc->set_errorcode(1);
    resc->set_errormsg("登录处理失败");
    std::string returnstr;
    loginrsp.SerializeToString(&returnstr);
    std::cout<<returnstr<<std::endl;

    GetFrindListsResponse rsp;
    ResultCode *resc1 = rsp.mutable_result();
    resc1->set_errorcode(0);
    
    //往包含User的列表里添加新的数据
    User *user = rsp.add_friend_list();//他会生成一个User对象，并把生成的User对象返回回来，然后再修改User对象即可
    user->set_name("张三");
    user->set_age(20);
    user->set_sex(User::MAN);

    User *user1 = rsp.add_friend_list();//他会生成一个User对象，并把生成的User对象返回回来，然后再修改User对象即可
    user1->set_name("张三");
    user1->set_age(20);
    user1->set_sex(User::MAN);

    std::cout<<"好友数量:"<<rsp.friend_list_size()<<std::endl;
    User user2 = rsp.friend_list(1);//获取下标为1的用户
    std::cout<<user->age()<<std::endl;

    rsp.SerializeToString(&returnstr);
    std::cout<<returnstr<<std::endl;

    return 0;
}

//没打包之前的代码写法，也就是没把ResultCode里的错误码和错误信息单独写一个message时写的代码
int main1(){
    //proto文件定义 message LoginRequest,执行protoc test.proto --cpp_out=./会自动生成头文件里面就包含类LoginRequest
    LoginRequest req;
    req.set_name("zhang san");
    req.set_pwd("123456");

    //LoginRequst对象序列化到send_str
    std::string send_str;
    if(req.SerializeToString(&send_str)){
        std::cout<<send_str.c_str()<<std::endl;
    }

    //send_str反序列化为一个LoginRequest对象
    LoginRequest reqB;
    if(reqB.ParseFromString(send_str)){
        std::cout<<reqB.name()<<std::endl;
        std::cout<<reqB.pwd()<<std::endl;
    }
    
    return 0;
}