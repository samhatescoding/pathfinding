#pragma once

#include "LinkedNode.hpp"

#include <cstddef>
#include <stdexcept>
#include <utility>

template <typename T>
class Stack {
public:
    Stack() = default;
    ~Stack() { clear(); }

    Stack(const Stack&) = delete;
    Stack& operator=(const Stack&) = delete;

    Stack(Stack&& other) noexcept { swap(other); }

    Stack& operator=(Stack&& other) noexcept {
        if (this != &other) {
            clear();
            swap(other);
        }
        return *this;
    }

    bool isEmpty() noexcept { return head_ == nullptr; }
    std::size_t getSize() noexcept { return size_; }

    template <typename U>
    void push(U&& value) {
        head_ = new LinkedNode<T>(std::forward<U>(value), head_);
        ++size_;
    }

    T& getTop() {
        if (isEmpty()) throw std::out_of_range("Stack is empty");
        return head_->value;
    }

    T pop() {
        if (isEmpty()) throw std::out_of_range("Stack is empty");
        T value = std::move(head_->value);
        LinkedNode<T>* old = head_;
        head_ = head_->next;
        delete old;
        --size_;
        return value;
    }

    void clear() noexcept {
        while (head_ != nullptr) {
            LinkedNode<T>* old = head_;
            head_ = head_->next;
            delete old;
        }
        size_ = 0;
    }

private:
    void swap(Stack& other) noexcept {
        std::swap(head_, other.head_);
        std::swap(size_, other.size_);
    }

    LinkedNode<T>* head_ = nullptr;
    std::size_t size_ = 0;
};
