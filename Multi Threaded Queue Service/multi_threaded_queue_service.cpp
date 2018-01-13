//
//  multi_threaded_queue_service.cpp
//  Multi Threaded Queue Service
//
//  Created by Tim Madorma on 1/6/18.
//  Copyright © 2018 Tim Madorma. All rights reserved.
//

#include "multi_threaded_queue_service.hpp"
#include "multi_threaded_queue_service.hpp"

using namespace std;

//--------------------------------------------------------------------------
// multi_threaded_queue_service::multi_threaded_queue_service
//
//  Threads complete execution when the multi_threaded_queue_service object
//  goes out of scope
//
//--------------------------------------------------------------------------
multi_threaded_queue_service::~multi_threaded_queue_service()
{
    for (auto& rd_thrd : read_threads) {
        if (rd_thrd.joinable()) {
            rd_thrd.join();
        }
    }
}

//--------------------------------------------------------------------------
// multi_threaded_queue_service::create_read_queue_threads
//--------------------------------------------------------------------------
void multi_threaded_queue_service::create_read_queue_thrds(int num_threads)
{
    for (int i = 0; i < num_threads; i++) {
        read_threads.push_back(thread(&multi_threaded_queue_service::read_queue, this, i));
    }
}

//--------------------------------------------------------------------------
// multi_threaded_queue_service::read_queue - This is the thread function to read
// the queue. All created threads are "reused" via the outer while loop such that
// they only complete execution when told to do so via the run flag.
//--------------------------------------------------------------------------
int multi_threaded_queue_service::read_queue(int thread_num) {
    
    // NOTE: unique_lock is to be used per thread on a global/shared mutex. Hence
    // the need to instantiate here rather than have it as a member of the class.
    unique_lock<mutex> queue_lock(mtx);
    
    while (run) {
        while( msg_queue.empty() && run) {
            cout << "reader thread " << thread_num << " checked queue but it is empty" << endl;
            wait_for_queue_check_signal(queue_lock);
        }
        
        if (run == false) { break; }
        
        // Call a "processing" function here to process the incoming message
        cout << "reader thread " << thread_num << " operating on message: " << msg_queue.front() << endl;
        msg_queue.pop();
        
        notify_thrds_to_check_queue();
        wait_for_queue_check_signal(queue_lock);
    }
    cout << "reader thread: " << thread_num << " executing by request" << endl;
    return 0;
}

//----------------------------------------------------------------------
// multi_threaded_queue_service::write_queue
//
// A thread safe callback function utilized by "client" thread writers
//----------------------------------------------------------------------
int multi_threaded_queue_service::write_queue(string write_message, int thread_num) {
    
    unique_lock<mutex> queue_lock(mtx);
    
    // The loop that waits for the right circumstance to write. If it's not right,
    // just wait until another thread sends a "check queue signal", then try again.
    while( (msg_queue.size() == max_queue_elems)  && run) {
        if ( msg_queue.size() == max_queue_elems) {
            cout << "writer thread: " << thread_num << " queue is full - wait for reader thrds to pop elements" << endl;
        }
        wait_for_queue_check_signal(queue_lock);
    }
    
    if (run == true) {
        cout << "writer thread: " << thread_num << " writing string: \"" << write_message << "\" to the queue" << endl;
        msg_queue.push(write_message);
        notify_thrds_to_check_queue();
    }
    else { cout << "writer thread: " << thread_num << " exiting by request" << endl; }
    
    return 0;
}

