/*********************************************************
 Copyright (C),2013-2019,Electronic Technology Co.,Ltd.
 File name: 		OpenCLKernel.h
 Author: 			Txl
 Version: 			1.0
 Date: 				2016-10-31
 Description: 		
 History: 			
 					
   1.Date:	 		2016-10-31
 	 Author:	 	Txl
 	 Modification:  Created file
 	 
*********************************************************/
#ifndef _OPENCLKERNEL_H_
#define _OPENCLKERNEL_H_

#include "OpenCLEnv.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

class OpenCLKernel
{
    public:
        OpenCLKernel(KernelType Type, char* Source, int SourceLen, OpenCLEnv* Env);
        ~OpenCLKernel();
    private:
        char *LoadProgramSource(const char *cFilename, size_t *finalLength);
        cl_program CreateProgramWithSource(char** Source);
        cl_program CreateProgramWithBinary(char** Source);
        cl_int BuildProgram(cl_program curClProgram);

        OpenCLEnv* mEnv;
        OpenCLHandle* mOpenCLHandle;
        cl_context   mContext;
        cl_device_id mDeviceID;

        KernelType  mKernelType;
        cl_kernel   kernel;
        size_t      WGsize;
        cl_ulong    localMemory;
        size_t      compiler_WGsize[3];
        char isSchedulerEnabled;
        char programBuildOptions[100];

};

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* ifndef _OPENCLKERNEL_H_ Edit By Txl 2016-10-31 */

