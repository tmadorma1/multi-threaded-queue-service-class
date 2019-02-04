# multi-threaded-queue-service-class

 The multi_threaded_queue_service is a class that operates as a message
 processing service. It provides the following functionalities:
 
 1) A service via a thread-safe function (write_queue()) to allow a
 client/producer to write to its internal FIFO/queue. When the queue
 is full, it will disallow writing and wait for a signal indicating that it
 can resume because reader threads have popped messages off of the queue/FIFO.
 
 2) Built-in internal "consumer/service/reader" threads. The number of these
 threads is configurable. They are reused after they finish
 processing an associated message from the queue, thereby eliminating
 the expense of dynamically creating threads as needed.
 
 3) Conditional_variable functionality for signaling to and between
 all pending threads so they know when it is and is not appropriate to
 attempt their respective queue operations (reads/writes).
 
 4) A *customizable* read_queue() thread function to process the message
 that is read from the queue as required by the application.
 
 Note: The queue currently contains string elements (as messages to process) in 
 this implementation. It is expected that the format of the message will be 
 modified as needed (based on application requirements).
 
 Several STL templates and classes are used to accomplish the aforementioned 
 functionalities, including:
 
 std::vector <std::thread> read_threads; // vector of the internal read threads
 
 std::atomic<bool> execute_threads;      // flag telling if we should execute or not
 
 std::queue <std::string> msg_queue;     // queue to write to and read from
 
 std::mutex mtx;                         // shared mutex, managed by a per thread queue_lock
 
 std::condition_variable check_queue_readiness_cv; // cv for accessing msg_queue
