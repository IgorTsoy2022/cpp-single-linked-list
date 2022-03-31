#pragma once

#include <cstddef>
#include <string>
#include <utility>

template <typename Type>
class SingleLinkedList {
    // ���� ������
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };

    // ������ ������ �������� ��������.
    // ���������� ��������� ��������� �� �������� ������������
    // ������
    // ValueType � ��������� � Type (��� Iterator) ���� �
    // const Type (��� ConstIterator)
    template <typename ValueType>
    class BasicIterator {
        // ����� ������ ����������� �������������, ����� ��
        // ������� ������ ��� ������ � ��������� ������� ���������
        friend class SingleLinkedList;

        // �������������� ����������� ��������� �� ��������� ��
        // ���� ������
        explicit BasicIterator(Node* node) {
            node_ = node;
        }

    public:
        // ����������� ���� ���� �������� ����������� ���������� �
        // ��������� ����� ���������

        // ��������� ��������� � forward iterator
        // (��������, ������� ������������ �������� ���������� �
        // ������������ �������������)
        using iterator_category = std::forward_iterator_tag;
        // ��� ���������, �� ������� ������������ ��������
        using value_type = Type;
        // ���, ������������ ��� �������� �������� �����
        // �����������
        using difference_type = std::ptrdiff_t;
        // ��� ��������� �� ����������� ��������
        using pointer = ValueType*;
        // ��� ������ �� ����������� ��������
        using reference = ValueType&;

        BasicIterator() = default;

        // �������������� �����������/����������� �����������
        // ��� ValueType, ����������� � Type, ������ ����
        // ����������� ������������
        // ��� ValueType, ����������� � const Type, ������ ����
        // ��������������� ������������
        BasicIterator(const BasicIterator<Type>& other) noexcept {
            node_ = other.node_;
        }

        // ����� ���������� �� ������� �������������� �� ����������
        // ��������� = ��� ������� ����������������� ������������
        // �����������, ���� ������� �������� = � ��������
        // ���������� ������������� ��� �� ���
        BasicIterator& operator=(const BasicIterator& rhs) =
                                 default;

        // �������� ��������� ���������� (� ���� ������� ���������
        // ��������� ����������� ��������)
        // ��� ��������� �����, ���� ��� ��������� �� ���� � ���
        // �� ������� ������ ���� �� end()
        [[nodiscard]] bool operator==
            (const BasicIterator<const Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }

        // �������� �������� ���������� �� �����������
        // �������������� !=
        [[nodiscard]] bool operator!=
            (const BasicIterator<const Type>& rhs) const noexcept {
            return node_ != rhs.node_;
        }

        // �������� ��������� ���������� (� ���� ������� ���������
        // ��������)
        // ��� ��������� �����, ���� ��� ��������� �� ���� � ���
        // �� ������� ������ ���� �� end()
        [[nodiscard]] bool operator==
            (const BasicIterator<Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }

        // �������� �������� ���������� �� �����������
        // �������������� !=
        [[nodiscard]] bool operator!=
            (const BasicIterator<Type>& rhs) const noexcept {
            return node_ != rhs.node_;
        }

        // �������� ��������������. ����� ��� ������ ��������
        // ��������� �� ��������� ������� ������
        // ���������� ������ �� ������ ����
        // ��������� ���������, �� ������������ �� ������������
        // ������� ������, �������� � �������������� ���������
        BasicIterator& operator++() noexcept {
            this->node_ = this->node_->next_node;
            return *this;
        }

        // �������� ��������������. ����� ��� ������ ��������
        // ��������� �� ��������� ������� ������
        // ���������� ������� �������� ���������
        // ��������� ���������, �� ������������ �� ������������
        // ������� ������,
        // �������� � �������������� ���������
        BasicIterator operator++(int) noexcept {
            auto old_value(*this);
            ++(*this);
            return old_value;
        }

        // �������� �������������. ���������� ������ �� �������
        // �������
        // ����� ����� ��������� � ���������, �� ������������ ��
        // ������������ ������� ������, �������� � ��������������
        // ���������
        [[nodiscard]] reference operator*() const noexcept {
            return node_->value;
        }

        // �������� ������� � ����� ������. ���������� ���������
        // �� ������� ������� ������
        // ����� ����� ��������� � ���������, �� ������������ ��
        // ������������ ������� ������, �������� � ��������������
        // ���������
        [[nodiscard]] pointer operator->() const noexcept {
            return &node_->value;
        }

    private:
        Node* node_ = nullptr;
    };

