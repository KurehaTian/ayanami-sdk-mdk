#ifndef __AYA_COMMON_UI_H
#define __AYA_COMMON_UI_H

#include "aya_device_head.h"
#include "aya_driver_head.h"

#include "aya_device_lcd180.h"

#define UI_FONTSIZE_6x8 0
// #define UI_FONTSIZE_8x16  0

typedef enum
{
    INFOLINE_INT,   // 整数类型
    INFOLINE_FLOAT, // 浮点类型
} infoline_type_enum;

typedef enum
{
    INFOLINE_UNEDITABLE, // 不可编辑
    INFOLINE_EDITABLE    // 可以编辑
} infoline_chmod_enum;

typedef struct
{
    char s[20];                // 条目标题
    void *num;                 // 数据地址
    infoline_type_enum type;   // 数据类型(整数/浮点)
    infoline_chmod_enum chmod; // 编辑权限
    double step;               // 编辑步长
} infoline_t;

typedef struct
{
    int page,         // 当前所在页数
        line_in_page, // 当前所在页数中的行数
        line,         // 当前所在总行数
        ms_line,      // 当前页面中最小的行
        ls_line;      // 当前页面中最大的行
} UIstatus_t;


typedef struct
{
    int max_line_number;
    int page_volume;
    int font_height;  // 字高(px)
} ui_instance_t;


void ui_init();
void ui_handler();


#endif