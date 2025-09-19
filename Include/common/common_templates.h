#pragma once
#include <atomic>
#include <exception>
#include <iostream>
#include <memory>
#include <concepts>

#define snew std::make_shared
namespace common {
template <typename T> class array_atomic {
    int size;
    std::atomic<T> *data;

  public:
    array_atomic(int size, T value) : size(size) {
        data = new std::atomic<T>[size];
        for (int i = 0; i < size; ++i) {
            data[i].store(value);
        }
    }

    array_atomic(const array_atomic &) = delete;

    ~array_atomic() { delete[] data; }

    std::atomic<T> &operator[](int i) { return data[i % size]; }

    int getSize() { return size; }
};

template <typename Int> void setBigEndian(uint8_t *pac, Int x) {
    for (int i = sizeof(x) - 1; i >= 0; --i) {
        pac[i] = x & 0xFF;
        x >>= __CHAR_BIT__;
    }
}

template <typename Int> Int getBigEndian(const uint8_t *pac) {
    Int ans = 0;
    for (std::size_t i = 0; i < sizeof(ans); ++i) {
        ans <<= __CHAR_BIT__;
        ans += pac[i];
    }

    return ans;
}

template <typename Int> void setLittleEndian(uint8_t *pac, Int x) {
    for (std::size_t i = 0; i < sizeof(x); ++i) {
        pac[i] = x & 0xFF;
        x >>= __CHAR_BIT__;
    }
}

template <typename Int> Int getLittleEndian(const uint8_t *pac) {
    Int ans = 0;
    for (int i = sizeof(ans) - 1; i >= 0; --i) {
        ans <<= __CHAR_BIT__;
        ans += pac[i];
    }

    return ans;
}

template <typename Int> Int reverseBytes(Int x) {
    Int ans = 0;
    for (int i = 0; i < sizeof(ans); ++i) {
        ans <<= __CHAR_BIT__;
        ans += x & 0xFF;
        x >>= __CHAR_BIT__;
    }

    return ans;
}

template <typename Int> Int toGrayCode(Int x) {
    Int ans = 0;
    for (int i = 0; i < sizeof(x); ++i) {
        uint8_t byte = x & 0xFF;
        uint8_t grey = byte ^ (byte >> 1);
        ans |= grey << (i * __CHAR_BIT__);
        x >>= __CHAR_BIT__;
    }

    return ans;
}

template <typename Int> void setAtomicBits(std::atomic<Int> &atomic, Int bits) {
    Int current = 0;

    Int newValue = current | bits;

    while (!atomic.compare_exchange_strong(current, newValue)) {
        newValue = current | bits;
    }
}

template <typename Int> void unsetAtomicBits(std::atomic<Int> &atomic, Int bits) {
    Int current = ~0;

    Int newValue = current & (~bits);

    while (!atomic.compare_exchange_strong(current, newValue)) {
        newValue = current & (~bits);
    }
}

template <typename Ptr> class no_null_ptr {
    Ptr p_;

  public:
    no_null_ptr(Ptr p) {
        if (p == nullptr) {
            std::cerr << "terminate after providing nullptr in no_null_ptr constructor" << std::endl;
            std::terminate();
        }

        p_ = p;
    }

    Ptr point() { return p_; }
};

template <typename _Tp, typename... _Args>
inline no_null_ptr<std::shared_ptr<_Tp>> make_no_null_shared_ptr(_Args &&...__args) {

    auto ptr = std::make_shared<_Tp>(std::forward<_Args>(__args)...);

    return no_null_ptr(ptr);
}

template <typename T> class lockable {
  public:
    virtual ~lockable() = default;

    void store(no_null_ptr<std::shared_ptr<T>> ptr) { storeNoNull(ptr.point()); }

    void store(const T *ptr) { storeNoNull(std::make_shared<const T>(no_null_ptr<const T *>(ptr).point())); }

    virtual std::shared_ptr<const T> lock() = 0;

  private:
    virtual void storeNoNull(std::shared_ptr<const T> inst) = 0;
};

template <typename T> class lockable_ptr : public lockable<T> {

    std::atomic<std::shared_ptr<const T>> p;

  public:
    lockable_ptr(no_null_ptr<std::shared_ptr<T>> instance) : p(instance.point()) {}

    std::shared_ptr<const T> lock() override { return p.load(std::memory_order_acquire); }

  private:
    void storeNoNull(std::shared_ptr<const T> newPtr) override {
        p.store(common::no_null_ptr(newPtr)->point(), std::memory_order_release);
    }
};

template <typename T> class lockfree_ptr : public lockable<T> {

    std::shared_ptr<const T> inst1;
    std::shared_ptr<const T> inst2;
    std::atomic<std::shared_ptr<const T> *> switchTo;
    std::atomic<int> calls{0};
    std::atomic<int> storeCalls{0};

    std::atomic<std::shared_ptr<const T> *> p;
    std::atomic_bool rightToModifyInsts{true};

  public:
    lockfree_ptr(no_null_ptr<std::shared_ptr<T>> instance) : inst1(instance.point()), inst2(instance.point()) {
        p.store(&inst1);
        switchTo.store(&inst2);
    }

    std::shared_ptr<const T> lock() override {
        calls.fetch_add(1, std::memory_order_acq_rel);
        std::shared_ptr<const T> *myPtr = p.load(std::memory_order_acquire);
        std::shared_ptr<const T> result = *myPtr;
        lastCallUpdateInstPtr();
        return result;
    }

  private:
    void storeNoNull(std::shared_ptr<const T> newPtr) override {
        calls.fetch_add(1, std::memory_order_acq_rel);
        storeCalls.fetch_add(1, std::memory_order_acq_rel);

        if (p.load(std::memory_order_acquire) == &inst1) {
            switchTo.store(&inst2, std::memory_order_release);
        } else {
            switchTo.store(&inst1, std::memory_order_release);
        }

        lastStoreCallUpdateInst(newPtr);
        lastCallUpdateInstPtr();
    }

    void lastCallUpdateInstPtr() {
        std::shared_ptr<const T> *switchFrom = (switchTo.load(std::memory_order_acquire) == &inst1) ? &inst2 : &inst1;
        if (calls.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            p.compare_exchange_strong(switchFrom, switchTo.load(std::memory_order_acquire), std::memory_order_acq_rel,
                                      std::memory_order_relaxed);
        }
    }

    void lastStoreCallUpdateInst(std::shared_ptr<const T> newPtr) {
        if (storeCalls.fetch_sub(1, std::memory_order_acq_rel) == 1) {
            bool rightToModify = true;
            if (rightToModifyInsts.compare_exchange_strong(rightToModify, false, std::memory_order_acq_rel,
                                                           std::memory_order_relaxed)) {
                *switchTo = newPtr;
                rightToModifyInsts.store(true, std::memory_order_release);
            }
        }
    }
};

template <typename _Tp, typename... _Args> inline std::shared_ptr<lockable<_Tp>> make_lockable_ptr(_Args &&...__args) {

    std::shared_ptr<_Tp> ptr = std::make_shared<_Tp>(std::forward<_Args>(__args)...);
    no_null_ptr<std::shared_ptr<_Tp>> nnp = no_null_ptr(ptr);

    return std::make_shared<lockfree_ptr<_Tp>>(nnp);
}

template <std::integral Int> std::size_t as_size(Int x) {
    return static_cast<std::size_t>(static_cast<std::make_unsigned<Int>::type>(x));
}

template <std::floating_point Float> std::size_t as_size(Float x) { return static_cast<std::size_t>(x); }

template <typename Uint> auto as_signed(Uint x) { return static_cast<std::make_signed<Uint>::type>(x); }

template <std::floating_point Float> auto as_signed(Float x) { return static_cast<int64_t>(x); }

} // namespace common