public:

    SingleLinkedList() = default;

    SingleLinkedList(std::initializer_list<Type> values) {
        SingleLinkedList tmp;
        for (auto value : values) {
            tmp.PushBack(value);
        }
        swap(tmp);
    }

    SingleLinkedList(const SingleLinkedList& other) {
        assert(size_ == 0 && head_.next_node == nullptr);

        if (head_.next_node != other.head_.next_node) {
            SingleLinkedList tmp;
            for (auto it = other.begin(); it != other.end();
                 ++it) {
                tmp.PushBack(*it);
            }
            swap(tmp);
        }
    }

    // ���������� ���������� ������� �� ����� O(1)
    void swap(SingleLinkedList& other) noexcept {
        auto tmp_node = head_.next_node;
        auto tmp_size = size_;

        head_.next_node = other.head_.next_node;
        size_ = other.size_;

        other.head_.next_node = tmp_node;
        other.size_ = tmp_size;
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        if (head_.next_node != rhs.head_.next_node) {
            SingleLinkedList tmp;
            for (auto it = rhs.begin(); it != rhs.end();
                 ++it) {
                tmp.PushBack(*it);
            }
            Clear();
            swap(tmp);
        }
        return *this;
    }

    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;

    // ��������, ����������� ��������� ��������� ������
    using Iterator = BasicIterator<Type>;
    // ����������� ��������, ��������������� ������ ��� ������ �
    // ��������� ������
    using ConstIterator = BasicIterator<const Type>;

    // ���������� ��������, ����������� �� ������� ����� ������
    // ��������� ������������ ������.
    // �������������� ���� �������� ������ - ������� �������������
    // ������� � �������������� ���������
    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator{&head_};
    }

    // ���������� ����������� ��������, ����������� �� �������
    // ����� ������ ��������� ������������ ������.
    // �������������� ���� �������� ������ - ������� �������������
    // ������� � �������������� ���������
    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return ConstIterator{&head_};
    }

    // ���������� ����������� ��������, ����������� �� �������
    // ����� ������ ��������� ������������ ������.
    // �������������� ���� �������� ������ - ������� �������������
    // ������� � �������������� ���������
    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return ConstIterator(const_cast<Node*>(&head_));
    }

    // ���������� ��������, ����������� �� ������ �������
    // ���� ������ ������, ������������ �������� ����� ����� end()
    [[nodiscard]] Iterator begin() noexcept {
        return Iterator{head_.next_node};
    }

    // ���������� ��������, ����������� �� �������, ��������� ��
    // ��������� ��������� ������������ ������
    // �������������� ���� �������� ������ � ������� �������������
    // ������� � �������������� ���������
    [[nodiscard]] Iterator end() noexcept {
        return Iterator{nullptr};
    }

    // ���������� ����������� ��������, ����������� �� ������
    // �������
    // ���� ������ ������, ������������ �������� ����� ����� end()
    // ��������� ������ ������������ ������ ������ cbegin()
    [[nodiscard]] ConstIterator begin() const noexcept {
        return ConstIterator{head_.next_node};
    }

    // ���������� ����������� ��������, ����������� �� �������,
    // ��������� �� ��������� ��������� ������������ ������
    // �������������� ���� �������� ������ � ������� �������������
    // ������� � �������������� ���������
    // ��������� ������ ������������ ������ ������ cend()
    [[nodiscard]] ConstIterator end() const noexcept {
        return ConstIterator{nullptr};
    }

    // ���������� ����������� ��������, ����������� �� ������
    // �������
    // ���� ������ ������, ������������ �������� ����� ����� cend()
    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return ConstIterator{head_.next_node};
    }

    // ���������� ����������� ��������, ����������� �� �������,
    // ��������� �� ��������� ��������� ������������ ������
    // �������������� ���� �������� ������ � ������� �������������
    // ������� � �������������� ���������
    [[nodiscard]] ConstIterator cend() const noexcept {
        return ConstIterator{nullptr};
    }

    // ���������� ���������� ��������� � ������ �� ����� O(1)
    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    // ��������, ������ �� ������ �� ����� O(1)
    [[nodiscard]] bool IsEmpty() const noexcept {
        return size_ == 0;
    }

    // ��������� ������� value � ������ ������ �� ����� O(1)
    void PushFront(const Type& value) {
        Node* ptr_new_node = new Node(value, head_.next_node);

        if (head_.next_node == nullptr) { 
            last_node_ = ptr_new_node;
        }
        head_.next_node = ptr_new_node;
        ++size_;
    }

    void PushBack(const Type& value) {
        Node* ptr_new_node = new Node(value, nullptr);

        if (last_node_ != nullptr) {
            last_node_->next_node = ptr_new_node;
        }
        last_node_ = ptr_new_node;
        if (head_.next_node == nullptr) {
            head_.next_node = last_node_;
        }
        ++size_;
    }

    /*
     * ��������� ������� value ����� ��������, �� �������
     * ��������� pos.
     * ���������� �������� �� ����������� �������
     * ���� ��� �������� �������� ����� ��������� ����������,
     * ������ ��������� � ������� ���������
     */

    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        if (pos.node_ == nullptr) { return Iterator{nullptr}; }

        Node* ptr_new_node = new Node(value, pos.node_->next_node);

        pos.node_->next_node = ptr_new_node;
        if (pos.node_->next_node == nullptr) { 
            last_node_ = ptr_new_node;
        }
        ++size_;
        return Iterator{ptr_new_node};
    }

    void PopFront() noexcept {
        if (head_.next_node != nullptr) {
            Node* ptr_next_node = head_.next_node->next_node;
            delete head_.next_node;

            if (ptr_next_node == nullptr) { last_node_ = nullptr; }
            head_.next_node = ptr_next_node;
            --size_;
        }
    }

    /*
     * ������� �������, ��������� �� pos.
     * ���������� �������� �� �������, ��������� �� ��������
     */
    Iterator EraseAfter(ConstIterator pos) noexcept {
        if (pos.node_ == nullptr || pos.node_->next_node == nullptr) {
            return Iterator{nullptr};
        }

        Node* ptr_node_after_erase = pos.node_->next_node->next_node;
        delete pos.node_->next_node;

        pos.node_->next_node = ptr_node_after_erase;
        if (ptr_node_after_erase == nullptr) {
            last_node_ = pos.node_;
        }
        --size_;
        return Iterator{ptr_node_after_erase};
    }

    // ������� ������ �� ����� O(N)
    void Clear() noexcept {
        Node* next_node;
        while (head_.next_node != nullptr) {
            next_node = head_.next_node->next_node;
            delete head_.next_node;
            head_.next_node = next_node;
        }
        size_ = 0;
        last_node_ = nullptr;
    }

    ~SingleLinkedList() {
        Clear();
    }

