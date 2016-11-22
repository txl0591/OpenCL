/*********************************************************
 Copyright (C),2013-2019,Electronic Technology Co.,Ltd.
 File name: 		OpenCLRun.cpp
 Author: 			Txl
 Version: 			1.0
 Date: 				2016-11-1
 Description: 		
 History: 			
 					
   1.Date:	 		2016-11-1
 	 Author:	 	Txl
 	 Modification:  Created file
 	 
*********************************************************/
#include "config.h"
#include "OpenCLEnv.h"

/*************************************************
 Function:		main
 Descroption:	Ö÷³ÌÐò
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
int main(void)
{   
    ms_message("**********************************************************");
    ms_message("*                        OpenCL Core                     *");
    ms_message("*Version:    %s                                    *",_BUILD_VERSION_);
    ms_message("*Build Time: %s                        *",_BUILD_TIME_);
    ms_message("**********************************************************");

	OpenCLEnv* mOpenCLEnv = new OpenCLEnv();
	if(mOpenCLEnv == NULL)	
	{
		ms_error("OpenCL Env Error");
	}
	
	mOpenCLEnv->Init();
	
	delete mOpenCLEnv;
		
    return 0;
}


