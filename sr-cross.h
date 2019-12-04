#pragma once
#include "relacija.h"
#include "skup.h"

namespace automati {

skup operator* (const skup& a, const relacija& b);

skup operator* (const relacija& a, const skup& b);

// levi rezidual
relacija operator/ (const skup& b, const skup& a);

// desni rezidual
relacija operator% (const skup& a, const skup& b);

}