#ifndef MYJSON_H
#define MYJSON_H

#include <stdio.h>  

#include "myJsonPack.h"

/*�趨JSON��������С*/
#define MYJSON_ARRAYY_SIZE (4096)
#define MYJSON_BUFFER_SIZE (4096)

//����UBUNTU����
#define UBUNTU_TEST	0

#if UBUNTU_TEST
extern char myJsonArray[MYJSON_ARRAYY_SIZE];
#endif

/*��������*/
#define VAL_TYPE 0
#define STR_TYPE 1

/*return state*/
#define MYJSON_OK					0//�����ɹ�
#define MYJSON_ERR				1//�����쳣
#define INPUT_NULL				2//����Ϊ��
#define SIZE_LIMIT				3//�������ݳ����趨�����С

/************************************JSON��������**************************************/
/*
����:����һ��JSON�Ŀսṹ�������֮ǰ��JSON��������
*/
void myJson_creatObject(void);

/*
����:��JSON������������һ���ַ���Ϣ
*/
char myJson_addString(char *name,char *str);

/*
����:��JSON������������һ��������ֵ
*/
char myJson_addInt(char *name,int val);

/*
����:��JSON��������������������
*/
char myJson_addArray(char *name,int *val,int len);

/*
����:JSON У�麯��
*/
char myJson_check(char *str);

/*
����:JSON ��ֵ�ԱȺ���
*/
char myJson_checkValue(char *str1, char *str2);

/*
����:JSON ��ȡ�ַ�������
*/
char myJson_getStr(char *json,char *json_key , char *json_value);

/*
����:JSON ��ȡ��ֵ����
*/
char myJson_getValue(char *json,char *json_key,int *val);

/*
����:JSON ��ȡ��������
*/
char myJson_getArray(char *json,char *json_key,int *val,int len);

/*
����:���Json����
*/
void myJson_returnJsonArray(char *buff);

/************************************JSON��չ����**************************************/
/*
����:JSON ����һ�����ڵ���������
*/
void myJson_creatArrayChild(char *name);

/*
����:JSON �������ڵ�
����:��
���:��
*/
void myJson_returnArrayChild(void);

/*
����:JSON ������ֵ���ӽڵ�
*/
char myJson_addValToChild(int val);

/*
����:JSON �����ַ������ӽڵ�
*/
char myJson_addStrToChild(char *str);

/*
����:JSON �����ַ������ӽڵ�
*/
char myJson_addStrTargetToChild(char *name,char *str);

/*
����:JSON ������ֵ�����ӽڵ�
*/
char myJson_addValTargetToChild(char *name,int val);

/*
����:JSON ������������ӽڵ�
*/
char myJson_addArrayTargetToChild(char *name,int *val,int len);

#endif


