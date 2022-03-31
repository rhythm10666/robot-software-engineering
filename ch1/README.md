# ch1: Display Image

项目包含：
- Source code: DisplayImage.cpp
- Sample image: lena.png
- CMakeLists.txt

## 代码编译
通过编写 CMakelist.txt 文件，自动生成 makefile：
```bash
cmake .
```
make 用来批处理.c 或.cpp 文件，它从 makefile 中读取相应指令，然后编译：
```bash
make
```

## 上传gitee
```bash
# 设置config
git config --global user.name Padparadscha
git config --global user.email rhythm10666@126.com

# 设置密钥
ssh-keygen -t rsa -C "rhythm10666@126.com"
cd ~/.ssh
gedit id_rsa.pub
cd
ssh -T git@gitee.com

# push到远程仓库
cd RSE2022/
git init
git remote add origin git@gitee.com:PadparadschaCorundum/rse.git
git add . # 将指定目录或文件添加到暂存区
git commit -m "ch1" # 将暂存区内容添加到仓库中
git push origin master
git push -f origin master # 强制推送
```
