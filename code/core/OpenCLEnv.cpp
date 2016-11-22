/*********************************************************
 Copyright (C),2013-2019,Electronic Technology Co.,Ltd.
 File name: 		OpenCLEnv.cpp
 Author: 			Txl
 Version: 			1.0
 Date: 				2016-10-31
 Description: 		
 History: 			
 					
   1.Date:	 		2016-10-31
 	 Author:	 	Txl
 	 Modification:  Created file
 	 
*********************************************************/
#include "config.h"
#include "OpenCLEnv.h"

static CLDeviceInfo mCLDeviceInfo[] =
{
        {CL_DEVICE_NAME,    "Device Name"},
        {CL_DEVICE_VENDOR,  "Device Vendor"},
        {CL_DEVICE_PROFILE, "Device Profile"},
        {CL_DEVICE_VERSION, "Device Version"},
};

OpenCLEnv::OpenCLEnv()
:mDeviceGPU(0)
,mDeviceCPU(0)
,mPlatformID(NULL)
,mNumPlatform(0)
,mDeviceID(NULL)
,mNumDevice(0)
,mContext(NULL)
,mCmdueue(NULL)
,mDevices(CL_DEVICE_TYPE_GPU)
{
	mDlOpenLib = new DlOpenLib();
	mDlOpenLib->addlib(_ADRENO_BIT32_PATH_, TARGET_BIT32, PLATFORM_ADRENO);
	mDlOpenLib->addlib(_MALI_BIT32_PATH_, TARGET_BIT32, PLATFORM_MAIL);
	mDlOpenLib->addlib(_ADRENO_BIT64_PATH_, TARGET_BIT64, PLATFORM_ADRENO);
	mDlOpenLib->addlib(_MALI_BIT64_PATH_, TARGET_BIT64, PLATFORM_MAIL);
	mOpenCLHandle.mHandle = mDlOpenLib->openlib();
	if(mOpenCLHandle.mHandle != NULL)
	{
		mOpenCLHandle.m_clGetPlatformIDs = (cl_int (*)())(dlsym (mOpenCLHandle.mHandle, _CL_GETPLATFORMIDS_));
		mOpenCLHandle.m_clGetPlatformInfo = (cl_int (*)())(dlsym (mOpenCLHandle.mHandle, _CL_GETPLATFORMINFO_));
		mOpenCLHandle.m_clGetDeviceIDs = (cl_int (*)())(dlsym (mOpenCLHandle.mHandle, _CL_GETDEVICEIDS_));
		mOpenCLHandle.m_clGetDeviceInfo = (cl_int (*)())(dlsym (mOpenCLHandle.mHandle, _CL_GETDEVICEINFO_));
		mOpenCLHandle.m_clCreateContext = (cl_context (*)())(dlsym (mOpenCLHandle.mHandle, _CL_CREATECONTEXT_));
		mOpenCLHandle.m_clReleaseContext = (cl_int (*)())(dlsym (mOpenCLHandle.mHandle, _CL_RELEASECONTEXT_));
		mOpenCLHandle.m_clCreateCommandQueue = (cl_command_queue (*)())(dlsym (mOpenCLHandle.mHandle, _CL_CREATECOMMANDQUEUE_));
		mOpenCLHandle.m_clReleaseCommandQueue = (cl_int (*)())(dlsym (mOpenCLHandle.mHandle, _CL_RELEASECOMMANDQUEUE_));
		mOpenCLHandle.m_clCreateProgramWithSource = (cl_program (*)())(dlsym (mOpenCLHandle.mHandle, _CL_CREATEPROGRAMWITHSOURCE_));
		mOpenCLHandle.m_clCreateProgramWithBinary = (cl_program (*)())(dlsym (mOpenCLHandle.mHandle, _CL_CREATEPROGRAMWITHBIN_));
		mOpenCLHandle.m_clReleaseProgram = (cl_int (*)())(dlsym (mOpenCLHandle.mHandle, _CL_RELEASEPROGRAM_));
		mOpenCLHandle.m_clGetProgramBuildInfo = (cl_int (*)())(dlsym (mOpenCLHandle.mHandle, _CL_GETPROGRAMINFO_));
		mOpenCLHandle.m_clBuildProgram = (cl_int (*)())(dlsym (mOpenCLHandle.mHandle, _CL_BUILDPROGRAM_));
		mOpenCLHandle.m_clCreateKernel = (cl_kernel (*)())(dlsym (mOpenCLHandle.mHandle, _CL_CREATEKERNEL_));
		mOpenCLHandle.m_clReleaseKernel = (cl_int (*)())(dlsym (mOpenCLHandle.mHandle, _CL_RELEASEKERNEL_));
		mOpenCLHandle.m_clGetKernelWorkGroupInfo = (cl_int (*)())(dlsym (mOpenCLHandle.mHandle, _CL_GETKERNELWORKGROUP_));
		mOpenCLHandle.m_clSetKernelArg = (cl_int (*)())(dlsym (mOpenCLHandle.mHandle, _CL_SETKERNELARG_));
		mOpenCLHandle.m_clGetKernelInfo = (cl_int (*)())(dlsym (mOpenCLHandle.mHandle, _CL_SETKERNELINFO_));
		mOpenCLHandle.m_clCreateBuffer = (cl_mem (*)())(dlsym (mOpenCLHandle.mHandle, _CL_CREATEBUFFER_));
		mOpenCLHandle.m_clReleaseMemObject = (cl_int (*)())(dlsym (mOpenCLHandle.mHandle, _CL_RELEASEMEMOBJ_));
		mOpenCLHandle.m_clEnqueueReadBuffer = (cl_int (*)())(dlsym (mOpenCLHandle.mHandle, _CL_ENQUEUEREADBUFFER_));
		mOpenCLHandle.m_clEnqueueNDRangeKernel = (cl_int (*)())(dlsym (mOpenCLHandle.mHandle, _CL_ENQUEUENDRKERNEL_));
		mOpenCLHandle.m_clCreateContextFromType = (cl_context (*)())(dlsym (mOpenCLHandle.mHandle, _CL_CREATECONTEXTFORMTYPE_));
	}
}

