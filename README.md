# LibraryManage

## 简介

XDU 20级程序设计大作业。

这是一个简单的拥有图形界面的图书管理系统。

## 特色

- 前后端完全分离
- 易于使用的交互界面
- 

# 功能介绍

主要有图书管理和用户管理两项功能。

默认读取同一目录下的 `user.csv` 和 `book.csv` 作为用户和图书数据文件。也可在登录后导入其他数据文件。
在无数据文件的情况下，默认打开时是空白表格，记得先创建个管理员账户再保存哦！

## 登录界面
<img src="https://user-images.githubusercontent.com/26119430/118392283-7247dc00-b66b-11eb-9d76-62ac68466e68.png" height=200px>

## 图书管理界面（管理员）
<img src="https://user-images.githubusercontent.com/26119430/118392294-81c72500-b66b-11eb-8742-db82785919ac.png" height=500px>

## 搜索结果（非管理员）
<img src="https://user-images.githubusercontent.com/26119430/118392902-90fba200-b66e-11eb-80e7-1db76811c06e.png" height=500px>

## 用户管理界面（管理员）
<img src="https://user-images.githubusercontent.com/26119430/118392322-b0dd9680-b66b-11eb-862b-6c9d8b30121d.png" height=500px>

## 图书详情界面（管理员）
<img src="https://user-images.githubusercontent.com/26119430/118392348-d8ccfa00-b66b-11eb-9042-fc63c4baa835.png" height=500px>

## 用户详情界面（管理员）
<img src="https://user-images.githubusercontent.com/26119430/118392358-eedaba80-b66b-11eb-9a95-5584c3901f69.png" height=500px>

## 管理员重设密码
<img src="https://user-images.githubusercontent.com/26119430/118392459-73c5d400-b66c-11eb-87e9-1e903f3418d5.png" height=400px>

## 用户修改密码
<img src="https://user-images.githubusercontent.com/26119430/118392504-a7a0f980-b66c-11eb-9e35-e7d6b1b34c6b.png" height=500px>

# 后端实现

## 图书链表与用户链表
底层数据结构是自己写的链表。一个图书馆有两个链表，分别存储用户和图书信息。

每个用户节点与图书节点内部还有两个链表，其中一个链表用于存储 借阅的图书节点指针 或 借阅该书的用户节点指针。另一个链表临时存储对应的 图书编号 和 用户编号（读取文件时存储，会在预处理之后清空）。

## csv 文件数据库
数据通过两个 csv 文件存储。

### 图书文件数据库
每一列的含义为：图书名称，图书编号，图书数量，借阅该书的用户编号
![image](https://user-images.githubusercontent.com/26119430/118393093-c8b71980-b66f-11eb-9e25-9c68c8fef1f6.png)

### 用户文件数据库
每一列的含义为：用户名，密码，编号，是否为管理员，借阅的图书编号
![image](https://user-images.githubusercontent.com/26119430/118393214-80e4c200-b670-11eb-83f8-7b7c4aac3c13.png)

