#!/bin/bash

set -e

# 获取当前脚本所在的目录
PWD=$(pwd)

# 清理 build 目录
rm -rf "${PWD}/build/*"

# 进入 build 目录并执行构建命令
cd "${PWD}/build" && {
    cmake ..
    make
}

# 返回上一级目录
cd ..

# 复制 include 目录到 lib 目录
cp -r "${PWD}/src/include" "${PWD}/lib"