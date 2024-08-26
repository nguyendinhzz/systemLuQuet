
#include "ui.h"
#include "DeviceManager.h"

extern DeviceManager deviceManager;

void ui_Screen3_screen_init(void)
{
    ui_Screen3 = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Screen3, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Table1 = lv_table_create(ui_Screen3);
    lv_obj_set_size(ui_Table1, 320, 240); // Set table size to match the screen size
    lv_obj_align(ui_Table1, LV_ALIGN_TOP_MID, 0, 0); // Align table at the top
    lv_obj_set_style_pad_ver(ui_Table1, 15, LV_PART_ITEMS);
    lv_obj_set_style_pad_hor(ui_Table1, 0, LV_PART_ITEMS);
    // lv_obj_set_style_text_font(ui_Table1, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Set the number of columns and rows
    lv_table_set_col_cnt(ui_Table1, 5);  // 5 columns: ID, Name, Value, Unit, State
    lv_table_set_row_cnt(ui_Table1, deviceManager.getDevices().size() + 1); // Add 1 for header

    // Set column widths to fill the screen width (320px total)
    lv_table_set_col_width(ui_Table1, 0, 42);   // ID column
    lv_table_set_col_width(ui_Table1, 1, 98);  // Name column
    lv_table_set_col_width(ui_Table1, 2, 60);   // Value column
    lv_table_set_col_width(ui_Table1, 3, 55);   // Unit column
    lv_table_set_col_width(ui_Table1, 4, 65);   // State column

    // Set table column titles
    lv_table_set_cell_value(ui_Table1, 0, 0, "ID");
    lv_table_set_cell_value(ui_Table1, 0, 1, "Name");
    lv_table_set_cell_value(ui_Table1, 0, 2, "Value");
    lv_table_set_cell_value(ui_Table1, 0, 3, "Unit");
    lv_table_set_cell_value(ui_Table1, 0, 4, "State");

    int row = 1;
    for (const auto& device : deviceManager.getDevices()) {
         // Show only last 2 characters
        lv_table_set_cell_value(ui_Table1, row, 0, device.id.c_str());
        lv_table_set_cell_value(ui_Table1, row, 1, device.name.c_str());
        lv_table_set_cell_value(ui_Table1, row, 2, String(device.parameters).c_str());
        lv_table_set_cell_value(ui_Table1, row, 3, device.unit.c_str()); 
        lv_table_set_cell_value(ui_Table1, row, 4, device.status.c_str());
        row++;
    }

    // Create Back Button
    ui_Button3 = lv_btn_create(ui_Screen3);
    lv_obj_set_size(ui_Button3, 65, 25);
    lv_obj_align(ui_Button3, LV_ALIGN_BOTTOM_MID, 0, -10);

    ui_Label8 = lv_label_create(ui_Button3);
    lv_label_set_text(ui_Label8, "Back");
    lv_obj_center(ui_Label8);

    lv_obj_add_event_cb(ui_Button3, ui_event_Button3, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Screen3, ui_event_Screen3, LV_EVENT_ALL, NULL);
}
