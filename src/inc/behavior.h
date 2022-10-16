#pragma once

#ifdef DEBUG
# include <stdlib.h>

# define BHV_ABORT() abort();

#else
# define BHV_ABORT() ;
#endif

# define BHV_IDX(I, L) if (I >= L) { BHV_ABORT() }
