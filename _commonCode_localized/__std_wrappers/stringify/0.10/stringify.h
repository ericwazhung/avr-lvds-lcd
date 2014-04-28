//stringify.h 0.10

//0.10-1 - Looking into DESTRINGIFY
//         a/o sdramThing3.0-0.7+delayCyc0.10ncf
//         NOGO
//0.10 - first version, stolen from cTools/preprocTest


/* from info cpp 3.4:
   There is no way to combine an argument with surrounding text and
   stringify it all together.  Instead, you can write a series of adjacent
   string constants and stringified arguments.  The preprocessor will
   replace the stringified arguments with string constants.  The C
   compiler will then combine all the adjacent string constants into one
   long string.

   That dun woik for #include...
*/

//Also info cpp 3.5: Concatenation

// FROM: http://stackoverflow.com/questions/1767683/c-programming-preprocessor-macros-as-tokens/1769037#1769037
// AND:  http://stackoverflow.com/questions/2831934/c-preprocessor-using-if-inside-define

// It's best to have a level of indirection...

// Use STRINGIFY(x) and PASTE(a,b)


// STRINGIFY(x) -> "x"
// PASTE(a,b) -> ab (not quoted)
//  e.g. int a1;
//       PASTE(a,1) = 1;
//       -> a1 = 1;


//ACTUALLY, I think I got these backwards... look at 3.4
#define STRINGIFY_INDIRECTOR( x) #x
#define STRINGIFY(x) STRINGIFY_INDIRECTOR(x)

#define PASTE_INDIRECTOR( a, b) a##b
#define PASTE( a, b) PASTE_INDIRECTOR( a, b)

//Look into info cpp: DO_PRAGMA or search for "destring"
//#define DESTRINGIFY(string)	_Pragma(string)
