#include "aya_common_UI.h"

int debug_var_i[10];
float debug_var_f[10];

ui_instance_t UI;

int a, b, c, d;
infoline_t ui_database[] =
    {
        /*样例：{"名称"，数据指针,数据类型，是否允许编辑，编辑步长}
         *       名称：          长度20以内的ASCII字符串
         *       数据指针：      将所需数据extern后取地址
         *       数据类型：      INFOLINE_INT-整数           INFOLINE_FLOAT-浮点数
         *       是否允许编辑：  INFOLINE_EDITABLE-可编辑    INFOLINE_UNEDITABLE-不可编辑，仅显示
         *       编辑步长：      每次左右拨动五向开关数值的变动步长
         */
        //  {"ECD1", &Encoder.encoder1, INFOLINE_INT, INFOLINE_UNEDITABLE},
        //  {"ECD2", &Encoder.encoder2, INFOLINE_INT, INFOLINE_UNEDITABLE},
        //  {"ECD3", &Encoder.encoder3, INFOLINE_INT, INFOLINE_UNEDITABLE},
        //  {"ECD4", &Encoder.encoder4, INFOLINE_INT, INFOLINE_UNEDITABLE},

        //{"ACCX", &BMI.acc_X, INFOLINE_FLOAT, INFOLINE_UNEDITABLE},
        //{"ACCY", &BMI.acc_Y, INFOLINE_FLOAT, INFOLINE_UNEDITABLE},
        //{"ACCZ", &BMI.acc_Z, INFOLINE_FLOAT, INFOLINE_UNEDITABLE},
        //{"YAW", &BMI.yaw, INFOLINE_FLOAT, INFOLINE_UNEDITABLE},

        {"a", &a, INFOLINE_INT, INFOLINE_EDITABLE, 1},
        {"b", &b, INFOLINE_INT, INFOLINE_EDITABLE, 2},
        {"c", &c, INFOLINE_INT, INFOLINE_EDITABLE, 3},
        {"d", &d, INFOLINE_INT, INFOLINE_EDITABLE, 4},

};

UIstatus_t ui_status =
    {
        .line = 0,
        .page = 0,
        .line_in_page = 0, // 0..14
};

static void ui_page_to_line()
{
#ifdef UI_FONTSIZE_6x8
    ui_status.line = ui_status.line_in_page + ui_status.page * UI.page_volume;
#elif UI_FONSIZE_8x16
    ui_status.line = ui_status.line_in_page + ui_status.page * UI.page_volume;
#endif
}

static void ui_line_to_page()
{
#ifdef UI_FONTSIZE_6x8
    ui_status.page = ui_status.line / UI.page_volume;
    ui_status.line_in_page = ui_status.line % UI.page_volume;
    ui_status.ls_line = ui_status.line - ui_status.line_in_page;
    ui_status.ms_line = _min(ui_status.ls_line + UI.page_volume - 1, UI.max_line_number);
#elif UI_FONSIZE_8x16
    ui_status.page = ui_status.line / UI.page_volume;
    ui_status.line_in_page = ui_status.line % UI.page_volume;
    ui_status.ls_line = ui_status.line - ui_status.line_in_page;
    ui_status.ms_line = _min(ui_status.ls_line + UI.page_volume - 1, UI.max_line_number);
#endif
}

/**
 * @brief 下调选中的行
 *
 */
static void ui_line_add()
{
    ui_status.line = (ui_status.line + 1) % UI.max_line_number;
    ui_line_to_page();
}

/**
 * @brief 上调选中的行
 *
 */
static void ui_line_sub()
{
    ui_status.line = (ui_status.line - 1 + UI.max_line_number) % UI.max_line_number;
    ui_line_to_page();
}

/**
 * @brief 选中的值增加
 *
 */
static void ui_value_add()
{
    infoline_type_enum current_line_type = ui_database[ui_status.line].type;
    if (ui_database[ui_status.line].chmod == INFOLINE_UNEDITABLE)
        return;
    if (current_line_type == INFOLINE_INT)
        *(int *)(ui_database[ui_status.line].num) += (int)ui_database[ui_status.line].step;

    if (current_line_type == INFOLINE_FLOAT)
        *(float *)(ui_database[ui_status.line].num) += (float)ui_database[ui_status.line].step;
}

