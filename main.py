import sys
from PyQt5.QtWidgets import QApplication
from src.gui.main_window import FloodWarningApp

def main():
    try:
        app = QApplication(sys.argv)
        window = FloodWarningApp()
        window.show()
        return app.exec_()
    except Exception as e:
        print(f"Application Error: {str(e)}")
        return 1

if __name__ == '__main__':
    sys.exit(main())