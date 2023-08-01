#include "aya_common_UI.h"

int debug_var_i[10];
float debug_var_f[10];

ui_instance_t UI;

int a, b, c, d;
infoline_t ui_database[] =
    {
        /*������{"����"������ָ��,�������ͣ��Ƿ�����༭���༭����}
         *       ���ƣ�          ����20���ڵ�ASCII�ַ���
         *       ����ָ�룺      ����������extern��ȡ��ַ
         *       �������ͣ�      INFOLINE_INT-����           INFOLINE_FLOAT-������
         *       �Ƿ�����༭��  INFOLINE_EDITABLE-�ɱ༭    INFOLINE_UNEDITABLE-���ɱ༭������ʾ
         *       �༭������      ÿ�����Ҳ������򿪹���ֵ�ı䶯����
         */

        {"speed", &motor.speed_center, INFOLINE_INT, INFOLINE_EDITABLE, 10},
        //{"EN", &drv_8701_ins.idle, INFOLINE_INT, INFOLINE_EDITABLE, 1},
        //{"speed_R", &motor.speed_exp[1], INFOLINE_INT, INFOLINE_EDITABLE, 10},
        //{"p", &motor.pid_left_wheel.kp, INFOLINE_FLOAT, INFOLINE_EDITABLE, 1},
        //{"i", &motor.pid_left_wheel.ki, INFOLINE_FLOAT, INFOLINE_EDITABLE, 0.1},
        //{"d", &motor.pid_left_wheel.kd, INFOLINE_FLOAT, INFOLINE_EDITABLE, 1},
        {"p", &steering.kp, INFOLINE_FLOAT, INFOLINE_EDITABLE, 0.02},
        {"i", &steering.ki, INFOLINE_FLOAT, INFOLINE_EDITABLE, 0.01},
        {"d", &steering.kd, INFOLINE_FLOAT, INFOLINE_EDITABLE, 0.01},
        {"YAW", &bmi088_data.yaw, INFOLINE_FLOAT, INFOLINE_UNEDITABLE},
        {"ROL", &bmi088_data.roll, INFOLINE_FLOAT, INFOLINE_UNEDITABLE},
        {"PIT", &bmi088_data.pitch, INFOLINE_FLOAT, INFOLINE_UNEDITABLE},
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
 * @brief �µ�ѡ�е���
 *
 */
static void ui_line_add()
{
    ui_status.line = (ui_status.line + 1) % UI.max_line_number;
    ui_line_to_page();
}

/**
 * @brief �ϵ�ѡ�е���
 *
 */
static void ui_line_sub()
{
    ui_status.line = (ui_status.line - 1 + UI.max_line_number) % UI.max_line_number;
    ui_line_to_page();
}

/**
 * @brief ѡ�е�ֵ����
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
 * @brief ѡ�е�ֵ����
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
 * @brief UI������Ӧ
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
    if (key_read(KEY_SW1))
    {
        UI.en = 1 - UI.en;
        tft180_clear();
    }
    // if (key_poll_C())
}

/**
 * @brief UI��Ⱦ��
 *
 */
static void ui_shader()
{
    // ��һ����ʾҳ��
    tft180_show_int(0, 0, ui_status.page + 1, 2);
    tft180_show_int(30, 0, ui_status.line + 1, 2);

    // ��ʾ����
    for (int i = 1; i <= UI.page_volume; i++)
    {
        // ������������ʾ
        if (ui_status.ls_line + i - 1 >= UI.max_line_number)
        {
            tft180_fill_block(0, i * UI.font_height, 159, (i + 1) * UI.font_height - 1, tft180_bgcolor);
            continue;
        }

        // ���б�ѡ��
        if (ui_status.line_in_page == i - 1)
        {
            tft180_show_string_color(0, i * UI.font_height, ui_database[ui_status.ls_line + i - 1].s, tft180_bgcolor, tft180_pencolor);

            if (ui_database[ui_status.ls_line + i - 1].type == INFOLINE_INT)
                tft180_show_int_color(88, i * UI.font_height, *(int *)ui_database[ui_status.ls_line + i - 1].num, 8, tft180_bgcolor, tft180_pencolor);
            if (ui_database[ui_status.ls_line + i - 1].type == INFOLINE_FLOAT)
            {
                // tft180_show_string_color(88, i * UI.font_height, "            ", tft180_bgcolor, tft180_pencolor);
                tft180_show_float_color(88, i * UI.font_height, *(float *)ui_database[ui_status.ls_line + i - 1].num, 8, 2, tft180_bgcolor, tft180_pencolor);
            }
        }
        // ����δ��ѡ��
        else
        {
            tft180_show_string_color(0, i * UI.font_height, ui_database[ui_status.ls_line + i - 1].s, tft180_pencolor, tft180_bgcolor);

            if (ui_database[ui_status.ls_line + i - 1].type == INFOLINE_INT)
                tft180_show_int_color(88, i * UI.font_height, *(int *)ui_database[ui_status.ls_line + i - 1].num, 8, tft180_pencolor, tft180_bgcolor);
            if (ui_database[ui_status.ls_line + i - 1].type == INFOLINE_FLOAT)
            {
                // tft180_show_string_color(88, i * UI.font_height, "            ", tft180_pencolor, tft180_bgcolor);
                tft180_show_float_color(88, i * UI.font_height, *(float *)ui_database[ui_status.ls_line + i - 1].num, 8, 2, tft180_pencolor, tft180_bgcolor);
            }
        }
    }
}

/**
 * @brief UI��ʼ��
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
 * @brief UI�߳�
 *
 */
void ui_handler()
{
    ui_input_respond();
    if (UI.en)
        ui_shader();
}
