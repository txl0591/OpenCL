/*********************************************************
 Copyright (C),2013-2019,Electronic Technology Co.,Ltd.
 File name: 		utils.cpp
 Author: 			Txl
 Version: 			1.0
 Date: 				2016-11-17
 Description: 		
 History: 			
 					
   1.Date:	 		2016-11-17
 	 Author:	 	Txl
 	 Modification:  Created file
 	 
*********************************************************/

#include "utils.h"

/*************************************************
 Function:		GetPlatformFromName
 Descroption:	 
 Input: 
	1.*name
 Output: 
 Return: 	
 Other:  
*************************************************/
PLATFORM_TYPE GetPlatformFromName(char *name)
{
	if(0 == strcmp(name, ADRENO_NAME))
	{
		return PLATFORM_ADRENO;
	}
	else if(0 == strcmp(name, MALI_NAME))
	{
		return PLATFORM_MAIL;
	}

	return PLATFORM_UNKNOW;
}

/*************************************************
 Function:		GetPlatformFromType
 Descroption:	 
 Input: 
	1.platform
 Output: 
 Return: 	
 Other:  
*************************************************/
char* GetPlatformFromType(PLATFORM_TYPE platform)
{
	if(platform == PLATFORM_ADRENO)
	{
		return (char*)ADRENO_NAME;
	}
	else if (platform == PLATFORM_MAIL)
	{
		return (char*)MALI_NAME;
	}

	return (char*)"Unknow";
}

