#include "learn_multithreaded_rendering.h"

LearnMultithreadedRendering::LearnMultithreadedRendering(QWidget *parent)
    : QWidget(parent), renderer_(nullptr)
{
    ui.setupUi(this);
}

LearnMultithreadedRendering::~LearnMultithreadedRendering()
{
  renderer_.reset();
}