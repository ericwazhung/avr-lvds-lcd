//errorHandling .99

//0.99 now returns num in the high nibble, and the function's return value
//     in the low nibble...
//0.98 first version

#ifndef __ERRORHANDLING_H__
#define __ERRORHANDLING_H__

//If __ERR_DISABLE__ is true, save a few bytes by not testing...
#ifndef __ERR_DISABLE__
 #define __ERR_DISABLE__ FALSE
#endif


#if !(__ERR_DISABLE__)
 #define ERR_HANDLE(function)				\
 {											\
	if(uint8_t err_val = (function))		\
		return err_val;						\
 }

 #define ERR_HANDLE_NUM(num, function)    \
 {                                        \
	uint8_t err_val = (function);          \
	if(err_val)                            \
		return ((num<<4) | err_val);        \
 }
#else
 #define ERR_HANDLE(function)			(function)
 #define ERR_HANDLE_NUM(num, function)	(function)
#endif

//Problem 1:
// if(test)
//   ERR_HANDLE_NUM(2, fn());
// else...
//
//becomes
// if(test)
// {
//   if((fn()))
//		return 2;
// }; <<<<-----
// else...
//BUT:
// the semicolon closes the if statement, and else appears from nowhere...
//SO:
//Either:
//enclose the macro in brackets...
// if(test)
// {
//   ERR_...
// }
//becomes
// if(test)
// {
//   {
//     if(fn())
//   };
// }
//OR:
//don't use a semicolon... 
// if(test)
//   ERR_HANDLE(fn())
//either way's hokey...
#endif
