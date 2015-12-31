#pragma once

// Standard includes
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>

// API exports
#include "../Common/ApiExports.h"


#pragma warning(push)
#pragma warning(disable : 4251)

namespace lightfx {
    namespace utils {

        class LFXE_API NotifyEvent {

        public:
            NotifyEvent() {}
            virtual ~NotifyEvent() {}

            void Notify() {
                std::lock_guard<std::mutex> lock(this->m);
                this->notify = true;
                this->cv.notify_all();
            }

            bool IsNotified() {
                return this->notify;
            }

            void Wait() {
                std::unique_lock<std::mutex> lock(this->m);
                this->cv.wait(lock, [&] { return (bool)this->notify; });
                this->notify = false;
            }

            template <typename _Predicate>
            void Wait(_Predicate predicate) {
                auto check = std::bind(std::forward<_Predicate>(predicate));

                std::unique_lock<std::mutex> lock(this->m);
                this->cv.wait(lock, [&] { return this->notify || check(); });
                this->notify = false;
            }

        private:
            std::atomic<bool> notify = false;
            std::condition_variable cv;
            std::mutex m;

        };

    }
}

#pragma warning(pop)
