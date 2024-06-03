#include "viewer.h"
#include "../math/geometry.h"
#include "button.h"

#define DEFAULT_ANGLE = 

static polar_coord_three_dims_t viewer_coord;

void viewer_process_main()
{
    uint8_t button_left = BUTTON_left_read();
	uint8_t button_right = BUTTON_right_read();
	uint8_t button_center = BUTTON_center_read();
    uint8_t button_up = BUTTON_up_read();
    uint8_t button_down = BUTTON_down_read();
}

void viewer_button(button_left, button_right, button_up, button_down)
{
    if (button_left)
    {
        polar_coord_three_dims_t.distance += polar_coord_three_dims_t.alpha;
    }

    else if (button_right)
    {
        polar_coord_three_dims_t.distance -= polar_coord_three_dims_t.alpha;
    }

    else if (button_up)
    {
        polar_coord_three_dims_t.distance += polar_coord_three_dims_t.beta;
    }

    else if (button_up)
    {
        polar_coord_three_dims_t.distance -= polar_coord_three_dims_t.beta;
    }

    else 
    {
        polar_coord_three_dims_t.distance = 0
    }
}