/**
 * @brief 选中的值减少
 *
 */
static void ui_value_sub()
{
    infoline_type_enum current_line_type = ui_database[ui_status.line].type;
    if (ui_database[ui_status.line].chmod == INFOLINE_UNEDITABLE)
        return;
    if (current_line_type == INFOLINE_INT)
        *(int *)(ui_database[ui_status.line].num) -= (int)ui_database[ui_status.line].step;

    if (current_line_type == INFOLINE_FLOAT)
        *(float *)(ui_database[ui_status.line].num) -= (float)ui_database[ui_status.line].step;
}

/**
 * @brief UI按键响应
 *
 */
static void ui_input_respond()
{
    if (key_read(KEY_DOWN))
        ui_line_add();
    if (key_read(KEY_UP))
        ui_line_sub();
    if (key_read(KEY_LEFT))
        ui_value_sub();
    if (key_read(KEY_RIGHT))
        ui_value_add();
    // if (key_poll_C())
}

/**
 * @brief UI渲染器
 *
 */
static void ui_shader()
{
    // 第一行显示页码
    tft180_show_int(0, 0, ui_status.page + 1, 2);
    tft180_show_int(30, 0, ui_status.line + 1, 2);

    // 显示内容
    for (int i = 1; i <= UI.page_volume; i++)
    {
        // 该行无内容显示
        if (ui_status.ls_line + i - 1 >= UI.max_line_number)
        {
            tft180_fill_block(0, i * UI.font_height, 159, (i + 1) * UI.font_height - 1, tft180_bgcolor);
            continue;
        }

        // 该行被选中
        if (ui_status.line_in_page == i - 1)
        {
            tft180_show_string_color(0, i * UI.font_height, ui_database[ui_status.ls_line + i - 1].s, tft180_bgcolor, tft180_pencolor);

            if (ui_database[ui_status.ls_line + i - 1].type == INFOLINE_INT)
                tft180_show_int_color(88, i * UI.font_height, *(int *)ui_database[ui_status.ls_line + i - 1].num, 8, tft180_bgcolor, tft180_pencolor);
            if (ui_database[ui_status.ls_line + i - 1].type == INFOLINE_FLOAT)
                tft180_show_float_color(88, i * UI.font_height, *(float *)ui_database[ui_status.ls_line + i - 1].num, 8, 2, tft180_bgcolor, tft180_pencolor);
        }
        // 该行未被选中
        else
        {
            tft180_show_string_color(0, i * UI.font_height, ui_database[ui_status.ls_line + i - 1].s, tft180_pencolor, tft180_bgcolor);

            if (ui_database[ui_status.ls_line + i - 1].type == INFOLINE_INT)
                tft180_show_int_color(88, i * UI.font_height, *(int *)ui_database[ui_status.ls_line + i - 1].num, 8, tft180_pencolor, tft180_bgcolor);
            if (ui_database[ui_status.ls_line + i - 1].type == INFOLINE_FLOAT)
                tft180_show_float_color(88, i * UI.font_height, *(float *)ui_database[ui_status.ls_line + i - 1].num, 8, 2, tft180_pencolor, tft180_bgcolor);
        }
    }
}

/**
 * @brief UI初始化
 *
 */
void ui_init()
{
#ifdef UI_FONTSIZE_6x8
    UI.page_volume = 14;
    UI.font_height = 8;
#elif UI_FONTSIZE_8x16
    UI.page_volume = 7;
    UI.font_height = 16;
#endif
    UI.max_line_number = sizeof(ui_database) / sizeof(ui_database[0]);
    ui_status.ls_line = 0, ui_status.ms_line = _min(UI.page_volume, UI.max_line_number);
}

/**
 * @brief UI线程
 *
 */
void ui_handler()
{
    ui_input_respond();
    ui_shader();
}
