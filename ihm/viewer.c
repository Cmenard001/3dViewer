#include "viewer.h"
#include "../math/geometry.h"
#include "button.h"
#include "../math/geometry_unity.h"
#include "../math/drawing.h"

#define DEFAULT_ANGLE (PI_INT/64)

static polar_coord_three_dims_t viewer_coord = (polar_coord_three_dims_t){0, 0};

void button_init()
{
    BUTTONS_init();
}

static void viewer_button(uint8_t button_left, uint8_t button_right, uint8_t button_up, uint8_t button_down, uint8_t button_center);
static void viewer_button(uint8_t button_left, uint8_t button_right, uint8_t button_up, uint8_t button_down, uint8_t button_center)
{
    if (button_left)
    {
        viewer_coord.alpha += DEFAULT_ANGLE;
    }

    if (button_right)
    {
        viewer_coord.alpha -= DEFAULT_ANGLE;
    }

    if (button_up)
    {
        viewer_coord.beta -= DEFAULT_ANGLE;
    }

    if (button_down)
    {
        viewer_coord.beta += DEFAULT_ANGLE;
    }

    if (button_center)
    {
        increment_current_drawing();
    }
}

void viewer_process_main()
{
    uint8_t button_left = BUTTON_left_read();
	uint8_t button_right = BUTTON_right_read();
	uint8_t button_center = BUTTON_center_read();
    uint8_t button_up = BUTTON_up_read();
    uint8_t button_down = BUTTON_down_read();
    viewer_button(button_left, button_right, button_up, button_down, button_center);
}

void get_polar_coord(polar_coord_three_dims_t *polar_coord_three_dims)
{
    *polar_coord_three_dims = viewer_coord;
}