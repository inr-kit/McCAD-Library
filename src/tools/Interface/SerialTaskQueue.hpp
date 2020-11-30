#ifndef SERIALTASKQUEUE_HPP
#define SERIALTASKQUEUE_HPP

#include <queue>
#include <functional>

namespace McCAD{

    class SerialTaskQueue{

    public:
        SerialTaskQueue() = default;
        SerialTaskQueue(const SerialTaskQueue&) = delete;
        SerialTaskQueue(SerialTaskQueue&&) = default;
        SerialTaskQueue& operator=(const SerialTaskQueue&) = delete;
        SerialTaskQueue& operator=(SerialTaskQueue&&) = default;
        ~SerialTaskQueue();

        void submit(std::function<void(void)>&& task);
        void complete();

    private:
        std::queue<std::function<void(void)>> queue;

    };

}

#endif // SERIALTASKQUEUE_HPP