OpenCLEnv::~OpenCLEnv()
{
	ReleaseContext();
	if(mDlOpenLib)
	{
		delete mDlOpenLib;
	}
}

cl_int OpenCLEnv::CreateContext()
{
	cl_int err = CL_SUCCESS;

    /** Create the context */
    if(mOpenCLHandle.m_clCreateContext)
    {
        mContext = mOpenCLHandle.m_clCreateContext(0, 1, &mDeviceID, NULL, NULL, &err);
        if(CL_SUCCESS != err)
        {
            ms_error("clCreateContext failed: error: %d\n", err);
            return err;
        }
    }

    if(mOpenCLHandle.m_clCreateCommandQueue)
    {
        mCmdueue = mOpenCLHandle.m_clCreateCommandQueue(mContext, mDeviceID, 0, &err);
        if(CL_SUCCESS != err)
        {
            ms_error("clCreateCommandQueue failed: error: %d\n", err);
        }
    }
	return err;
}

cl_int OpenCLEnv::ReleaseContext()
{
	cl_int err = CL_SUCCESS;

	if(mContext == NULL)
	{
	    return CL_DEVICE_NOT_FOUND;
	}

	if(mOpenCLHandle.m_clReleaseCommandQueue)
	{
	    err= mOpenCLHandle.m_clReleaseCommandQueue(mCmdueue);
	}

	if(mOpenCLHandle.m_clReleaseContext)
	{
	    err= mOpenCLHandle.m_clReleaseContext(mContext);
	}

	return err;
}

