#include "main_window.hpp"
#include <QApplication>


int main(int argc, char *argv[]) {

    QApplication app(argc, argv);
    
    jw::main_window window;
    window.show();

    return app.exec();
}