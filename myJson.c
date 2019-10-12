/*����ͷ�ļ�*/
#include "myJson.h"

/*����һ�����飬���JSON���ݵ�ʱ�򣬻�ʹ�õ�����ռ�*/
#if UBUNTU_TEST
char myJsonArray[MYJSON_ARRAYY_SIZE];
char myJsonChild[MYJSON_ARRAYY_SIZE];
char myJsonBuffer[MYJSON_BUFFER_SIZE];
char myJsonCache[MYJSON_BUFFER_SIZE];
#else
AT_NONCACHEABLE_SECTION_ALIGN(char myJsonArray[MYJSON_ARRAYY_SIZE], FRAME_BUFFER_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(char myJsonChild[MYJSON_ARRAYY_SIZE], FRAME_BUFFER_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(char myJsonBuffer[MYJSON_BUFFER_SIZE], FRAME_BUFFER_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(char myJsonCache[MYJSON_BUFFER_SIZE], FRAME_BUFFER_ALIGN);
#endif

/*������������¼��дλ��*/
int myJsonWpt = 0;
int myJsonChildWpt = 0;
int myJsonChildFlag = 0;

/*�����������*/
static void cleanBuffer(void)
{
	int i=0;
	for(i=0;i<MYJSON_BUFFER_SIZE;i++)
	{
		myJsonBuffer[i] = 0;
	}
}

/* ʵ��itoa������Դ���� */
static char *myJson_itoa(int num, char *str, int radix)
{
	/* ������ */ 
	char index[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"; 
	unsigned unum; /* �м���� */ 
	int i=0,j,k; 
	/* ȷ��unum��ֵ */ 
	if(radix==10&&num<0) /* ʮ���Ƹ��� */ 
	{ 
		unum=(unsigned)-num; 
		str[i++]='-'; 
	} 
	else unum=(unsigned)num; /* ������� */ 
	/* ���� */ 
	do  
	{ 
		str[i++]=index[unum%(unsigned)radix]; 
		unum/=radix; 
	}while(unum); 
	str[i]='\0'; 

	/* ת�� */ 
	if(str[0]=='-') k=1; /* ʮ���Ƹ��� */ 
	else k=0; 
	/* ��ԭ���ġ�/2����Ϊ��/2.0������֤��num��16~255֮�䣬radix����16ʱ��Ҳ�ܵõ���ȷ��� */ 
	char temp;
	for(j=k;j<(i)/2.0;j++)
	{
		temp=str[j]; 
		str[j]=str[i-j-1+k]; 
		str[i-j-1+k]=temp; 
	}

	return str; 
} 

/*�ַ���ת��ֵ*/
int myJson_atol(char *str)//�ַ���ת���� 
{
	char flag = '+';//ָʾ����Ƿ������ 
	long res = 0;
	
	if(*str=='-')//�ַ��������� 
	{
		++str;//ָ����һ���ַ� 
		flag = '-';//����־��Ϊ���� 
	} 
	//����ַ�ת�������ۼӵ����res 
	while(*str>=48 && *str<=57)//��������ֲŽ���ת��������0~9��ASCII�룺48~57 
	{
		res = 10*res+  *str++-48;//�ַ�'0'��ASCII��Ϊ48,48-48=0�պ�ת��Ϊ����0 
	} 
 
    if(flag == '-')//�����Ǹ��������
	{
		res = -res;
	}
 
	return (int)res;
}

/*�ַ�������*/
char *myJson_insert(char *s1, char *s2, int n)
{
	int len1 = 0, len2 = 0, j = 0, k = 0;
	int i;
	if (s1 == NULL)
		return NULL;
	if (s2 == NULL)
		return s1;
	len1 = strlen(s1);
	len2 = strlen(s2);

	if(n>len1)
		return NULL;
	for (i = 0; i<n; i++) 
	{ 
		j++; 
	}
	for (i= 0; i<len1; i++)
	{ 
		myJsonCache[k++] = s1[i];
	}
 
	for (i = 0; i<len2; i++)
		s1[j++] = s2[i];

	for(i=n;i<len1;i++)
		s1[j++]=myJsonCache[i];
 
	s1[j] = '\0';
	
	return s1;
}

/* ��ȡ�ַ������� */
static int myJson_strlen(char *string)
{
    int len = 0;
    while (*string != '\0') {
        string++;
        len++;
    }
    return len;
}

/* �ַ�ƥ�� */
static char *myJson_strstr(char *str, char *sub_str)
{
	int i = 0;
    for(i = 0; str[i] != '\0'; i++)
    {
        int tem = i; //tem���������е���ʼ�ж��±�λ��
        int j = 0;
        while(str[i++] == sub_str[j++])
        {
            if(sub_str[j] == '\0')
            {
                return &str[tem];
            }
        }
        i = tem;
    }
    
    return "";
}

/*�ַ�����*/
static char* myJson_strncpy(char* dest, char* src, int len)
{
    
    char* temp=dest;
    int i=0;
    while(i++ < len  && (*temp++ = *src++)!='\0')
    {}
    if(*(temp)!='\0')     *temp='\0';
    return dest;
}

static int myJson_strcmp(char* str1, char* str2)
{
   while(*str1 && *str2 && *str1==*str2)
   {
			++str1;
			++str2;
   }
   return *str1-*str2;
}

/*
����:������������
����:��
���:��
*/
static char myJson_checkInputPack(char *str)
{
	int length = 0;

	/*can't input NULL data*/
	if(str == NULL || str == NULL)
	{
		return INPUT_NULL;
	}	
	
	//�жϴ���������û�г�����Χ
	length = strlen(str) + 10;
	if((myJsonWpt + length )>= MYJSON_ARRAYY_SIZE)
	{
		return SIZE_LIMIT;
	}
	
	/*�жϵ�ǰ���λ���ǲ��ǵ�һ������*/
	if(myJsonWpt > 3)
	{
		/*�´������*/
		myJsonArray[myJsonWpt++] = ',';
	}
	
	return MYJSON_OK;
}

/*
����:������������
����:��
���:��
*/
static void myJson_checkInputChildPack(void)
{
	/*�жϵ�ǰ���λ���ǲ��ǵ�һ������*/
	if(!myJsonChildFlag)
	{
		/*�´������*/
		myJsonChild[myJsonChildWpt++] = ',';
	}
	else myJsonChildFlag = 0;
}

/************************************JSON��������**************************************/
/*
����:����һ��JSON�Ŀսṹ�������֮ǰ��JSON��������
����:��
���:��
*/
void myJson_creatObject(void)
{
	int i=0;
	
	/*clean buff*/
	for(i=0;i<MYJSON_ARRAYY_SIZE;i++)
	{
		myJsonArray[i] = 0;
	}

	myJsonWpt = 0;
	/*add json data in array*/
	strcat(&myJsonArray[myJsonWpt],"{}");

	/*the Wpt will be used,when add new data*/
	myJsonWpt = 1;
}

/*
����:��JSON������������һ���ַ���Ϣ
����:�ֶ����� �ֶ�����
���������ɹ�����MYJSON_OK ʧ�ܷ���MYJSON_ERR
*/
char myJson_addString(char *name,char *str)
{
	int length = 0;
    int wpt=0;
    int i=0;
	
	//�����������
	cleanBuffer();

	/*�����ֶ�����*/
	myJson_checkInputPack(name);
	myJsonBuffer[wpt++] = '\"';
    length = myJson_strlen(name);
    for(i=0;i<length;i++)
    {
        myJsonBuffer[1+i] = name[i];
    }
    wpt+=length;
    myJsonBuffer[wpt++] = '\"';
    myJsonBuffer[wpt++] = ':';
    myJsonBuffer[wpt++] = '\"';

	/*�����ֶ�����*/
    length = myJson_strlen(str);
    for(i=0;i<length;i++)
    {
        myJsonBuffer[i+wpt] = str[i];
    }
    wpt+=length;
    myJsonBuffer[wpt++] = '\"';
    
    /*�������λ��*/
    myJson_insert(myJsonArray,myJsonBuffer,myJsonWpt);
    myJsonWpt += wpt;
    myJsonArray[myJsonWpt] = '}';

	return MYJSON_OK;
}

/*
����:��JSON������������һ��������ֵ
����:�ֶ����� �ֶ�����
���������ɹ�����MYJSON_OK ʧ�ܷ���MYJSON_ERR
*/
char myJson_addInt(char *name,int val)
{
	int length = 0;
	int wpt=0;
    int i=0;
	
	//�����������
	cleanBuffer();
	
	/*�����ֶ�����*/
	myJson_checkInputPack(name);
	myJsonBuffer[wpt++] = '\"';
    length = myJson_strlen(name);
    for(i=0;i<length;i++)
    {
        myJsonBuffer[1+i] = name[i];
    }
    wpt+=length;
    myJsonBuffer[wpt++] = '\"';
    myJsonBuffer[wpt++] = ':';

	/*����������ֵ*/
	myJson_itoa(val,&myJsonBuffer[wpt],10);
	wpt = myJson_strlen(myJsonBuffer);
	
	/*�������λ��*/
    myJson_insert(myJsonArray,myJsonBuffer,myJsonWpt);
    myJsonWpt += wpt;
    myJsonArray[myJsonWpt] = '}';

	return MYJSON_OK;
}

/*
����:��JSON��������������������
����:�ֶ����� �ֶ����� ���鳤��
���������ɹ�����MYJSON_OK ʧ�ܷ���MYJSON_ERR
*/
char myJson_addArray(char *name,int *val,int len)
{
	int length = 0;
	int wpt=0;
    int i=0;
	
	//�����������
	cleanBuffer();
	
	/*�����ֶ�����*/
	myJson_checkInputPack(name);
	myJsonBuffer[wpt++] = '\"';
    length = myJson_strlen(name);
    for(i=0;i<length;i++)
    {
        myJsonBuffer[1+i] = name[i];
    }
    wpt+=length;
    myJsonBuffer[wpt++] = '\"';
    myJsonBuffer[wpt++] = ':';
    myJsonBuffer[wpt++] = '[';
	
	for(i=0;i<len;i++)
	{
		if(i!=0)
		{
			myJsonBuffer[wpt++] = ',';
		}
		/*����������ֵ*/
		myJson_itoa(val[i],&myJsonBuffer[wpt],10);
		wpt = myJson_strlen(myJsonBuffer);
	}
	/*�������λ��*/
	myJsonBuffer[wpt++] = ']';
    myJson_insert(myJsonArray,myJsonBuffer,myJsonWpt);
    myJsonWpt += wpt;
    myJsonArray[myJsonWpt] = '}';

	return MYJSON_OK;
}
 
/*
����:JSON У�麯��
����:�ַ���
���:�Ϸ�JAON ����MYJSON_OK ���Ϸ�JSON ����MYJSON_ERR
*/
char myJson_check(char *str)
{
    int str_length; //�����ַ����ȱ���
    str_length = myJson_strlen(str); //�����ַ�����
    if(str[0] == '{' && str[str_length-1] == '}') //ͨ����β�������ж��Ƿ�ΪJSON
    {
        return MYJSON_OK; //����ַ���Ϊ�Ϸ�JSON
    }
    else
    {
        return MYJSON_ERR; //����ַ���Ϊ�Ϸ�JSON
    }
}
 
/*
����:JSON ��ֵ�ԱȺ���
����:JSON ��ֵ Ҫƥ����ַ�
���:�����ȡ�ɹ�����MYJSON_OK ��ȡʧ�ܷ���MYJSON_ERR
*/
char myJson_checkValue(char *str1, char *str2)
{
    if(myJson_strcmp(str1,str2) == 0)
    {
        return MYJSON_OK; //ƥ��ɹ�
    }
    else
    {
        return MYJSON_ERR;//ƥ��ɹ�
    }
}

/*
����:JSON ��ȡ��������
����:JSON�ַ��� Ҫ��ȡ�ļ��� ��ȡ��ֵ���ַ��� ��ֵ  ��������
���:�����ȡ�ɹ�����MYJSON_OK ��ȡʧ�ܷ���MYJSON_ERR
*/
static char myJson_getData(char *json,char *json_key , char *json_value,int *val, char type)
{
	char *json_key_start; //���������ʼ��λ��
    char *json_key_end; //�������������λ��
    char json_key_length; //�����������
    
    char *json_value_start; //�����ֵ��ʼ��λ��
    char *json_value_end; //�����ֵ������λ��
    char json_value_length; //�����ֵ����
    
    json_key_start = myJson_strstr(json,json_key); //��ȡ������ʼ��λ��
    json_key_length = myJson_strlen(json_key); //��ȡ�����ĳ���
    json_key_end = json_key_start + json_key_length;  //��ȡ����������λ��
    
    if(json_key_start != 0 && *(json_key_start - 1) == '\"' && *(json_key_end) == '\"' && *(json_key_end + 1) == ':')
    {
        json_value_start = json_key_end + 2 + type; //��ȡ��ֵ��ʼ��λ��
				json_value_end = (type == STR_TYPE)?myJson_strstr(json_value_start,"\""):myJson_strstr(json_value_start,",");
        json_value_length = json_value_end - json_value_start; //��ȡ��ֵ�ĳ���
        
		if(type == STR_TYPE)
		{
			myJson_strncpy(json_value,json_value_start,json_value_length); //����ֵ����ָ������
			json_value[json_value_length] = '\0'; //ָ�����һλ���ַ��������� \0
		}
		else
		{
			cleanBuffer();
			myJson_strncpy(myJsonBuffer,json_value_start,json_value_length); //����ֵ����ָ������
			*val = myJson_atol(myJsonBuffer);
		}
        
        return MYJSON_OK; //�ɹ���ȡ��ֵ
    }
    else
    {
		if(type == STR_TYPE)
			json_value[0] = '\0';
        return MYJSON_ERR; //ʧ�ܻ�ȡ��ֵ
    }
}

/*
����:JSON ��ȡ�ַ�������
����:JSON�ַ��� Ҫ��ȡ�ļ��� ��ȡ��ֵ���ַ���
���:�����ȡ�ɹ�����MYJSON_OK ��ȡʧ�ܷ���MYJSON_ERR
*/
char myJson_getStr(char *json,char *json_key , char *json_value)
{
    return myJson_getData(json,json_key,json_value,NULL,STR_TYPE);
}

/*
����:JSON ��ȡ��ֵ����
����:JSON�ַ��� Ҫ��ȡ�ļ��� ��ֵ
���:�����ȡ�ɹ�����MYJSON_OK ��ȡʧ�ܷ���MYJSON_ERR
*/
char myJson_getValue(char *json,char *json_key,int *val)
{
	return myJson_getData(json,json_key,NULL,val,VAL_TYPE);
}
 
/*
����:JSON ��ȡ��������
����:JSON�ַ��� Ҫ��ȡ�ļ��� ��ֵ�������� ���ݳ���
���:�����ȡ�ɹ�����MYJSON_OK ��ȡʧ�ܷ���MYJSON_ERR
*/
char myJson_getArray(char *json,char *json_key,int *val,int len)
{
	char *json_key_start; //���������ʼ��λ��
    char *json_key_end; //�������������λ��
    char json_key_length; //�����������
    
    char *json_value_start; //�����ֵ��ʼ��λ��
    char *json_value_end; //�����ֵ������λ��
    char json_value_length; //�����ֵ����
    
    json_key_start = myJson_strstr(json,json_key); //��ȡ������ʼ��λ��
    json_key_length = myJson_strlen(json_key); //��ȡ�����ĳ���
    json_key_end = json_key_start + json_key_length;  //��ȡ����������λ��
    
    if(json_key_start != 0 && *(json_key_start - 1) == '\"' && *(json_key_end) == '\"' && *(json_key_end + 1) == ':')
    {
        json_value_start = json_key_end + 3; //��ȡ��ֵ��ʼ��λ��

		int i=0;
		for(i=0;i< len;i++)
		{
			json_value_end = myJson_strstr(json_value_start,",");
			json_value_length = json_value_end - json_value_start; //��ȡ��ֵ�ĳ���
		
			cleanBuffer();
			myJson_strncpy(myJsonBuffer,json_value_start,json_value_length); //����ֵ����ָ������
			val[i] = myJson_atol(myJsonBuffer);
			json_value_start = json_value_end + 1;//�ƶ����ݿ�ʼλ��
		}
        
        return MYJSON_OK; //�ɹ���ȡ��ֵ
    }
    else
    {
        return MYJSON_ERR; //ʧ�ܻ�ȡ��ֵ
    }
}

/*
����:���Json����
����:��
���:Json�������������
*/
void myJson_returnJsonArray(char *buff)
{
	int i=0;
	
	while(myJsonArray[i])
	{
		buff[i] = myJsonArray[i];
		i++;
	}
}
 
/************************************JSON��չ����**************************************/
/*
����:JSON ����һ�����ڵ���������
����:�ڵ�����
���:�����ȡ�ɹ�����MYJSON_OK ��ȡʧ�ܷ���MYJSON_ERR
*/
void myJson_creatArrayChild(char *name)
{
	int i=0;
	int length = 0;
	
	/*clean buff*/
	for(i=0;i<MYJSON_ARRAYY_SIZE>>2;i++)
	{
		myJsonChild[i] = 0;
	}

	myJsonChildWpt = 0;

	/*add json data in array*/
	strcat(&myJsonChild[myJsonChildWpt++],"\"");
	length = strlen(name);
	strcat(&myJsonChild[myJsonChildWpt],name);
	myJsonChildWpt += length;
	strcat(&myJsonChild[myJsonChildWpt],"\":[]");
	myJsonChildWpt += 3;
	myJsonChildFlag = 1;
}

/*
����:JSON �������ڵ�
����:��
���:��
*/
void myJson_returnArrayChild(void)
{
	/*�жϵ�ǰ���λ���ǲ��ǵ�һ������*/
	if(myJsonWpt > 3)
	{
		/*�´������*/
		myJsonArray[myJsonWpt++] = ',';
	}
	
	strcat(&myJsonArray[myJsonWpt],myJsonChild);
	myJsonWpt += myJsonChildWpt;
	strcat(&myJsonArray[myJsonWpt++],"]}");
}

/*
����:JSON ������ֵ���ӽڵ�
����:������ֵ
���:�����ȡ�ɹ�����MYJSON_OK ��ȡʧ�ܷ���MYJSON_ERR
*/
char myJson_addValToChild(int val)
{
	int length = 0;

	myJson_checkInputChildPack();
	
	/*������ֵ����*/
	cleanBuffer();
	myJson_itoa(val,myJsonBuffer,10);
	length = myJson_strlen(myJsonBuffer);
	myJson_insert(myJsonChild,myJsonBuffer,myJsonChildWpt);
	myJsonChildWpt+=length;
	
	return MYJSON_OK;
}

/*
����:JSON �����ַ������ӽڵ�
����:�ַ���
���:�����ȡ�ɹ�����MYJSON_OK ��ȡʧ�ܷ���MYJSON_ERR
*/
char myJson_addStrToChild(char *str)
{
	int length = 0;

	myJson_checkInputChildPack();
	
	/*�����ַ�������*/
	length = myJson_strlen(str);
	myJsonChild[myJsonChildWpt++] = '\"';
	myJson_insert(myJsonChild,str,myJsonChildWpt);
	myJsonChildWpt+=length;
	myJsonChild[myJsonChildWpt++] = '\"';
	return MYJSON_OK;
}

/*
����:JSON �����ַ������ӽڵ�
����:�ڵ�����
���:�����ȡ�ɹ�����MYJSON_OK ��ȡʧ�ܷ���MYJSON_ERR
*/
char myJson_addStrTargetToChild(char *name,char *str)
{
	int length = 0;
	myJson_checkInputChildPack();
	
	/*��������*/
	length = myJson_strlen(name);
	myJson_insert(myJsonChild,"{\"",myJsonChildWpt);
	myJsonChildWpt+=2;
	myJson_insert(myJsonChild,name,myJsonChildWpt);
	myJsonChildWpt+=length;
	myJson_insert(myJsonChild,"\":",myJsonChildWpt);
	myJsonChildWpt+=2;
	
	/*�����ַ�����*/
	length = myJson_strlen(str);
	myJsonChild[myJsonChildWpt++] = '\"';
	myJson_insert(myJsonChild,str,myJsonChildWpt);
	myJsonChildWpt+=length;
	myJson_insert(myJsonChild,"\"}",myJsonChildWpt);
	myJsonChildWpt+=2;
	
	return MYJSON_OK;
}

/*
����:JSON ������ֵ�����ӽڵ�
����:�ڵ�����
���:�����ȡ�ɹ�����MYJSON_OK ��ȡʧ�ܷ���MYJSON_ERR
*/
char myJson_addValTargetToChild(char *name,int val)
{
	int length = 0;
	myJson_checkInputChildPack();
	
	/*��������*/
	length = myJson_strlen(name);
	myJson_insert(myJsonChild,"{\"",myJsonChildWpt);
	myJsonChildWpt+=2;
	myJson_insert(myJsonChild,name,myJsonChildWpt);
	myJsonChildWpt+=length;
	myJson_insert(myJsonChild,"\":",myJsonChildWpt);
	myJsonChildWpt+=2;
	
	/*������ֵ����*/
	cleanBuffer();
	myJson_itoa(val,myJsonBuffer,10);
	length = myJson_strlen(myJsonBuffer);
	myJson_insert(myJsonChild,myJsonBuffer,myJsonChildWpt);
	myJsonChildWpt+=length;
	myJson_insert(myJsonChild,"}",myJsonChildWpt++);
	
	return MYJSON_OK;
}

/*
����:JSON ������������ӽڵ�
����:�ڵ�����
���:�����ȡ�ɹ�����MYJSON_OK ��ȡʧ�ܷ���MYJSON_ERR
*/
char myJson_addArrayTargetToChild(char *name,int *val,int len)
{
	int length = 0;
	myJson_checkInputChildPack();
	
	/*��������*/
	length = myJson_strlen(name);
	myJson_insert(myJsonChild,"{\"",myJsonChildWpt);
	myJsonChildWpt+=2;
	myJson_insert(myJsonChild,name,myJsonChildWpt);
	myJsonChildWpt+=length;
	myJson_insert(myJsonChild,"\":[",myJsonChildWpt);
	myJsonChildWpt+=3;
	
	int i=0;
	for(i=0;i<len;i++)
	{
		/*������ֵ����*/
		cleanBuffer();
		myJson_itoa(val[i],myJsonBuffer,10);
		length = myJson_strlen(myJsonBuffer);
		myJson_insert(myJsonChild,myJsonBuffer,myJsonChildWpt);
		myJsonChildWpt+=length;
		if(i != (len - 1))
			myJson_insert(myJsonChild,",",myJsonChildWpt++);
	}
	
	myJson_insert(myJsonChild,"]}",myJsonChildWpt);
	myJsonChildWpt+=2;
	
	return MYJSON_OK;
}


