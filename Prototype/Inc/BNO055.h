#ifndef __BNO055_H
#define __BNO055_H

#include <stdbool.h>

void BNO055_init(void);
bool BNO055_new_data(void);
void BNO055_get_data(void);
void BNO055_request_data(void);
void BNO055_refresh_orientation(void);


#endif /* __BNO055_H */
