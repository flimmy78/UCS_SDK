#-------------------------------------------------
#
# Project created by QtCreator 2018-01-02T09:26:50
#
#-------------------------------------------------

#表示这个是Qt跨目录,由多个子项目(如1个应用程序+多个共享库)组成的大项目
TEMPLATE = subdirs

#大项目包含的各个子项目,其中 UCSDemo 是应用程序, UCSTcpSDK UCSIMLib 是共享库
SUBDIRS += 3rdparty
SUBDIRS += UCSTcpSDK \
        UCSIMLib \
        UCSDemo

#CONFIG选项要求各个子项目按顺序编译，子目录的编译顺序在SUBDIRS中指明
CONFIG += ordered
