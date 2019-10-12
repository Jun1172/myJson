#include "stdio.h"
#include "myJson.h"

int main()
{
	int data[12]={-3,0,-1515,20,45,18,151,9,-49,-87,47,-564};
	
	int getdata[12]={0};
    char getstr[12];

	myJson_creatObject();

	myJson_addString("test1","datat1");
	myJson_addInt("val1",-20);
	myJson_addArray("arr1",data,12);
    myJson_creatArrayChild("child");
    myJson_addStrToChild("jun");
    myJson_addValToChild(-789);
    myJson_addStrTargetToChild("tetr","mystr");
    myJson_addArrayTargetToChild("childArr",data,12);
    myJson_addValTargetToChild("testdata",-87);
    myJson_returnArrayChild();
    myJson_addString("test2","datat2");
    myJson_addInt("val2",7);

	#if 1
	printf("%s\r\n",myJsonArray);
	#endif

    /*char *json = myJsonArray;
    char rs = myJson_check(json);
    if(rs == MYJSON_OK){
       printf("json is valid....\r\n");
    }else{
        printf("json is invalid....\r\n");
    }
    
	char value[127]={"{\"data\":[23,{\"val1\":12},{\"val2\":34},{\"arr\":[3,5,7]}]}"};
	int intValue;
    rs =  myJson_getArray(value,"arr",getdata,3);
	if(rs == MYJSON_OK){
		int i=0;
		for(i=0;i<3;i++)
      	  printf("array%d:%d  \r\n",i,getdata[i]);
    }else{
        printf("parser val4 fail....\r\n");
    }

	rs =  myJson_getStr(value,"val2",getstr);
    if(rs == MYJSON_OK){
      	printf("str:%s  \r\n",getstr);
    }else{
        printf("parser val4 fail....\r\n");
    }*/
    
	return 0;
}
