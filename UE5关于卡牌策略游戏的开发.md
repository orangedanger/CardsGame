# UE5关于卡牌策略游戏的开发

## 0.git的设置和项目设置

### 0.1创建git仓库

[github](https://github.com/)中新建repository
输入仓库名即可创建git

需要.gitignore文件

```c++
Binaries
DerivedDataCache
Intermediate
Saved
Build

.vscode
.vs
*.VC.db
*.opensdf
*.opendb
*.sdf
*.sln
*.suo
*.xcodeproj
*.xcworkspace
```



在git中新建git后需要在对于文件中打开GitBash

```c++
git init
git status
git add .
git status
git commit -m "这是一个提交的名字"
git branch -M main
复制你网站上的链接
如:
git remote add origin git@github.com:orangedanger/Aura.git
git push -u origin main
```

如果在修改后进行提交

```c++
git add .
git status
git commit -m "这是一个提交的名字"
git push -u orgin main
```



关于项目设置

### 0.2.设置为Idea启动项目

在编译器偏好中设置编译器为Idea uproject



打开项目文件夹对项目文件右键选择打开方式为idea即可

## 1.设置摄像头和鼠标选定角色进行移动



## 2.材质制作 



## 3.棋盘地形和棋子

### 3.1.棋盘

#### 3.1.1.棋盘网格制作

棋盘是一个Actor，

#### 3.1.2.棋盘对齐


AddInstance需要的是WorldSpace

#### 3.1.3.检测环境

## 4.UI界面

