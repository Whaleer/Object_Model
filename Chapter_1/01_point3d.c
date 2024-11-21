#include <stdio.h>
#define Point3d_print(pd) \
    printf("(%g, %g, %g)", pd.x, pd.y, pd.z);


typedef struct point3d{
    float x;
    float y;
    float z;
}Point3d;

//void Point3d_print(const Point3d *pd){
//    printf("(%g, %g, %g)", pd->x, pd->y, pd->z);
//}

int main(){
    Point3d pd;
    pd.x = 1.0;
    pd.y = 2.0;
    pd.z = 3.0;
    Point3d_print(pd);
}