#ifndef __IO_SERVICES__
  #define __IO_SERVICES__ 1

  #include "diocommon.h"
  #include "dio.h"

  #include "s99.h"

  /*
   * Error codes from dsd_alloc
   */
   #define IOSVC_ERR_NOERROR                  0
   #define IOSVC_ERR_SVC99INIT_ALLOC_FAILURE  4
   #define IOSVC_ERR_SVC99_ALLOC_FAILURE      8

  enum DIOERR dsdd_alloc(struct DFILE* dfile, struct s99_common_text_unit* dsn, struct s99_common_text_unit* dd, struct s99_common_text_unit* disp);
  enum DIOERR ddfree(struct DFILE* dfile, struct s99_common_text_unit* dd);
  enum DIOERR init_dsnam_text_unit(struct DFILE* dfile, const char* dsname, struct s99_common_text_unit* dsn);
#endif
