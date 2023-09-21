#ifndef RINGBUFFER
#define RINGBUFFER

#include <atomic>
#include <array>

template<typename T, std::uint64_t N>
class RingBuffer {
    std::array<T, N> buffer;
    std::atomic<std::uint64_t> readPos = {0};
    std::atomic<std::uint64_t> writePos = {0};

public:
    bool try_push(const T& value) {
        const auto current_write = writePos.load(std::memory_order_relaxed);
        const auto next_write = increment(current_write);
        if(next_write != readPos.load(std::memory_order_acquire)) {
            buffer[current_write] = value;
            writePos.store(next_write, std::memory_order_release);
            return true;
        }

        return false;  // buffer is full
    }

    bool try_pop(T& value) {
        const auto current_read = readPos.load(std::memory_order_relaxed);
        if(current_read == writePos.load(std::memory_order_acquire)) {
            return false;  // buffer is empty
        }

        value = buffer[current_read];
        readPos.store(increment(current_read), std::memory_order_release);
        return true;
    }


    bool is_empty() {
        auto current_read = readPos.load(std::memory_order_relaxed);
        return current_read == writePos.load(std::memory_order_acquire);
    }

private:
    static constexpr std::uint64_t increment(std::uint64_t n) noexcept {
        return (n + 1) % N;
    }
};

#endif