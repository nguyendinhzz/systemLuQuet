// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.4.2
// LVGL version: 8.3.11
// Project name: SquareLine_Project

#include "ui.h"
#include "ui_helpers.h"
#include <HTTPClient.h>
///////////////////// VARIABLES ////////////////////

//infor Login
extern String tokenAccess;
const char *accLogin="admin";
const char *passLogin="12345";
int screenFrom=1;
// SCREEN: ui_Screen1
void ui_Screen1_screen_init(void);
lv_obj_t * ui_Screen1;
lv_obj_t * ui_Label1;
lv_obj_t * ui_Label2;
void ui_event_TextArea1(lv_event_t * e);
lv_obj_t * ui_TextArea1;
void ui_event_TextArea2(lv_event_t * e);
lv_obj_t * ui_TextArea2;
void ui_event_Button1(lv_event_t * e);
lv_obj_t * ui_Button1;
lv_obj_t * ui_Label3;
void ui_event_Button5(lv_event_t * e);
lv_obj_t * ui_Button5;
void ui_event_Label10(lv_event_t * e);
lv_obj_t * ui_Label10;
lv_obj_t * ui_Label26;
lv_obj_t * ui_Label30;
lv_obj_t * ui_Keyboard1;

// SCREEN: ui_Screen2
void ui_Screen2_screen_init(void);
lv_obj_t * ui_Screen2;
lv_obj_t * ui_Label5;
lv_obj_t * ui_Container1;
void ui_event_Button10(lv_event_t * e);
lv_obj_t * ui_Button10;
void ui_event_Button11(lv_event_t * e);
lv_obj_t * ui_Button11;
void ui_event_Button12(lv_event_t * e);
lv_obj_t * ui_Button12;
void ui_event_Button13(lv_event_t * e);
lv_obj_t * ui_Button13;
lv_obj_t * ui_Container3;
lv_obj_t * ui_Label9;
lv_obj_t * ui_Label4;
lv_obj_t * ui_Label6;
lv_obj_t * ui_Label7;
void ui_event_Button2(lv_event_t * e);
lv_obj_t * ui_Button2;


// SCREEN: ui_Screen3
void ui_Screen3_screen_init(void);
void ui_event_Screen3(lv_event_t * e);
lv_obj_t * ui_Screen3;
lv_obj_t * ui_Table1;
lv_obj_t * ui_id;
lv_obj_t * ui_name;
lv_obj_t * ui_value;
lv_obj_t * ui_state;
void ui_event_Button3(lv_event_t * e);
lv_obj_t * ui_Button3;
lv_obj_t * ui_Label8;


// SCREEN: ui_Screen4
void ui_Screen4_screen_init(void);
void ui_event_Screen4(lv_event_t * e);
lv_obj_t * ui_Screen4;
void ui_event_Button7(lv_event_t * e);
lv_obj_t * ui_Button7;
lv_obj_t * ui_Dropdown3;
lv_obj_t * ui_Label11;
lv_obj_t * ui_Table2;
lv_obj_t * ui_idS;
lv_obj_t * ui_deviceCS;
lv_obj_t * ui_statieS;
void ui_event_Button6(lv_event_t * e);
lv_obj_t * ui_Button6;
lv_obj_t * ui_Label13;
void ui_event_Button8(lv_event_t * e);
lv_obj_t * ui_Button8;
void ui_event_Label14(lv_event_t * e);
lv_obj_t * ui_Label14;
void ui_event_Button9(lv_event_t * e);
lv_obj_t * ui_Button9;
lv_obj_t * ui_Label15;


