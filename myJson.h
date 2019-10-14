#ifndef MYJSON_H
#define MYJSON_H

#include <stdio.h>  

#include "myJsonPack.h"

/*设定JSON缓存区大小*/
#define MYJSON_ARRAYY_SIZE (4096)
#define MYJSON_BUFFER_SIZE (4096)

//允许UBUNTU调试
#define UBUNTU_TEST	0

#if UBUNTU_TEST
extern char myJsonArray[MYJSON_ARRAYY_SIZE];
#endif

/*数据类型*/
#define VAL_TYPE 0
#define STR_TYPE 1

/*return state*/
#define MYJSON_OK					0//操作成功
#define MYJSON_ERR				1//操作异常
#define INPUT_NULL				2//输入为空
#define SIZE_LIMIT				3//传入数据超出设定缓存大小

/************************************JSON基础功能**************************************/
/*
功能:创建一个JSON的空结构，会清除之前的JSON缓存数据
*/
void myJson_creatObject(void);

/*
功能:向JSON缓存里面增加一个字符信息
*/
char myJson_addString(char *name,char *str);

/*
功能:向JSON缓存里面增加一个整型数值
*/
char myJson_addInt(char *name,int val);

/*
功能:向JSON缓存里面增加整型数组
*/
char myJson_addArray(char *name,int *val,int len);

/*
功能:JSON 校验函数
*/
char myJson_check(char *str);

/*
功能:JSON 键值对比函数
*/
char myJson_checkValue(char *str1, char *str2);

/*
功能:JSON 获取字符串函数
*/
char myJson_getStr(char *json,char *json_key , char *json_value);

/*
功能:JSON 获取数值函数
*/
char myJson_getValue(char *json,char *json_key,int *val);

/*
功能:JSON 获取数组数据
*/
char myJson_getArray(char *json,char *json_key,int *val,int len);

/*
功能:输出Json数组
*/
void myJson_returnJsonArray(char *buff);

/************************************JSON扩展功能**************************************/
/*
功能:JSON 创建一个带节点的数组对象
*/
void myJson_creatArrayChild(char *name);

/*
功能:JSON 返回主节点
输入:无
输出:无
*/
void myJson_returnArrayChild(void);

/*
功能:JSON 增加数值到子节点
*/
char myJson_addValToChild(int val);

/*
功能:JSON 增加字符串到子节点
*/
char myJson_addStrToChild(char *str);

/*
功能:JSON 增加字符对象到子节点
*/
char myJson_addStrTargetToChild(char *name,char *str);

/*
功能:JSON 增加数值对象到子节点
*/
char myJson_addValTargetToChild(char *name,int val);

/*
功能:JSON 增加数组对象到子节点
*/
char myJson_addArrayTargetToChild(char *name,int *val,int len);

#endif


