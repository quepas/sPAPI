# sPAPI
Simple [**Scilab**](http://www.scilab.org/) API for [**PAPI**](http://icl.cs.utk.edu/papi/) (Performance Application Programming Interface).

## Installation

1. Install [PAPI >=5.5.1](http://icl.cs.utk.edu/papi/)
3. Run `scilab` in sPAPI directory
4. Compile `sPAPI_register`, `sPAPI_tic`, and `sPAPI_toc` functions from Scilab console using _ilib_mex_build_ function:

```scilab
ilib_mex_build('sPAPI', ['sPAPI_tic', 'sPAPI_tic', 'cmex'; 'sPAPI_toc', 'sPAPI_toc', 'cmex'; 'sPAPI_register', 'sPAPI_register', 'cmex'], [], [], '', '-lpapi', '', '');
```

## Usage

1. Register hardware performance monitoring counters (PMC) using _preset_ or _native_ events:
```scilab
>> sPAPI_register('FP_ARITH:SCALAR_SINGLE', 'L1D:REPLACEMENT', 'PAPI_L2_ICA')
```
2. Start counters:
```scilab
>> sPAPI_tic
```
3. Read counters measurements:
```scilab
>> sPAPI_toc

ans = [0  577  1123]
```

## Comments

* The number of hardware counters available on the system defines the upper limit of counters you can register using ``sPAPI_register`` function.
* Not all hardware counters can be mixed and used simultaneously.

## Future works

* Use new [api_scilab 6.0](https://help.scilab.org/docs/6.0.0/en_US/api_scilab.html) for sPAPI