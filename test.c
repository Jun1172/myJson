#include "stdio.h"
#include "myJson.h"

int main()
{
	int data[12]={-3,0,-1515,20,45,18,151,9,-49,-87,47,-564};
	int getdata[12]={0};
	myJson_creatObject();

	myJson_addString("test4","datat1");
	myJson_addInt("val1",-20);
	myJson_addInt("val2",-1234567);
	myJson_addString("test1","datat1");
	myJson_addString("test2","datat2");
	myJson_addInt("val3",0);

	myJson_addInt("val5",555);
	myJson_addArray("arr1",data,12);

	myJson_addString("test3","datat3");

	myJson_addInt("val4",-456);
	myJson_addString("test5","datat2");
	myJson_addString("test6","datat6");

	#if 0//UBUNTU_TEST
	printf("%s\r\n",myJsonArray);
	#endif

    char *json = myJsonArray;
    char rs = myJson_check(json);
    if(rs == MYJSON_OK){
       printf("json is valid....\r\n");
    }else{
        printf("json is invalid....\r\n");
    }
    
    char value[127];
	int intValue;
    rs =  myJson_getArray(json,"arr1",getdata,12);
    if(rs == MYJSON_OK){
		int i=0;
		for(i=0;i<12;i++)
      	  printf("array%d:%d  \r\n",i,getdata[i]);
    }else{
        printf("parser val4 fail....\r\n");
    }
    
	return 0;
}
