#ifndef __MACROS__
#define __MACROS__

#define CHECK_RETURN_0(ptr) {if((ptr) == NULL)\
								return 0; }


#define CHECK_RETURN_NULL(ptr) {if((ptr) == NULL)\
								return NULL;}

#define CHECK_0_MSG_CLOSE_FILE(value, fp ,msg) {if((value) == 0 ){printf(#msg"\n");fclose(fp);return 0;}}
#define CHECK_NULL_MSG_CLOSE_FILE(value, fp ,msg) {if((value) == NULL ){printf(#msg"\n");fclose(fp);return 0;}}


#endif
