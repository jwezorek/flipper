#pragma once

#include <QMainWindow>
#include <QString>

class QAction;

/*------------------------------------------------------------------------------------------------*/

namespace jw {

    class main_window : public QMainWindow {
        Q_OBJECT

    public:
        explicit main_window(QWidget* parent = nullptr);


    private:

        QWidget* board_;
    };

} 