private:
    // ��������� ����, ������������ ��� �������
    // "����� ������ ���������"
    Node head_;
    size_t size_ = 0;
    Node* last_node_ = nullptr;
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs,
          SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs,
                const SingleLinkedList<Type>& rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs,
                const SingleLinkedList<Type>& rhs) {
    if (lhs.GetSize() != rhs.GetSize()) { return true; }
    auto rhs_it = rhs.begin();
    for (auto it = lhs.begin(); it != lhs.end(); ++it) {
        if (*it != *rhs_it) { return true; }
        ++rhs_it;
    }
    return false;
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs,
               const SingleLinkedList<Type>& rhs) {
    if (std::lexicographical_compare(lhs.begin(), lhs.end(),
                                     rhs.begin(), rhs.end())) {
        return true;
    }
    return false;
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs,
                const SingleLinkedList<Type>& rhs) {
    if (std::lexicographical_compare(rhs.begin(), rhs.end(),
                                     lhs.begin(), lhs.end())) {
        return false;
    }
    return true;
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs,
               const SingleLinkedList<Type>& rhs) {
    if (std::lexicographical_compare(rhs.begin(), rhs.end(),
                                     lhs.begin(), lhs.end())) {
        return true;
    }
    return false;
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs,
                const SingleLinkedList<Type>& rhs) {
    if (std::lexicographical_compare(lhs.begin(), lhs.end(),
                                     rhs.begin(), rhs.end())) {
        return false;
    }
    return true;
}