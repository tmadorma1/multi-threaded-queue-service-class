# multi-threaded-queue-service-class

 The multi threaded queue service is a class that operates as a message
 processing system that includes functionality to:
 
 1) Provides a service via a thread-safe method to allow a client/producer/writer
 to write to its internal FIFO/queue. When the queue is full, it will stop writing
 and wait for a signal indicating that the reader threads have popped messages off
 of the queue/FIFO.
 
 2) Provides the functionality to create a configurable number of internal
 "consumer/service/reader threads, that are continually reused after they
 finish processing a message from the queue, thereby eliminating the
 expense of dynamic "as needed" thread creation.
 
 3) Provides conditional_variable functionality for signaling to and between
 all pending threads so they know when it is and is not appropriate to
 attempt their respective queue operations.
 
 4) Provides an extendable read_queue thread function to process the message
 that is read from the queue as required by the application.
 
 5) The queue contains string elements in this implementation. It is expected
 that this will be modified as needed, based on application requirements.
 
 Several STL templates and classes are used to accomplish this, including:
 
 std::vector <std::thread> read_threads; // vector of the internal read threads
 std::atomic<bool> execute_threads;      // flag telling if we should execute or not
 std::queue <std::string> msg_queue;     // queue to write to and read from
 std::mutex mtx;                         // shared mutex managed by per thread queue_lock
 std::condition_variable check_queue_readiness_cv; // cv for accessing msg_queue
