FatFs Module Source Files R0.11


FILES

  00readme.txt This file.
  history.txt  Revision history.

  ffconf.h     Configuration file for FatFs module.
              (FatFs�������ļ�������ͨ���ü����ֺ궨�壬�ﵽ�Ժ���������(����/���Ӳ��ֹ����������õ�������Դ) �ļ�֧������/Ӣ��/���ĵȰ汾���޸�)
  ff.h         Common include file for FatFs and application module.
               (include .h �ļ�)
  ff.c         FatFs module.
               (���ĵ�FatFs�ļ�ϵͳ�������غ���)

  diskio.h     Common include file for FatFs and disk I/O module.
              (include .h �ļ�)
  diskio.c     An example of glue function to attach existing disk I/O module to FatFs.
              (������FATFS�ʹ洢���ʵײ�IO��һЩ����������Ҫ�Լ�ʵ��)

  integer.h    Integer type definitions for FatFs.   
              (���ϸ������͵Ķ��壬 �Ѹ��ֵı����������¶���һ�£�����ʹ��)
  
  option       Optional external functions.
              (�ⲿ��һЩ֧�ָ������Ե��ļ�  cc936֧������  Ӣ����Ҫccsbcs��syscall�ļ�)


  Low level disk I/O module is not included in this archive because the FatFs
  module is only a generic file system layer and not depend on any specific
  storage device. You have to provide a low level disk I/O module that written
  to control the target storage device.

