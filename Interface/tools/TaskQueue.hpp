#ifndef TASKQUEUE_HPP
#define TASKQUEUE_HPP

#include "SerialTaskQueue.hpp"
#include "ParallelTaskQueue.hpp"

namespace McCAD{

    namespace Policy{
        class Serial{};
        class Parallel{};
    }

    namespace detail{
        template<typename PolicyT>
        struct TaskQueueSwitch;

        template<>
        struct TaskQueueSwitch<Policy::Serial>{
            using type = SerialTaskQueue;
        };

        template<>
        struct TaskQueueSwitch<Policy::Parallel>{
            using type = ParallelTaskQueue;
        };
    }

    template<typename PolicyT>
    using TaskQueue
        = typename detail::TaskQueueSwitch<PolicyT>::type;

}

#endif // TASKQUEUE_HPP
