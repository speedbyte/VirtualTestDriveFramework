//
// Created by geislerd on 15.02.17.
//

#include <config.h>
#include <core/Utils.h>
#include <algorithm>
#include <sys/resource.h>
#include <chrono>

namespace saliency_sandbox {

    namespace core {

        std::string Utils::uniqueName() {
            auto randchar = []() -> char
            {
                const char charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
                const size_t max_index = (sizeof(charset) - 1);
                return charset[ rand() % max_index ];
            };
            std::string str(4,0);
            std::generate_n( str.begin(), 4, randchar );
            return str;
        }

        void Utils::setMainStackSize() {
            const rlim_t ss = STACK_SIZE;
            struct rlimit rl;

            if(getrlimit(RLIMIT_STACK,&rl)==0) {
                printf("current stack size: %ld (max: %ld, needed: %ld)\n",rl.rlim_cur,rl.rlim_max,ss);
                if(rl.rlim_cur < ss) {
                    rl.rlim_cur = std::min(ss,rl.rlim_max);
                    printf("increase stack size: %ld\n",rl.rlim_cur);
                    if(setrlimit(RLIMIT_STACK,&rl)!=0)
                        printf("cannot increase stack size... hop we will not crash later\n");
                }
                printf("current stack size: %ld (max: %ld, needed: %ld)\n",rl.rlim_cur,rl.rlim_max,ss);
            }
        }

        void Utils::setThreadStackSize(pthread_attr_t& attr) {
            const size_t ss = STACK_SIZE;

            pthread_attr_setstacksize(&attr,ss);
        }

        void Utils::fps(void (*calc)(), float& fps) {
            std::chrono::high_resolution_clock::time_point t1, t2;
            long microseconds;

            t1 = std::chrono::high_resolution_clock::now();
            calc();
            t2 = std::chrono::high_resolution_clock::now();

            microseconds = std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count();
            fps = (1000.0f*1000.0f)/float(microseconds);
        }
    }
}