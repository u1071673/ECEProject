#ifndef __BNO055_H
#define __BNO055_H

#include <stdbool.h>
#include "USARTs.h"

void BNO055_init(void);
bool BNO055_new_data(void);
void BNO055_get_data(void);
void BNO055_request_data(void);
bool BNO055_orientation_updated(void);
euler_data BNO055_get_orientation(void);


#endif /* __BNO055_H */
