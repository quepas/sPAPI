#include <stdio.h>
#include "mex.h"
#include "papi.h"

/*
 *  sPAPI_register -- registers and starts hardware performance monitoring counters.
 *                   Each input argument is a generic event name
 *                   or cpu-specific event name.
 */
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int papi_num_counters = PAPI_num_counters();
    if (nrhs < 1 || nrhs > papi_num_counters)
    {
        char error[256];
        sprintf(error, "sPAPI_register takes from 1 to %d "
                       "counter descriptors (up to number of available "
                       "hardware performance monitoring counters)",
                papi_num_counters);
        mexErrMsgTxt(error);
    }

    /* Stop all available counters (even not running ones) */
    long long mock_values[papi_num_counters];
    PAPI_stop_counters(mock_values, papi_num_counters);

    /* Set an output vector */
    plhs[0] = mxCreateNumericMatrix(1, nrhs, mxINT32_CLASS, mxREAL);
    int *counters = (int *)mxGetData(plhs[0]);

    int i, result;
    for (i = 0; i < nrhs; ++i)
    {
        if (!mxIsChar(prhs[i]))
        {
            mexErrMsgTxt("sPAPI_register requires a string with hardware counter name.");
        }

        size_t bufferSize = 256;
        char buffer[bufferSize];
        if (mxGetString(prhs[i], buffer, (mwSize)bufferSize) == 1)
        {
            mexErrMsgTxt("Error while reading a string name.");
        }

        /* Get integer code for each counter */
        if (result = PAPI_event_name_to_code(buffer, &(counters[i])) != PAPI_OK)
        {
            char error[256];
            sprintf(error, "Failed to obtained event code %s."
                           "Error message: %s.",
                    buffer, PAPI_strerror(result));
            mexErrMsgTxt(error);
        }
    }
    /* Start counters */
    if ((result = PAPI_start_counters(counters, nrhs)) != PAPI_OK)
    {
        char error[256];
        sprintf(error, "sPAPI_register couldn't start counters. Error message: %s", PAPI_strerror(result));
        mexErrMsgTxt(error);
    }
    /* Register number of running counters */
    mxArray *sPAPI_status = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
    int *status = (int *)mxGetData(sPAPI_status);
    *status = nrhs;
    mexPutVariable("global", "sPAPI_status", sPAPI_status);
}