cl_int OpenCLEnv::GetPlatformInfo()
{
	int i;
    char platformInfo[1024];
    char deviceInfo[1024];
	cl_int err = CL_DEVICE_NOT_FOUND;
	cl_platform_id* platforms = NULL;
	
	/** Get the platform Number*/
    if(mOpenCLHandle.m_clGetPlatformIDs)
    {
        err = mOpenCLHandle.m_clGetPlatformIDs(0, NULL, &mNumPlatform);
        if(CL_SUCCESS != err)
        {
            ms_error("clGetPlatformIDs failed: error: %d\n", err);
            goto ErrorPlatForm;
        }
        else
        {
            if(mNumPlatform == 0)
            {
                ms_error("No platform is available\n Exiting ...");
                goto ErrorPlatForm;
            }
            else
            {
                ms_message("platform Number [%d]",mNumPlatform);
            }
        }
    }
	platforms = (cl_platform_id *)malloc(sizeof(cl_platform_id)*mNumPlatform);
    if(platforms == NULL)
    {
        goto ErrorPlatForm;
    }

	if(mOpenCLHandle.m_clGetPlatformIDs)
    {
        err = mOpenCLHandle.m_clGetPlatformIDs(mNumPlatform, platforms, NULL);
        if(CL_SUCCESS != err)
        {
            ms_error("clGetPlatformIDs failed: error: %d\n", err);
            goto ErrorPlatForm;
        }
        else
        {
            if(mNumPlatform == 0)
            {
                ms_error("No platform is available\n Exiting ...");
                goto ErrorPlatForm;
            }
        }
    }

	/** Check for platform*/
    mPlatformID = 0;

    if(mOpenCLHandle.m_clGetPlatformInfo)
    {
        for (i = 0; i < mNumPlatform; i++)
        {
            err = mOpenCLHandle.m_clGetPlatformInfo(platforms[i],CL_PLATFORM_VENDOR, sizeof(platformInfo), platformInfo, NULL);
            if (CL_SUCCESS != err)
            {
                ms_error("clGetPlatformInfo failed: error: %d\n", err);
                goto ErrorPlatForm;
            }
            if(!strcmp(GetPlatformFromType(mDlOpenLib->getplatform()), platformInfo))
            {
                mPlatformID = platforms[i];
                break;
            }
        }
    }

    if(mPlatformID)
    {
        ms_message("Platform found: %s\n", platformInfo);
    }
    else
    {
        ms_message("Desired platform not found\n Using default one.\n");
        mPlatformID = platforms[0];
    }

	if(mOpenCLHandle.m_clGetDeviceIDs)
    {
        err = mOpenCLHandle.m_clGetDeviceIDs(mPlatformID, CL_DEVICE_TYPE_CPU, 0, 0, &mDeviceCPU);
        if(CL_SUCCESS != err)
        {
            ms_error("clGetDeviceIDs failed: error: %d\n", err);
            goto ErrorPlatForm;
        }
    }

	if(mOpenCLHandle.m_clGetDeviceIDs)
    {
        err = mOpenCLHandle.m_clGetDeviceIDs(mPlatformID, CL_DEVICE_TYPE_GPU, 0, 0, &mDeviceGPU);
        if(CL_SUCCESS != err)
        {
            ms_error("clGetDeviceIDs failed: error: %d\n", err);
            goto ErrorPlatForm;
        }
    }

    GetDevicesInfo(mDeviceCPU, CL_DEVICE_TYPE_CPU);
    GetDevicesInfo(mDeviceGPU, CL_DEVICE_TYPE_GPU);
	
	/** Get the device*/
    if(mOpenCLHandle.m_clGetDeviceIDs)
    {
            int Count = 0;
            switch(mDevices)
            {
            case CL_DEVICE_TYPE_CPU:
                    Count = mDeviceCPU;
                    break;

            case CL_DEVICE_TYPE_GPU:
                    Count = mDeviceGPU;
                    break;

            default:
                    break;
            }

            err = mOpenCLHandle.m_clGetDeviceIDs(mPlatformID, mDevices, Count,&mDeviceID, &mNumDevice);
            if(CL_SUCCESS != err)
            {
                    ms_error("clGetDeviceIDs failed: error: %d\n", err);
                    goto ErrorPlatForm;
            }
    }
	
ErrorPlatForm:
	if(platforms != NULL)
	{	
		free(platforms);
		platforms = NULL;
	}
	return err;
}

cl_int OpenCLEnv::GetDevicesInfo(cl_int Devices, cl_int type)
{
    int i,j;
    cl_device_id mDeviceID;
    char deviceInfo[1024];
    cl_int err = CL_SUCCESS;

    ms_message("************************************************");
    for (i = 0; i < Devices; i++)
    {
	    if(mOpenCLHandle.m_clGetDeviceIDs)
	    {
            err = mOpenCLHandle.m_clGetDeviceIDs(mPlatformID, type, i+1, &mDeviceID, NULL);
            if(CL_SUCCESS != err)
            {
                ms_message("clGetDeviceIDs failed: error: %d\n", err);
                err = CL_DEVICE_NOT_FOUND;
                break;
            }
	    }
	    for (j = 0; j < (sizeof(mCLDeviceInfo)/sizeof(CLDeviceInfo)); j++)
	    {
            err = mOpenCLHandle.m_clGetDeviceInfo(mDeviceID, mCLDeviceInfo[j].ID, sizeof(deviceInfo), deviceInfo, 0);
            if(CL_SUCCESS != err)
            {
                ms_message("CL_DEVICE_NAME failed: error: %d\n", err);
            }
            else
            {
                ms_message("%s: [%s] ",mCLDeviceInfo[j].Name,deviceInfo);
            }
	    }
    }
    ms_message("************************************************");
    return err;
}

cl_int OpenCLEnv::Init()
{
	if(CL_SUCCESS == GetPlatformInfo())
	{
		return CreateContext();
	}

	return CL_DEVICE_NOT_AVAILABLE;
}