// SCREEN: ui_Screen5
void ui_Screen5_screen_init(void);
void ui_event_Screen5(lv_event_t * e);
lv_obj_t * ui_Screen5;
void ui_event_Button14(lv_event_t * e);
lv_obj_t * ui_Button14;
lv_obj_t * ui_Dropdown5;
lv_obj_t * ui_Label12;
void ui_event_Button15(lv_event_t * e);
lv_obj_t * ui_Button15;
lv_obj_t * ui_Label17;
void ui_event_TextArea3(lv_event_t * e);
lv_obj_t * ui_TextArea3;
lv_obj_t * ui_Label16;
lv_obj_t * ui_Label18;
lv_obj_t * ui_Label19;
void ui_event_TextArea4(lv_event_t * e);
lv_obj_t * ui_TextArea4;
void ui_event_TextArea5(lv_event_t * e);
lv_obj_t * ui_TextArea5;
void ui_event_Button16(lv_event_t * e);
lv_obj_t * ui_Button16;
lv_obj_t * ui_Label21;
void ui_event_Button4(lv_event_t * e);
lv_obj_t * ui_Button4;
void ui_event_Label20(lv_event_t * e);
lv_obj_t * ui_Label20;
lv_obj_t * ui_Keyboard2;
lv_obj_t * ui_Label29;


// SCREEN: ui_Screen6
void ui_Screen6_screen_init(void);
void ui_event_Screen6(lv_event_t * e);
lv_obj_t * ui_Screen6;
void ui_event_Button18(lv_event_t * e);
lv_obj_t * ui_Button18;
lv_obj_t * ui_Container2;
lv_obj_t * ui_Label22;
lv_obj_t * ui_Label23;
lv_obj_t * ui_Label24;
lv_obj_t * ui_Label25;
void ui_event_Button19(lv_event_t * e);
lv_obj_t * ui_Button19;
lv_obj_t * ui_Label27;
void ui_event_Button20(lv_event_t * e);
lv_obj_t * ui_Button20;
lv_obj_t * ui_Label28;
void ui_event_TextArea6(lv_event_t * e);
lv_obj_t * ui_TextArea6;
void ui_event_TextArea7(lv_event_t * e);
lv_obj_t * ui_TextArea7;
lv_obj_t * ui_Label31;
lv_obj_t * ui_Keyboard3;
lv_obj_t * ui____initial_actions0;

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=0
    #error "LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////
