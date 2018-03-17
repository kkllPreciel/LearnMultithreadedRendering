#include "learn_multithreaded_rendering.h"
#include <QtWidgets/QApplication>

// デバッグ(メモリリーク)用
#if defined(_WIN32) && !defined(QT_NO_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

int main(int argc, char *argv[])
{
#if defined(_WIN32) && !defined(QT_NO_DEBUG)
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    QApplication a(argc, argv);
    LearnMultithreadedRendering w;
    w.show();
    return a.exec();
}