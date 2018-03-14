#include "learn_multithreaded_rendering.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LearnMultithreadedRendering w;
    w.show();
    return a.exec();
}