void ui_event_TextArea1(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_FOCUSED) {
        _ui_flag_modify(ui_Keyboard1, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
        _ui_keyboard_set_target(ui_Keyboard1,  ui_TextArea1);
        _ui_flag_modify(ui_Button1, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
        _ui_flag_modify(ui_Button5, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
    }
    if(event_code == LV_EVENT_DEFOCUSED) {
        _ui_flag_modify(ui_Keyboard1, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
        _ui_flag_modify(ui_Button1, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
        _ui_flag_modify(ui_Button5, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
    }
}
void ui_event_TextArea2(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_FOCUSED) {
        _ui_flag_modify(ui_Keyboard1, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
        _ui_keyboard_set_target(ui_Keyboard1,  ui_TextArea2);
        _ui_flag_modify(ui_Button1, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
        _ui_flag_modify(ui_Button5, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
    }
    if(event_code == LV_EVENT_DEFOCUSED) {
        _ui_flag_modify(ui_Keyboard1, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
        _ui_flag_modify(ui_Button1, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
        _ui_flag_modify(ui_Button5, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
    }
}
bool checkLogin(const char *acc, const char *pass) {
    HTTPClient http;
    http.begin("http://192.168.100.121:3000/login");
    http.addHeader("Content-Type", "application/json");

    String postData = "{\"username\":\"" + String(acc) + "\",\"password\":\"" + String(pass) + "\"}";

    int httpResponseCode = http.POST(postData);

    if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.println("Response:");
        tokenAccess=response;
        Serial.println(response);
        return true;
    } else {
        Serial.print("Error on sending POST: ");
        Serial.println(httpResponseCode);
        return false;
    }
    http.end();
}

void ui_event_Button1(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        const char * enteredAcc = lv_textarea_get_text(ui_TextArea1);
        const char * enteredPass = lv_textarea_get_text(ui_TextArea2);
        if (WiFi.status() == WL_CONNECTED) {
            // checkLogin(enteredAcc,enteredPass);
            _ui_screen_change(&ui_Screen2, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_Screen2_screen_init);
            lv_label_set_text(ui_Label30, "");
        }
        else{
            if(strcmp(enteredAcc, accLogin)==0&& strcmp(enteredPass, passLogin) == 0){
                _ui_screen_change(&ui_Screen2, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_Screen2_screen_init);
                lv_label_set_text(ui_Label30, "");
            }
            else lv_label_set_text(ui_Label30, "Login failed!");
        }
    }
}
void ui_event_Button5(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        screenFrom=1;
        _ui_screen_change(&ui_Screen3, LV_SCR_LOAD_ANIM_MOVE_BOTTOM, 500, 0, &ui_Screen3_screen_init);
    }
}
void ui_event_Label10(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_Screen3, LV_SCR_LOAD_ANIM_MOVE_TOP, 500, 0, &ui_Screen3_screen_init);
    }
}
void ui_event_Button10(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        screenFrom=2;
        _ui_screen_change(&ui_Screen3, LV_SCR_LOAD_ANIM_MOVE_BOTTOM, 500, 0, &ui_Screen3_screen_init);
    }
}
void ui_event_Button11(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_Screen4, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_Screen4_screen_init);
    }
}
void ui_event_Button12(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_Screen5, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_Screen5_screen_init);
    }
}
void ui_event_Button13(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_Screen6, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_Screen6_screen_init);
    }
}
void ui_event_Button2(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_Screen1, LV_SCR_LOAD_ANIM_OVER_RIGHT, 500, 0, &ui_Screen1_screen_init);
    }
}
void ui_event_Screen3(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_SCREEN_LOADED) {
        loadScreen3(e);
    }
}
void ui_event_Button3(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        if(screenFrom==1)
        _ui_screen_change(&ui_Screen1, LV_SCR_LOAD_ANIM_MOVE_TOP, 500, 0, &ui_Screen1_screen_init);
        else if(screenFrom==2)
        _ui_screen_change(&ui_Screen2, LV_SCR_LOAD_ANIM_MOVE_TOP, 500, 0, &ui_Screen2_screen_init);
    }
}
void ui_event_Screen4(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_SCREEN_LOADED) {
        loadScreen4(e);
    }
}

char *idNodeSelect = "";
int32_t selectedRow = -1;  // Global variable to store the selected row
// add selected
void table_event_handler(lv_event_t * e) {
    lv_obj_t * table = lv_event_get_target(e);  // Lấy đối tượng bảng từ sự kiện

    // Lấy tọa độ của điểm nhấn (click)
    lv_point_t point;
    lv_indev_get_point(lv_indev_get_act(), &point); 

    // Xác định vị trí của bảng
    lv_area_t table_area;
    lv_obj_get_coords(table, &table_area);

    // Tính chiều cao của từng dòng và phần tiêu đề
    int row_height = lv_font_get_line_height(lv_obj_get_style_text_font(table, LV_PART_MAIN | LV_STATE_DEFAULT));
    int header_height = row_height*2;  // Chiều cao tiêu đề (dòng đầu tiên)

    // Tính toán dòng dựa trên tọa độ click, trừ đi phần tiêu đề
    int row = (point.y - table_area.y1 - header_height) / row_height + 1;  // +1 để bỏ qua dòng tiêu đề

    // Kiểm tra nếu dòng hợp lệ (tránh nhầm lẫn với dòng tiêu đề và ngoài phạm vi bảng)
    if(row > 0 && row < lv_table_get_row_cnt(table)) {
        idNodeSelect = (char *)lv_table_get_cell_value(table, row, 0);  // Lấy giá trị ID từ cột đầu tiên (cột 0) và lưu vào idNodeSelect
        Serial.println(idNodeSelect);  // Xuất giá trị idNode ra Serial để kiểm tra
    }
}

