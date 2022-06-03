#include "ParallelTaskQueue.hpp"
#include <thread>
#include <algorithm>
#include <boost/asio/post.hpp>

McCAD::ParallelTaskQueue::ParallelTaskQueue()
    : pool{std::max(std::thread::hardware_concurrency(), 4u)}{
}

McCAD::ParallelTaskQueue::~ParallelTaskQueue(){
    complete();
}

void
McCAD::ParallelTaskQueue::submit(std::function<void(void)>&& task){
    boost::asio::post(pool, task);
}

void
McCAD::ParallelTaskQueue::complete(){
    pool.join();
}
