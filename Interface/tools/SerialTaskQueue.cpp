#include "SerialTaskQueue.hpp"

McCAD::SerialTaskQueue::~SerialTaskQueue(){
    complete();
}

void
McCAD::SerialTaskQueue::submit(std::function<void(void)>&& task){
    queue.push(std::move(task));
}

void
McCAD::SerialTaskQueue::complete(){
    while(!queue.empty()){
        queue.front()();
        queue.pop();
    }
}
