//
//  multi_threaded_queue_service.hpp
//  Multi Threaded Queue Service
//
//  Created by Tim Madorma on 1/6/18.
//  Copyright © 2018 Tim Madorma. All rights reserved.
//
/*
 The multi_threaded_queue_service is a class that operates as a message
 processing service. It provides the following functionalities:
 
 1) A service via a thread-safe function (write_queue()) to allow a
 client/producer to write to its internal FIFO/queue. When the queue
 is full, it will disallow writing and wait for a signal indicating that it
 can resume because reader threads have popped messages off of the queue/FIFO.
 
 2) The functionality to create a configurable number of internal
 "consumer/service/reader threads, that are continually reused after they
 finish processing a message from the queue, thereby eliminating
 the expensive of dynamically creating threads as needed.
 
 3) Conditional_variable functionality for signaling to and between
 all pending threads so they know when it is and is not appropriate to
 attempt their respective queue operations (reads and writes).
 
 4) An *extendable* read_queue() thread function to process the message
 that is read from the queue as required by the application.
 
 The queue contains string elements (as messages to process) in this implementation.
 It is expected that this will be modified as needed, based on application
 requirements.
 
 Several STL templates and classes are used to accomplish the aforementioned
 functionalities, including:
 
 std::vector <std::thread> read_threads; // vector of the internal read threads
 std::atomic<bool> execute_threads;      // flag telling if we should execute or not
 std::queue <std::string> msg_queue;     // queue to write to and read from
 std::mutex mtx;                         // shared mutex managed by per thread queue_lock
 std::condition_variable check_queue_readiness_cv; // cv for accessing msg_queue
 */

#ifndef multi_threaded_queue_service_hpp
#define multi_threaded_queue_service_hpp

#include <stdio.h>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <thread>

class multi_threaded_queue_service
{
public:
    
    multi_threaded_queue_service(int max_q_elems): max_queue_elems(max_q_elems), execute_threads(true) {}
    ~multi_threaded_queue_service();
    
    // Disable the copy constructor since we have internal threads
    multi_threaded_queue_service(const multi_threaded_queue_service&) = delete;
    
    // Disable the Assignment opeartor since we have internal threads
    multi_threaded_queue_service& operator=(const multi_threaded_queue_service&) = delete;
    
    int write_queue(std::string write_message, int thread_num);
    int create_read_queue_thrds(int num_threads);
    
    void stop_execution(void) { execute_threads = false;
        notify_thrds_to_check_queue();}
    
private:
    
    int read_queue(int);
    
    inline void notify_thrds_to_check_queue(void) { check_queue_readiness_cv.notify_all(); }
    
    inline void wait_for_queue_check_signal(std::unique_lock<std::mutex> &unique_lock_ref)
    { check_queue_readiness_cv.wait(unique_lock_ref); }
    
    int max_queue_elems;
    std::vector <std::thread> read_threads; // vector of the internal read threads
    std::atomic<bool> execute_threads;      // flag telling if we should execute or not
    std::queue <std::string> msg_queue;     // queue to write to and read from
    std::mutex mtx;                         // shared mutex managed by per thread queue_lock
    std::condition_variable check_queue_readiness_cv; // cv for accessing msg_queue
    
};

#endif /* multi_threaded_queue_service_hpp */

