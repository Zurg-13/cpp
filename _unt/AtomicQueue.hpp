#ifndef ATOMICQUEUE_HPP
#define ATOMICQUEUE_HPP

#include <QAtomicPointer>

/* Атомарная очередь. *********************************************************/
/******************************************************************************/
template<class T>
class AtomicQueue {

    struct QueueNode {
        QueueNode(const T& value) : next(nullptr), data(value) {}
        QueueNode *next;
        T          data;
    };// QueueNode

  public:
    AtomicQueue() {
        m_front = new QueueNode(T());
        m_tail.store(m_front);
        m_divider.store(m_front);
    }// AtomicQueue

   ~AtomicQueue() {
        QueueNode *node = m_front;
        while(node->next) {
            QueueNode *n = node->next;
            delete node;
            node = n;
        }// while(node->next)
    }// ~AtomicQueue

    void push(const T& value) {
        m_tail.load()->next = new QueueNode(value);
        m_tail = m_tail.load()->next; // This moves the QueueNode into the atomic pointer, making it safe :)
        while(m_front != m_divider.load()) {
            QueueNode *tmp = m_front;
            m_front = m_front->next;
            delete tmp;
        }// while(m_front != m_divider.load())
    }// push

    bool peek(T& result) {
        if(m_divider.load() != m_tail.load()) {
            QueueNode *next = m_divider.load()->next;
            if(next) {
                result = next->data;
                return true;
            }// if(next)
        }// if(m_divider.load() != m_tail.load())
        return false;
    }// peek

    bool take(T& result) {
        bool res = this->peek(result);
        if(res) { m_divider = m_divider.load()->next; }
        return res;
    }// pop

  private:
    QueueNode                 *m_front;
    QAtomicPointer<QueueNode>  m_divider, m_tail;
};// AtomicQueue

#endif // ATOMICQUEUE_HPP
