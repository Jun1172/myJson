/*包含头文件*/
#include "myJson.h"

/*创建一个数组，打包JSON数据的时候，会使用到这个空间*/
char myJsonArray[MYJSON_ARRAYY_SIZE];
char myJsonBuffer[MYJSON_BUFFER_SIZE];

/*创建两个变量，记录读写位置*/
int myJsonWpt = 0;
int myJsonRpt = 0;

/*清除缓存数组*/
static void cleanBuffer(void)
{
	int i=0;
	for(i=0;i<MYJSON_BUFFER_SIZE;i++)
	{
		myJsonBuffer[i] = 0;
	}
}

/* 实现itoa函数的源代码 */
static char *myJson_itoa(int num, char *str, int radix)
{
	/* 索引表 */ 
	char index[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"; 
	unsigned unum; /* 中间变量 */ 
	int i=0,j,k; 
	/* 确定unum的值 */ 
	if(radix==10&&num<0) /* 十进制负数 */ 
	{ 
		unum=(unsigned)-num; 
		str[i++]='-'; 
	} 
	else unum=(unsigned)num; /* 其它情况 */ 
	/* 逆序 */ 
	do  
	{ 
		str[i++]=index[unum%(unsigned)radix]; 
		unum/=radix; 
	}while(unum); 
	str[i]='\0'; 

	/* 转换 */ 
	if(str[0]=='-') k=1; /* 十进制负数 */ 
	else k=0; 
	/* 将原来的“/2”改为“/2.0”，保证当num在16~255之间，radix等于16时，也能得到正确结果 */ 
	char temp;
	for(j=k;j<(i)/2.0;j++)
	{
		temp=str[j]; 
		str[j]=str[i-j-1+k]; 
		str[i-j-1+k]=temp; 
	}

	return str; 
} 

/*字符串转数值*/
int myJson_atol(char *str)//字符串转数字 
{
	char flag = '+';//指示结果是否带符号 
	long res = 0;
	
	if(*str=='-')//字符串带负号 
	{
		++str;//指向下一个字符 
		flag = '-';//将标志设为负号 
	} 
	//逐个字符转换，并累加到结果res 
	while(*str>=48 && *str<=57)//如果是数字才进行转换，数字0~9的ASCII码：48~57 
	{
		res = 10*res+  *str++-48;//字符'0'的ASCII码为48,48-48=0刚好转化为数字0 
	} 
 
    if(flag == '-')//处理是负数的情况
	{
		res = -res;
	}
 
	return (int)res;
}

/* 获取字符串长度 */
static int myJson_strlen(char *string)
{
    int len = 0;
    while (*string != '\0') {
        string++;
        len++;
    }
    return len;
}

/* 字符匹配 */
static char *myJson_strstr(char *str, char *sub_str)
{
    for(int i = 0; str[i] != '\0'; i++)
    {
        int tem = i; //tem保留主串中的起始判断下标位置
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

/*字符拷贝*/
static char* myJson_strncpy(char* dest, char* src, int len)
{
    
    char* temp=dest;
    int i=0;
    while(i++ < len  && (*temp++ = *src++)!='\0')
    {}
    if(*(temp)!='\0')     *temp='\0';
    return dest;
}

static int myJson_strcmp(char* str1, char* str2){
   while(*str1 && *str2 && *str1==*str2)
   {
	++str1;
	++str2;
   }
   return *str1-*str2;
}

/*
功能:检查输入的数据
输入:无
输出:无
*/
static char myJson_checkInputPack(char *str)
{
	int length = 0;

	/*can't input NULL data*/
	if(str == NULL || str == NULL)
	{
		return INPUT_NULL;
	}	
	
	//判断传入数据有没有超出范围
	length = strlen(str) + 10;
	if((myJsonWpt + length )>= MYJSON_ARRAYY_SIZE)
	{
		return SIZE_LIMIT;
	}
	
	/*判断当前添加位置是不是第一包数据*/
	if(myJsonWpt > 3)
	{
		/*新传入参数*/
		myJsonArray[myJsonWpt++] = ',';
	}
}

/************************************JSON基础功能**************************************/
/*
功能:创建一个JSON的空结构，会清除之前的JSON缓存数据
输入:无
输出:无
*/
void myJson_creatObject(void)
{
	int i=0;
	
	/*clean buff*/
	for(i=0;i<MYJSON_ARRAYY_SIZE;i++)
	{
		myJsonArray[i] = 0;
	}
	
	#if 0//UBUNTU_TEST
	printf("clean buff\r\n");
	#endif

	myJsonWpt = 0;
	myJsonRpt = 0;
	/*add json data in array*/
	strcat(&myJsonArray[myJsonWpt],"{}");

	/*the Wpt will be used,when add new data*/
	myJsonWpt = 1;
	
	#if 0//UBUNTU_TEST
	printf("Add new data:%s\r\n",myJsonArray);
	#endif
}

/*
功能:向JSON缓存里面增加一个字符信息
输入:字段名称 字段内容
输出：如果成功返回MYJSON_OK 失败返回MYJSON_ERR
*/
char myJson_addString(char *name,char *str)
{
	int length = 0;
	
	/*输入内容检查*/
	myJson_checkInputPack(name);
	
	/*传入字段名称*/
	length = strlen(name);
	myJsonArray[myJsonWpt++]	= '\"';
	strcat(&myJsonArray[myJsonWpt],name);
	myJsonWpt += length;
	strcat(&myJsonArray[myJsonWpt],"\":\"");
	myJsonWpt += 3;

	/*传入字段内容*/
	length = strlen(str);
	strcat(&myJsonArray[myJsonWpt],str);
	myJsonWpt += length;
	strcat(&myJsonArray[myJsonWpt++],"\"}");

	#if 0//UBUNTU_TEST
	printf("myJsonWpt:%d\r\n",myJsonWpt);
	printf("%s\r\n",myJsonArray);
	#endif
	return MYJSON_OK;
}

/*
功能:向JSON缓存里面增加一个整型数值
输入:字段名称 字段内容
输出：如果成功返回MYJSON_OK 失败返回MYJSON_ERR
*/
char myJson_addInt(char *name,int val)
{
	int length = 0;
	char intStr[10];
	
	/*输入内容检查*/
	myJson_checkInputPack(name);
	
	/*传入字段名称*/
	length = strlen(name);
	myJsonArray[myJsonWpt] = '\"';
	strcat(&myJsonArray[myJsonWpt],name);
	myJsonWpt += length;
	strcat(&myJsonArray[myJsonWpt],"\":");
	myJsonWpt += 2;

	/*传入数值内容*/
	myJson_itoa(val,intStr,10);
	length = strlen(intStr);
	strcat(&myJsonArray[myJsonWpt],intStr);
	myJsonWpt += length;
	strcat(&myJsonArray[myJsonWpt++],"}");

	#if 0//UBUNTU_TEST
	printf("myJsonWpt:%d\r\n",myJsonWpt);
	printf("%s\r\n",myJsonArray);
	#endif
	return MYJSON_OK;
}

/*
功能:向JSON缓存里面增加整型数组
输入:字段名称 字段内容 数组长度
输出：如果成功返回MYJSON_OK 失败返回MYJSON_ERR
*/
char myJson_addArray(char *name,int *val,int len)
{
	int length = 0;
	char intStr[10];
	
	/*输入内容检查*/
	myJson_checkInputPack(name);
	
	/*传入字段名称*/
	length = strlen(name);
	myJsonArray[myJsonWpt] = '\"';
	strcat(&myJsonArray[myJsonWpt],name);
	myJsonWpt += length;
	strcat(&myJsonArray[myJsonWpt],"\":[");
	myJsonWpt += 3;

	int i=0;
	for(i=0;i<len;i++)
	{
		if(i!=0)
		{
			strcat(&myJsonArray[myJsonWpt++],",");
		}
		/*传入数值内容*/
		myJson_itoa(val[i],intStr,10);
		length = strlen(intStr);
		strcat(&myJsonArray[myJsonWpt],intStr);
		myJsonWpt += length;
		
	}
	myJsonWpt-=2;
	strcat(&myJsonArray[myJsonWpt],"]}");
	myJsonWpt+=4;
	
	#if 0//UBUNTU_TEST
	printf("myJsonWpt:%d\r\n",myJsonWpt);
	printf("%s\r\n",myJsonArray);
	#endif
	return MYJSON_OK;
}
 
/*
功能:JSON 校验函数
输入:字符串
输出:合法JAON 返回MYJSON_OK 不合法JSON 返回MYJSON_ERR
*/
char myJson_check(char *str)
{
    char str_length; //定义字符长度变量
    str_length = myJson_strlen(str); //计算字符长度
    if(str[0] == '{' && str[str_length-1] == '}') //通过首尾大括号判断是否为JSON
    {
        return MYJSON_OK; //如果字符串为合法JSON
    }
    else
    {
        return MYJSON_ERR; //如果字符串为合法JSON
    }
}
 
/*
功能:JSON 键值对比函数
输入:JSON 键值 要匹配的字符
输出:如果获取成功返回MYJSON_OK 获取失败返回MYJSON_ERR
*/
char myJson_checkValue(char *str1, char *str2)
{
    if(myJson_strcmp(str1,str2) == 0)
    {
        return MYJSON_OK; //匹配成功
    }
    else
    {
        return MYJSON_ERR;//匹配成功
    }
}

/*
功能:JSON 获取对象内容
输入:JSON字符串 要获取的键名 获取键值的字符串 数值  数据类型
输出:如果获取成功返回MYJSON_OK 获取失败返回MYJSON_ERR
*/
static char myJson_getData(char *json,char *json_key , char *json_value,int *val, char type)
{
	char *json_key_start; //定义键名开始的位置
    char *json_key_end; //定义键名结束的位置
    char json_key_length; //定义键名长度
    
    char *json_value_start; //定义键值开始的位置
    char *json_value_end; //定义键值结束的位置
    char json_value_length; //定义键值长度
    
    json_key_start = myJson_strstr(json,json_key); //获取键名开始的位置
    json_key_length = myJson_strlen(json_key); //获取键名的长度
    json_key_end = json_key_start + json_key_length;  //获取键名结束的位置
    
    if(json_key_start != 0 && *(json_key_start - 1) == '\"' && *(json_key_end) == '\"' && *(json_key_end + 1) == ':')
    {
        json_value_start = json_key_end + 2 + type; //获取键值开始的位置
		json_value_end = (type == STR_TYPE)?myJson_strstr(json_value_start,"\""):myJson_strstr(json_value_start,",");
        json_value_length = json_value_end - json_value_start; //获取键值的长度
        
		if(type == STR_TYPE)
		{
			myJson_strncpy(json_value,json_value_start,json_value_length); //将键值存入指针数组
			json_value[json_value_length] = '\0'; //指针最后一位补字符串结束符 \0
		}
		else
		{
			cleanBuffer();
			myJson_strncpy(myJsonBuffer,json_value_start,json_value_length); //将键值存入指针数组
			*val = myJson_atol(myJsonBuffer);
		}
        
        return MYJSON_OK; //成功获取键值
    }
    else
    {
		if(type == STR_TYPE)
			json_value[0] = '\0';
        return MYJSON_ERR; //失败获取键值
    }
}

/*
功能:JSON 获取字符串函数
输入:JSON字符串 要获取的键名 获取键值的字符串
输出:如果获取成功返回MYJSON_OK 获取失败返回MYJSON_ERR
*/
char myJson_getStr(char *json,char *json_key , char *json_value)
{
    return myJson_getData(json,json_key,json_value,NULL,STR_TYPE);
}

/*
功能:JSON 获取数值函数
输入:JSON字符串 要获取的键名 数值
输出:如果获取成功返回MYJSON_OK 获取失败返回MYJSON_ERR
*/
char myJson_getValue(char *json,char *json_key,int *val)
{
	return myJson_getData(json,json_key,NULL,val,VAL_TYPE);
}
 
/*
功能:JSON 获取数组数据
输入:JSON字符串 要获取的键名 数值缓存数组 数据长度
输出:如果获取成功返回MYJSON_OK 获取失败返回MYJSON_ERR
*/
char myJson_getArray(char *json,char *json_key,int *val,int len)
{
	char *json_key_start; //定义键名开始的位置
    char *json_key_end; //定义键名结束的位置
    char json_key_length; //定义键名长度
    
    char *json_value_start; //定义键值开始的位置
    char *json_value_end; //定义键值结束的位置
    char json_value_length; //定义键值长度
    
    json_key_start = myJson_strstr(json,json_key); //获取键名开始的位置
    json_key_length = myJson_strlen(json_key); //获取键名的长度
    json_key_end = json_key_start + json_key_length;  //获取键名结束的位置
    
    if(json_key_start != 0 && *(json_key_start - 1) == '\"' && *(json_key_end) == '\"' && *(json_key_end + 1) == ':')
    {
        json_value_start = json_key_end + 3; //获取键值开始的位置

		int i=0;
		for(i=0;i< len;i++)
		{
			json_value_end = myJson_strstr(json_value_start,",");
			json_value_length = json_value_end - json_value_start; //获取键值的长度
		
			cleanBuffer();
			myJson_strncpy(myJsonBuffer,json_value_start,json_value_length); //将键值存入指针数组
			val[i] = myJson_atol(myJsonBuffer);
			json_value_start = json_value_end + 1;//移动数据开始位置
		}
        
        return MYJSON_OK; //成功获取键值
    }
    else
    {
        return MYJSON_ERR; //失败获取键值
    }
}
 
/************************************JSON扩展功能**************************************/
/*
功能:JSON 获取数组数据
输入:JSON字符串 要获取的键名 获取键值的字符串
输出:如果获取成功返回MYJSON_OK 获取失败返回MYJSON_ERR
*/
char myJson_addChild(char *json,char *json_key,int *val,int len)


