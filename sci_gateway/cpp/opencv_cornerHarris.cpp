/********************************************************
Author: Shubheksha Jalan
********************************************************/
#include <numeric>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
using namespace cv;
using namespace std;
extern "C"
{
  #include "api_scilab.h"
  #include "Scierror.h"
  #include "BOOL.h"
  #include <localization.h>
  #include "../common.h"

  int opencv_cornerHarris(char *fname, unsigned long fname_len)
  {
        // Error management variables
        SciErr sciErr;
        int intErr = 0;

        // variables required to read arguments
        int iRows=0,iCols=0;
        int *piLen = NULL;
        int *piAddr = NULL;
        int *piAddrNew = NULL;
        int *piAddr2  = NULL;
        int *piAddr3  = NULL;
        int *piAddr4 = NULL;
        int *piAddr5 = NULL;
        int i,j;
        double blockSize=0, ksize=0, k=0;
        char **borderType;

        //Checking number of input and output arguments (enviromnet variable, min arguments, max arguments)
        CheckInputArgument(pvApiCtx, 5, 5);
        CheckOutputArgument(pvApiCtx, 1, 1);

        // to get the argument #1
        Mat image;
        retrieveImage(image, 1);
        image.convertTo(image,CV_32F);
        cvtColor(image,image,CV_BGR2GRAY);

        // to get the argument #2
        sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
         intErr = getScalarDouble(pvApiCtx, piAddr2, &blockSize);
         if(intErr)
        {
            return intErr;
        }

        // to get the argument #3
        //for ksize
        sciErr = getVarAddressFromPosition(pvApiCtx,3,&piAddr3);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
         intErr = getScalarDouble(pvApiCtx, piAddr3, &ksize);
        if(intErr)
        {
            return intErr;
        }

        // to get the argument #4
        //for value of k
        sciErr = getVarAddressFromPosition(pvApiCtx,4,&piAddr4);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
         intErr = getScalarDouble(pvApiCtx, piAddr4, &k);
        if(intErr)
        {
            return intErr;
        }

        // to get the argument #5
        //for border type
        sciErr = getVarAddressFromPosition(pvApiCtx, 5, &piAddr5);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
        //Now, we will retrieve the string from the input parameter. For this, we will require 3 calls
        //first call to retrieve dimensions
        sciErr = getMatrixOfString(pvApiCtx, piAddr5, &iRows, &iCols, NULL, NULL);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
        piLen = (int*)malloc(sizeof(int) * iRows * iCols);
        //second call to retrieve length of each string
        sciErr = getMatrixOfString(pvApiCtx, piAddr5, &iRows, &iCols, piLen, NULL);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
        borderType = (char**)malloc(sizeof(char*) * iRows * iCols);
        for(i = 0 ; i < iRows * iCols ; i++)
            borderType[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));//+ 1 for null termination
        //third call to retrieve data
        sciErr = getMatrixOfString(pvApiCtx, piAddr5, &iRows, &iCols, piLen, borderType);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        //Application Code
        Mat new_image(image.rows, image.cols, CV_32FC1);
        try
        { 
            if(strcmp(borderType[0], "BORDER_CONSTANT") == 0){
                   cornerHarris(image, new_image, blockSize, ksize, k, BORDER_CONSTANT);
               }
            else if(strcmp(borderType[0], "BORDER_DEFAULT") == 0)
                cornerHarris(image, new_image, blockSize, ksize, k, BORDER_DEFAULT);
        }
        catch(cv::Exception& e)
        {
            const char* err=e.what();
            Scierror(999,("%s"),err);
        }
        
        //returning final Image
        new_image.convertTo(new_image, CV_32FC1, 1.0/255.0);
        string tempstring = type2str(new_image.type());
        char *checker;
        checker = (char *)malloc(tempstring.size() + 1);
        memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
        returnImage(checker,new_image,1);
        free(checker);

        //Assigning the list as the Output Variable
        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;

        //Returning the Output Variables as arguments to the Scilab environment
        ReturnArguments(pvApiCtx);
        return 0;

  }
/* ==================================================================== */
}
