#ifndef PARALLELTASKQUEUE_HPP
#define PARALLELTASKQUEUE_HPP

#include <boost/asio/thread_pool.hpp>
#include <functional>

namespace McCAD{

    class ParallelTaskQueue{

    public:
        ParallelTaskQueue();
        ParallelTaskQueue(const ParallelTaskQueue&) = delete;
        ParallelTaskQueue(ParallelTaskQueue&&) = default;
        ParallelTaskQueue& operator=(const ParallelTaskQueue&) = delete;
        ParallelTaskQueue& operator=(ParallelTaskQueue&&) = default;
        ~ParallelTaskQueue();

        void submit(std::function<void(void)>&& task);
        void complete();

    private:
        boost::asio::thread_pool pool;

    };

}

#endif // PARALLELTASKQUEUE_HPP
