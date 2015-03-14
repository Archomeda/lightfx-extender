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

        class LFXE_API Timer {

        public:
            Timer() {}

            template <typename func, typename... arguments>
            Timer(long long milliseconds, func&& f, arguments&&... args) {
                this->isActive = true;
                std::function<typename std::result_of<func(arguments...)>::type()> task = std::bind(std::forward<func>(f), std::forward<arguments>(args)...);

                this->t = std::thread([&, milliseconds, task]() {
                    while (!this->stop) {
                        bool timePassed = false;
                        while (!this->stop && !timePassed) {
                            std::unique_lock<std::mutex> lock(this->cvm);
                            if (this->cv.wait_for(lock, chrono::milliseconds(milliseconds)) == std::cv_status::no_timeout) {
                                // Woke early, nothing to do here
                                // If the timer needs to be stopped, it will automatically break out of the loop because of this->stop
                            } else {
                                // Time passed
                                timePassed = true;
                            }
                        }

                        if (timePassed) {
                            // Do task
                            task();
                        }
                    }
                    this->isActive = false;
                });
            }

            virtual ~Timer() {
                this->Stop();
            }

            bool IsActive() {
                return this->isActive;
            }

            void Stop() {
                if (this->isActive) {
                    this->stop = true;
                    this->cv.notify_all();
                    if (this->t.joinable()) {
                        this->t.join();
                    }
                }
            }

        private:
            bool isActive = false;
            std::atomic<bool> stop = false;
            std::thread t;
            std::condition_variable cv;
            std::mutex cvm;

        };

    }
}

#pragma warning(pop)
