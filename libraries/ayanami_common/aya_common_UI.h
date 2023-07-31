#ifndef __AYA_COMMON_UI_H
#define __AYA_COMMON_UI_H

#include "aya_device_head.h"
#include "aya_driver_head.h"

#include "aya_device_lcd180.h"

#define UI_FONTSIZE_6x8 0
// #define UI_FONTSIZE_8x16  0

typedef enum
{
    INFOLINE_INT,   // ��������
    INFOLINE_FLOAT, // ��������
} infoline_type_enum;

typedef enum
{
    INFOLINE_UNEDITABLE, // ���ɱ༭
    INFOLINE_EDITABLE    // ���Ա༭
} infoline_chmod_enum;

typedef struct
{
    char s[20];                // ��Ŀ����
    void *num;                 // ���ݵ�ַ
    infoline_type_enum type;   // ��������(����/����)
    infoline_chmod_enum chmod; // �༭Ȩ��
    double step;               // �༭����
} infoline_t;

typedef struct
{
    int page,         // ��ǰ����ҳ��
        line_in_page, // ��ǰ����ҳ���е�����
        line,         // ��ǰ����������
        ms_line,      // ��ǰҳ������С����
        ls_line;      // ��ǰҳ����������
} UIstatus_t;


typedef struct
{
    int max_line_number;
    int page_volume;
    int font_height;  // �ָ�(px)
} ui_instance_t;


void ui_init();
void ui_handler();


#endif