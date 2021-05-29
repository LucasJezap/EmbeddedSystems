#include "stm32f476g-disco-LJMS-flac_screen.h"

#include <memory.h>
#include <stdlib.h>

#include "Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_lcd.h"
#include "Drivers/BSP/STM32746G-Discovery/stm32746g_discovery_ts.h"

#define LCD_X_SIZE RK043FN48H_WIDTH
#define LCD_Y_SIZE RK043FN48H_HEIGHT

#define count(x) (sizeof(x) / sizeof(x[0]))

static uint8_t lcd_buffer_0[LCD_X_SIZE * LCD_Y_SIZE * 4] __attribute__((section(".sdram")));
static uint8_t lcd_buffer_1[LCD_X_SIZE * LCD_Y_SIZE * 4] __attribute__((section(".sdram")));

static int current_layer = 0;

static const Point first_usb_circle_position = {150, 200};
static const Point second_usb_circle_position = {240, 200};
static const Point third_usb_circle_position = {330, 200};
static const int small_circle_radius = 25;
static const int big_circle_radius = 42;

static const Point progress_bar_position = {17, 72};
static const Point progress_bar_size = {446, 10};
static const Point filename_position = {0, 100};
static const Point file_count_position = {0, 20};
static const Point current_timer_position = {-190, 20};
static const Point max_timer_position = {190, 20};

static const Point play_icon_position = {192, 145};
static const Point play_icon_circle_center = {48, 48};
static const int icon_play_pause_circle_radius = 40;
static const Point icon_play_points[] = {
    {43, 41},
    {53, 48},
    {43, 55}};
static const Point icon_pause_points[] = {
    {43, 43},
    {43, 53},
    {53, 53},
    {53, 43}};

static const Point previous_icon_position = {98, 200};
static const Point previous_icon_points_1[] = {
    {53, 10},
    {57, 10},
    {57, 32},
    {53, 32}};
static const Point previous_icon_points_2[] = {
    {60, 21},
    {75, 10},
    {75, 32}};

static const Point previous_icon_points_3[] = {
    {10, -8},
    {140, -8},
    {140, 50},
    {10, 50}};

static const Point next_icon_position = {382, 200};
static const Point next_icon_points_1[] = {
    {-53, 10},
    {-57, 10},
    {-57, 32},
    {-53, 32}};
static const Point next_icon_points_2[] = {
    {-60, 21},
    {-75, 10},
    {-75, 32}};
static const Point next_icon_points_3[] = {
    {-10, -8},
    {-140, -8},
    {-140, 50},
    {-10, 50}};

typedef struct
{
    const Point position;
    const Point size;
    bool is_touched;
    unsigned last_changed_at;
    bool was_touched_event;
} Button;

static Button button_play_pause = {
    .position = {192, 156},
    .size = {96, 96},
    .is_touched = false,
    .last_changed_at = 0};

static Button button_back = {
    .position = {98, 172},
    .size = {64, 64},
    .is_touched = false,
    .last_changed_at = 0};

static Button button_next = {
    .position = {318, 172},
    .size = {64, 64},
    .is_touched = false,
    .last_changed_at = 0};

/**
 * this function changes the current layer from 0 to 1 or from 1 to 0
 */
static void flip_layer(void)
{
    // wait for vsync
    while (!(LTDC->CDSR & LTDC_CDSR_VSYNCS))
    {
    }

    /* changing from layer 0 to 1 (1 to 0) */
    BSP_LCD_SetLayerVisible(current_layer, DISABLE);
    current_layer ^= 1;
    BSP_LCD_SetLayerVisible(current_layer, ENABLE);
    BSP_LCD_SelectLayer(current_layer ^ 1);
}

/**
 * this function initializes the microcontroller's screen
 */
void init_screen(void)
{
    /* Initialize LCD */
    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(0, (uint32_t)lcd_buffer_0);
    BSP_LCD_LayerDefaultInit(1, (uint32_t)lcd_buffer_1);

    /* clear two layers */
    BSP_LCD_SelectLayer(0);
    BSP_LCD_Clear(LCD_COLOR_WHITE);
    BSP_LCD_SelectLayer(1);
    BSP_LCD_Clear(LCD_COLOR_WHITE);

    current_layer = 0;
    flip_layer();

    /* Enable display */
    BSP_LCD_DisplayOn();

    /* Init the touch screen */
    BSP_TS_Init(LCD_X_SIZE, LCD_Y_SIZE);
}

