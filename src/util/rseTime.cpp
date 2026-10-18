#include "rseTime.h"

#include <GLFW/glfw3.h> // glfwGetTime()

RSE::rseTime::timer RSE::rseTime::mainTimer;

void RSE::rseTime::updateTimer(timer& t){
    t.priorTime = t.time;
    t.time = static_cast<float>(glfwGetTime()); // currentFrameTime
    t.deltatime = t.time - t.lastFrameTime;
    t.lastFrameTime = t.time;
    t.frame++;
}
