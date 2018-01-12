//
//  main.cpp
//  //  Example usage of the the Multi Threaded Queue Service Class
//
//  Created by Tim Madorma on 1/6/18.
//  Copyright © 2018 Tim Madorma. All rights reserved.
//

#include <iostream>
#include <thread>
#include <exception>
#include <stdexcept>
#include <typeinfo>
#include "multi_threaded_queue_service.hpp"

void simulate_client_thrds(multi_threaded_queue_service &queue_srvc, int num_one_time_write_thrds);
int client_queue_writer_thrd(multi_threaded_queue_service &queue_ref, int thrd_num);

using namespace std;

const int num_resuable_rd_queue_thrds = 10;
const int max_queue_elems = 10;
const int num_one_time_wrt_thrds = 20;

//--------------------------------------------------------------------------
// main()
//--------------------------------------------------------------------------
int main(int argc, const char * argv[])
{
    try {
        multi_threaded_queue_service my_queue_srvc(max_queue_elems);
        
        my_queue_srvc.create_read_queue_thrds(num_resuable_rd_queue_thrds);
        
        simulate_client_thrds(my_queue_srvc, num_one_time_wrt_thrds);
        
        // Sleep for a second to ensure my_queue_srvc.rd_queue() finish processing
        std::this_thread::sleep_for (std::chrono::seconds(1));
        
        my_queue_srvc.stop_execution();
        
    } catch (std::exception& e) {
        std::cerr << "Explanatory string: " << e.what() << std::endl;
        std::cout << "Exception Type: " << typeid(e).name() << "\n";
    }
    
    return 0;
}

//--------------------------------------------------------------------------
// simulate_client_thrds
//
//--------------------------------------------------------------------------
void simulate_client_thrds(multi_threaded_queue_service &queue_srvc, int num_one_time_write_thrds) {
    
    vector<thread> wrt_queue_thds;
    for (int i = 0; i < num_one_time_write_thrds; i++) {
        wrt_queue_thds.push_back(thread(&client_queue_writer_thrd, ref(queue_srvc), i));
    }
    
    for (auto& wrt_thrd : wrt_queue_thds) {
        if (wrt_thrd.joinable()) {
            wrt_thrd.join();
        }
    }
}

//--------------------------------------------------------------------------
// client_queue_writer_thrd()
//--------------------------------------------------------------------------
int client_queue_writer_thrd(multi_threaded_queue_service &queue_ref, int thrd_num) {
    
    // write a "string to the queue that has the thrd_num identifier in it for test purposes
    queue_ref.write_queue("test" + to_string(thrd_num), thrd_num);
    return 0;
}


