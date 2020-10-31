FatFs Module Source Files R0.11


FILES

  00readme.txt This file.
  history.txt  Revision history.

  ffconf.h     Configuration file for FatFs module.
              (FatFs的配置文件，可以通过裁剪各种宏定义，达到对函数的配置(消减/增加部分功能来尽量好的利用资源) 文件支持中文/英文/日文等版本的修改)
  ff.h         Common include file for FatFs and application module.
               (include .h 文件)
  ff.c         FatFs module.
               (核心的FatFs文件系统管理的相关函数)

  diskio.h     Common include file for FatFs and disk I/O module.
              (include .h 文件)
  diskio.c     An example of glue function to attach existing disk I/O module to FatFs.
              (连接了FATFS和存储介质底层IO的一些函数，函数要自己实现)

  integer.h    Integer type definitions for FatFs.   
              (整合各种类型的定义， 把各种的变量类型重新定义一下，方便使用)
  
  option       Optional external functions.
              (外部的一些支持各自语言的文件  cc936支持中文  英文需要ccsbcs，syscall文件)


  Low level disk I/O module is not included in this archive because the FatFs
  module is only a generic file system layer and not depend on any specific
  storage device. You have to provide a low level disk I/O module that written
  to control the target storage device.

