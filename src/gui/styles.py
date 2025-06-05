# Colors
COLORS = {
    "primary": "#2ecc71",
    "primary_dark": "#27ae60",
    "danger": "#e74c3c",
    "text": "#2c3e50",
    "border": "#dcdde1",
    "background": "#f5f6fa",
    "white": "white"
}

MAIN_STYLE = f"""
    QMainWindow {{
        background-color: {COLORS["background"]};
    }}
    
    QLabel {{
        color: {COLORS["text"]};
        font-size: 35px;
    }}
"""

SENSOR_CARD_STYLE = """
    QFrame {
        border: 1px solid #e1e1e1;
        background-color: white;
    }
"""
#list station 
STATION_CARD_STYLE = f"""
    QFrame {{
        background-color: {COLORS["white"]};
        border-radius: 10px;
        padding: 15px;
        margin: 5px;
    }}
    
    QLabel {{
        font-size: 16px;
        color: {COLORS["text"]};
    }}
    
    QLineEdit {{
        border: 1px solid {COLORS["border"]};
        border-radius: 5px;
        padding: 5px;
        font-size: 12px;
        width: 60px;
    }}
    
    QPushButton {{
        background-color: {COLORS["primary"]};
        color: {COLORS["white"]};
        border: none;
        border-radius: 5px;
        padding: 5px 10px;
        font-weight: bold;
    }}
    
    QPushButton:hover {{
        background-color: {COLORS["primary_dark"]};
    }}
"""

GRAPH_FRAME_STYLE = f"""
    QFrame {{
        background-color: {COLORS["white"]};
        border-radius: 10px;
        padding: 10px;
    }}
"""
#information status system
STATUS_FRAME_STYLE = f"""
    QFrame {{
        background-color: {COLORS["white"]};
        border-radius: 20px;
        padding: 5px;
    }}
    QLabel {{
        color: {COLORS["text"]};
        font-size: 11px; 
    }}
"""

BATTERY_STYLE = {
    "normal": f"""
        QProgressBar {{
            border: 2px solid {COLORS["border"]};
            border-radius: 5px;
            text-align: center;
            height: 20px;
            margin-left: 23px;
        }}
        QProgressBar::chunk {{
            background-color: {COLORS["primary"]};
            border-radius: 3px;
        }}
    """,
    "low": f"""
        QProgressBar {{
            border: 2px solid {COLORS["border"]};
            border-radius: 5px;
            text-align: center;
            height: 20px;  /* Giữ chiều cao nhỏ cho pin thấp */
            margin-left: 23px;
        }}
        QProgressBar::chunk {{
            background-color: {COLORS["danger"]};  /* Đỏ nếu pin thấp */
            border-radius: 3px;
        }}
    """
}

BATTERY_TEXT_STYLE = {
    "normal": f"color: {COLORS['primary']}; font-weight: bold;",
    "low": f"color: {COLORS['danger']}; font-weight: bold;"
}

# Thêm style mới cho trạng thái loa
SPEAKER_STATUS_STYLE = {
    "on": f"color: {COLORS['primary']}; font-weight: bold;",
    "off": f"color: {COLORS['text']}; font-weight: bold;"
}

SEPARATOR_STYLE = f"background-color: {COLORS['border']};"
