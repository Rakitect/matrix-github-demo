#include <queue>
#include <string>
#include <mutex>
#include <condition_variable>

class MessageQueue {
public:
    MessageQueue() : max_size(1000) {}

    void enqueue(const std::string& message) {
        std::unique_lock<std::mutex> lock(mtx);
        cond_var.wait(lock, [this]() { return queue.size() < max_size; });
        queue.push(message);
        cond_var.notify_all();
    }

    std::string dequeue() {
        std::unique_lock<std::mutex> lock(mtx);
        cond_var.wait(lock, [this]() { return !queue.empty(); });
        std::string message = queue.front();
        queue.pop();
        cond_var.notify_all();
        return message;
    }

private:
    std::queue<std::string> queue;
    std::mutex mtx;
    std::condition_variable cond_var;
    const size_t max_size;
};

// ...existing code...

