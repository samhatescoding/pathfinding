#pragma once

#include "LinkedNode.hpp"

#include <cstddef>
#include <stdexcept>
#include <utility>

template <typename T>
class Queue {
public:
    Queue() = default;
    ~Queue() { clear(); }

    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;

    Queue(Queue&& other) noexcept { swap(other); }

    Queue& operator=(Queue&& other) noexcept {
        if (this != &other) {
            clear();
            swap(other);
        }
        return *this;
    }

    bool isEmpty() noexcept { return head_ == nullptr; }
    std::size_t getSize() noexcept { return size_; }

    template <typename U>
    T& getFront() {
        if (isEmpty()) throw std::out_of_range("Queue is empty");
        return head_->value;
    }
    
    void push(U&& value) {
        auto* node = new LinkedNode<T>(std::forward<U>(value));
        if (tail_ == nullptr) head_ = node;
        else tail_->next = node;
        tail_ = node;
        ++size_;
    }

    T pop() {
        if (isEmpty()) throw std::out_of_range("Queue is empty");
        T value = std::move(head_->value);
        LinkedNode<T>* old = head_;
        head_ = head_->next;
        delete old;
        if (head_ == nullptr) tail_ = nullptr;
        --size_;
        return value;
    }

    void clear() noexcept {
        while (head_ != nullptr) {
            LinkedNode<T>* old = head_;
            head_ = head_->next;
            delete old;
        }
        tail_ = nullptr;
        size_ = 0;
    }

private:
    void swap(Queue& other) noexcept {
        std::swap(head_, other.head_);
        std::swap(tail_, other.tail_);
        std::swap(size_, other.size_);
    }

    LinkedNode<T>* head_ = nullptr;
    LinkedNode<T>* tail_ = nullptr;
    std::size_t size_ = 0;
};