void ui_event_Button7(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_Screen2, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_Screen2_screen_init);
    }
}
void ui_event_Button6(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        addStation(e);
    }
}
void ui_event_Button8(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_Screen4, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_Screen4_screen_init);
        loadScreen4(e);
    }
}
void ui_event_Label14(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        (e);
    }
}
void ui_event_Button9(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        delStation(e);
    }
}
void ui_event_Screen5(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_SCREEN_LOADED) {
        screenload5(e);
    }
}
void ui_event_Button14(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_Screen2, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_Screen2_screen_init);
    }
}
void ui_event_Button15(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        loadValues(e);
    }
}
void ui_event_TextArea3(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_FOCUSED) {
        _ui_keyboard_set_target(ui_Keyboard2,  ui_TextArea3);
        _ui_flag_modify(ui_Keyboard2, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
    }
    if(event_code == LV_EVENT_DEFOCUSED) {
        _ui_flag_modify(ui_Keyboard2, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
    }
}
void ui_event_TextArea4(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_FOCUSED) {
        _ui_flag_modify(ui_Keyboard2, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
        _ui_keyboard_set_target(ui_Keyboard2,  ui_TextArea4);
    }
    if(event_code == LV_EVENT_DEFOCUSED) {
        _ui_flag_modify(ui_Keyboard2, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
    }
}
void ui_event_TextArea5(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_FOCUSED) {
        _ui_flag_modify(ui_Keyboard2, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
        _ui_flag_modify(ui_TextArea3, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
        _ui_flag_modify(ui_Label16, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
        _ui_basic_set_property(ui_Label19, _UI_BASIC_PROPERTY_POSITION_Y,  -48);
        _ui_basic_set_property(ui_TextArea5, _UI_BASIC_PROPERTY_POSITION_Y,  -48);
        _ui_keyboard_set_target(ui_Keyboard2,  ui_TextArea5);
    }
    if(event_code == LV_EVENT_DEFOCUSED) {
        _ui_flag_modify(ui_Keyboard2, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
        _ui_flag_modify(ui_Label16, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
        _ui_flag_modify(ui_TextArea3, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
        _ui_basic_set_property(ui_Label19, _UI_BASIC_PROPERTY_POSITION_Y,  26);
        _ui_basic_set_property(ui_TextArea5, _UI_BASIC_PROPERTY_POSITION_Y,  26);
    }
}
void ui_event_Button16(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        changeValues(e);
    }
}
void ui_event_Button4(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_Screen2, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_Screen2_screen_init);
    }
}
void ui_event_Label20(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_Screen1, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_Screen1_screen_init);
    }
}
void ui_event_Screen6(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_SCREEN_LOADED) {
        screenLoad6(e);
    }
}
void ui_event_Button18(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_Screen2, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_Screen2_screen_init);
    }
}
void ui_event_Button19(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        configWifi(e);
    }
}
void ui_event_Button20(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_Screen2, LV_SCR_LOAD_ANIM_NONE, 500, 0, &ui_Screen2_screen_init);
    }
}
void ui_event_TextArea6(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_FOCUSED) {
        _ui_flag_modify(ui_Keyboard3, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
        _ui_keyboard_set_target(ui_Keyboard3,  ui_TextArea6);
    }
    if(event_code == LV_EVENT_DEFOCUSED) {
        _ui_flag_modify(ui_Keyboard3, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
    }
}
void ui_event_TextArea7(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_FOCUSED) {
        _ui_flag_modify(ui_Keyboard3, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
        _ui_keyboard_set_target(ui_Keyboard3,  ui_TextArea7);
    }
    if(event_code == LV_EVENT_DEFOCUSED) {
        _ui_flag_modify(ui_Keyboard3, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
    }
}

///////////////////// SCREENS ////////////////////

void ui_init(void)
{
    lv_disp_t * dispp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                               false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    ui_Screen1_screen_init();
    ui_Screen2_screen_init();
    ui_Screen3_screen_init();
    ui_Screen4_screen_init();
    ui_Screen5_screen_init();
    ui_Screen6_screen_init();
    ui____initial_actions0 = lv_obj_create(NULL);
    lv_disp_load_scr(ui_Screen1);
}
