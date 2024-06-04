#include "rotate.h"

#include "geometry.h"
#include "geometry_unity.h"
#include "trigo.h"

#include <stdint.h>


void rotate(drawing_three_dims_t *drawing, angle_three_dims_t angle)
{
    // on applique une matrice de rotation sur l'axe x
    // | 1   0    0   |
    // | 0   cos_int -sin_int |
    // | 0   sin_int  cos_int |
    for (uint16_t i = 0; i < drawing->nb_segment; i++)
    {
        // on récupère les coordonnées des points
        dist_t x1 = drawing->segment[i].p1.x;
        dist_t y1 = drawing->segment[i].p1.y;
        dist_t z1 = drawing->segment[i].p1.z;
        dist_t x2 = drawing->segment[i].p2.x;
        dist_t y2 = drawing->segment[i].p2.y;
        dist_t z2 = drawing->segment[i].p2.z;

        // on applique la rotation sur les points
        drawing->segment[i].p1.x = x1;
        drawing->segment[i].p1.y = y1 * cos_int(angle.angle_x)/QUANTUM - z1 * sin_int(angle.angle_x)/QUANTUM;
        drawing->segment[i].p1.z = y1 * sin_int(angle.angle_x)/QUANTUM + z1 * cos_int(angle.angle_x)/QUANTUM;
        drawing->segment[i].p2.x = x2;
        drawing->segment[i].p2.y = y2 * cos_int(angle.angle_x)/QUANTUM - z2 * sin_int(angle.angle_x)/QUANTUM;
        drawing->segment[i].p2.z = y2 * sin_int(angle.angle_x)/QUANTUM + z2 * cos_int(angle.angle_x)/QUANTUM;
    }

    // on applique une matrice de rotation sur l'axe y
    // |  cos_int 0 sin_int |
    // |  0   1  0  |
    // | -sin_int 0 cos_int |
    for (uint16_t i = 0; i < drawing->nb_segment; i++)
    {
        // on récupère les coordonnées des points
        dist_t x1 = drawing->segment[i].p1.x;
        dist_t y1 = drawing->segment[i].p1.y;
        dist_t z1 = drawing->segment[i].p1.z;
        dist_t x2 = drawing->segment[i].p2.x;
        dist_t y2 = drawing->segment[i].p2.y;
        dist_t z2 = drawing->segment[i].p2.z;

        // on applique la rotation sur les points
        drawing->segment[i].p1.x = x1 * cos_int(angle.angle_y)/QUANTUM + z1 * sin_int(angle.angle_y)/QUANTUM;
        drawing->segment[i].p1.y = y1;
        drawing->segment[i].p1.z = -x1 * sin_int(angle.angle_y)/QUANTUM + z1 * cos_int(angle.angle_y)/QUANTUM;
        drawing->segment[i].p2.x = x2 * cos_int(angle.angle_y)/QUANTUM + z2 * sin_int(angle.angle_y)/QUANTUM;
        drawing->segment[i].p2.y = y2;
        drawing->segment[i].p2.z = -x2 * sin_int(angle.angle_y)/QUANTUM + z2 * cos_int(angle.angle_y)/QUANTUM;
    }

    // on applique une matrice de rotation sur l'axe z
    // | cos_int -sin_int 0 |
    // | sin_int  cos_int 0 |
    // | 0    0   1 |
    for (uint16_t i = 0; i < drawing->nb_segment; i++)
    {
        // on récupère les coordonnées des points
        dist_t x1 = drawing->segment[i].p1.x;
        dist_t y1 = drawing->segment[i].p1.y;
        dist_t z1 = drawing->segment[i].p1.z;
        dist_t x2 = drawing->segment[i].p2.x;
        dist_t y2 = drawing->segment[i].p2.y;
        dist_t z2 = drawing->segment[i].p2.z;

        // on applique la rotation sur les points
        drawing->segment[i].p1.x = x1 * cos_int(angle.angle_z)/QUANTUM - y1 * sin_int(angle.angle_z)/QUANTUM;
        drawing->segment[i].p1.y = x1 * sin_int(angle.angle_z)/QUANTUM + y1 * cos_int(angle.angle_z)/QUANTUM;
        drawing->segment[i].p1.z = z1;
        drawing->segment[i].p2.x = x2 * cos_int(angle.angle_z)/QUANTUM - y2 * sin_int(angle.angle_z)/QUANTUM;
        drawing->segment[i].p2.y = x2 * sin_int(angle.angle_z)/QUANTUM + y2 * cos_int(angle.angle_z)/QUANTUM;
        drawing->segment[i].p2.z = z2;
    }
}
