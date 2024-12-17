#include "mprpcapplication.h"

void MprpcApplication::Init(int argc, char **argv)
{
}

MprpcApplication &MprpcApplication::GetInstance()
{
    static MprpcApplication ins;
    return ins;
}

MprpcApplication::MprpcApplication()
{
}