/**
 * this function fills the polygon of given points
 */
static void fill_lcd_polygon(Point position, const Point *points, uint16_t point_count)
{
    Point *translated_points = malloc(point_count * sizeof(Point));
    for (int i = 0; i < point_count; i++)
    {
        translated_points[i] = (Point){
            .X = points[i].X + position.X,
            .Y = points[i].Y + position.Y};
    }
    BSP_LCD_FillPolygon(translated_points, point_count);
    free(translated_points);
}

/**
 * this function displays a text on the microcontroller's screen
 */
void display_info_on_screen(const char *info)
{
    BSP_LCD_Clear(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    /* display text in the middle */
    BSP_LCD_DisplayStringAt(0, LCD_Y_SIZE / 2, (uint8_t *)info, CENTER_MODE);

    flip_layer();
}

/**
 * this function displays a waiting screen while waiting for USB
 */
void render_usb(int which_big)
{
    BSP_LCD_Clear(LCD_COLOR_WHITE);
    BSP_LCD_DisplayStringAt(0, LCD_Y_SIZE / 3, (uint8_t *)"Waiting for USB...", CENTER_MODE);
    BSP_LCD_DrawCircle(first_usb_circle_position.X, first_usb_circle_position.Y, which_big == 0 ? big_circle_radius : small_circle_radius);
    BSP_LCD_DrawCircle(second_usb_circle_position.X, second_usb_circle_position.Y, which_big == 1 ? big_circle_radius : small_circle_radius);
    BSP_LCD_DrawCircle(third_usb_circle_position.X, third_usb_circle_position.Y, which_big == 2 ? big_circle_radius : small_circle_radius);
    flip_layer();
}

/**
 * this function is a main function rendering screen - it changes all visible things on the flac player (after USB was plugged)
 */
void render_flac_player(
    int files_count,
    int current_file,
    const char *current_filename,
    unsigned sample_rate,
    unsigned current_timer,
    unsigned max_timer,
    bool is_playing)
{
    BSP_LCD_Clear(LCD_COLOR_LIGHTGRAY);
    BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGRAY);

    // previous
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    fill_lcd_polygon(previous_icon_position, previous_icon_points_3, count(previous_icon_points_3));
    BSP_LCD_SetTextColor(button_back.is_touched ? LCD_COLOR_GRAY : LCD_COLOR_BLACK);
    fill_lcd_polygon(previous_icon_position, previous_icon_points_1, count(previous_icon_points_1));
    fill_lcd_polygon(previous_icon_position, previous_icon_points_2, count(previous_icon_points_2));

    // next
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    fill_lcd_polygon(next_icon_position, next_icon_points_3, count(next_icon_points_3));
    BSP_LCD_SetTextColor(button_next.is_touched ? LCD_COLOR_GRAY : LCD_COLOR_BLACK);
    fill_lcd_polygon(next_icon_position, next_icon_points_1, count(next_icon_points_1));
    fill_lcd_polygon(next_icon_position, next_icon_points_2, count(next_icon_points_2));

    // play / pause
    BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
    BSP_LCD_FillCircle((uint16_t)(play_icon_position.X + play_icon_circle_center.X),
                       (uint16_t)(play_icon_position.Y + play_icon_circle_center.Y),
                       (uint16_t)icon_play_pause_circle_radius + 5);
    BSP_LCD_SetTextColor(button_play_pause.is_touched ? LCD_COLOR_GREEN : LCD_COLOR_BLUE);
    BSP_LCD_FillCircle((uint16_t)(play_icon_position.X + play_icon_circle_center.X),
                       (uint16_t)(play_icon_position.Y + play_icon_circle_center.Y),
                       (uint16_t)icon_play_pause_circle_radius);

    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    if (is_playing)
    {
        fill_lcd_polygon(play_icon_position, icon_pause_points, count(icon_pause_points));
    }
    else
    {
        fill_lcd_polygon(play_icon_position, icon_play_points, count(icon_play_points));
    }

    // progress
    BSP_LCD_SetTextColor((uint32_t)0xFFe9f2ec);
    BSP_LCD_FillRect((uint16_t)progress_bar_position.X,
                     (uint16_t)progress_bar_position.Y,
                     (uint16_t)progress_bar_size.X,
                     (uint16_t)progress_bar_size.Y);

    unsigned played_seconds = current_timer / sample_rate;
    unsigned total_seconds = max_timer / sample_rate;
    unsigned played_minutes = played_seconds / 60;
    unsigned total_minutes = total_seconds / 60;
    played_seconds %= 60;
    total_seconds %= 60;
    char timer_current[20];
    char timer_total[20];
    sprintf(timer_current, "%s%u:%s%u", (played_minutes < 10) ? "0" : "", played_minutes, (played_seconds < 10) ? "0" : "", played_seconds);
    sprintf(timer_total, "%s%u:%s%u", (total_minutes < 10) ? "0" : "", total_minutes, (total_seconds < 10) ? "0" : "", total_seconds);

    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_DisplayStringAt(current_timer_position.X, current_timer_position.Y, (uint8_t *)timer_current, CENTER_MODE);
    BSP_LCD_DisplayStringAt(max_timer_position.X, max_timer_position.Y, (uint8_t *)timer_total, CENTER_MODE);

    if (1.0 * current_timer / max_timer > 0)
    {
        BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
        BSP_LCD_FillRect((uint16_t)progress_bar_position.X,
                         (uint16_t)progress_bar_position.Y,
                         (uint16_t)(progress_bar_size.X * 1.0 * current_timer / max_timer),
                         (uint16_t)progress_bar_size.Y);
    }

    if (current_filename)
    {
        BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
        BSP_LCD_DisplayStringAt(filename_position.X, filename_position.Y, (uint8_t *)current_filename, CENTER_MODE);
    }

    if (current_file != -1 && files_count != -1)
    {
        char text[32];
        snprintf(text, count(text), "%d/%d", current_file + 1, files_count);
        BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
        BSP_LCD_DisplayStringAt(file_count_position.X, file_count_position.Y, (uint8_t *)text, CENTER_MODE);
    }

    flip_layer();
}

