/* mybc definitions */
enum VALUETYPE { ULONG, LONG, DOUBLE, UNKNOWN };
typedef enum VALUETYPE TYPE;

/* structure for storing symbol information */
struct VALUEDATA {
	TYPE type;
	union {
		long lvalue;
		unsigned long ulvalue;
		double dvalue;
	} value;
};
typedef struct VALUEDATA VALUE;

/* functions */
extern void mybcerror();
extern char *num2base();
extern VALUE number2double();