/**
 * this function checks if point (position) was touched - it checks the point and it's neighbourhood
 */
static bool is_position_touched(const TS_StateTypeDef *touch_state, Point position, Point size)
{
    if (touch_state->touchDetected == 0)
        return false;

    unsigned x = touch_state->touchX[0];
    unsigned y = touch_state->touchY[0];

    return x >= position.X &&
           x <= position.X + size.X &&
           y >= position.Y &&
           y <= position.Y + size.Y;
}

/**
 * this function handles the situation when button was touched
 */
void handle_touch(void)
{
    unsigned time = osKernelSysTick();
    TS_StateTypeDef touch_state;
    BSP_TS_GetState(&touch_state);

    Button *buttons[] = {&button_back, &button_next, &button_play_pause, NULL};
    for (int i = 0; buttons[i]; i++)
    {
        Button *button = buttons[i];

        bool is_touched = is_position_touched(&touch_state, button->position, button->size);
        if (is_touched != button->is_touched)
        {
            if (time - button->last_changed_at >= 100)
            {
                if (!button->is_touched && is_touched)
                {
                    button->was_touched_event = true;
                }
                button->is_touched = is_touched;
            }
            button->last_changed_at = time;
        }
    }
}

/**
 * this function checks if previous song button was touched
 */
bool is_back_button_touched(void)
{
    bool val = button_back.was_touched_event;
    button_back.was_touched_event = false;
    return val;
}

/**
 * this function checks if play/pause song button was touched
 */
bool is_play_button_touched(void)
{
    bool val = button_play_pause.was_touched_event;
    button_play_pause.was_touched_event = false;
    return val;
}

/**
 * this function checks if next song button was touched
 */
bool is_next_button_touched(void)
{
    bool val = button_next.was_touched_event;
    button_next.was_touched_event = false;
    return val;
}
