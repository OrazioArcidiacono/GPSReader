#ifndef THREADSAFEQUEUE_H
#define THREADSAFEQUEUE_H

#include <QQueue>
#include <QMutex>
#include <QWaitCondition>
#include <QString>

/**
 * @brief ThreadSafeQueue provides a thread-safe queue for QString objects.
 *
 * This class ensures that multiple threads can safely enqueue and dequeue raw NMEA sentences.
 */
class ThreadSafeQueue {
public:
    ThreadSafeQueue() {}

    /**
     * @brief Enqueues a raw NMEA sentence.
     * @param sentence The raw sentence to add.
     */
    void enqueue(const QString &sentence) {
        QMutexLocker locker(&m_mutex);
        m_queue.enqueue(sentence);
        m_cond.wakeOne();
    }

    /**
     * @brief Dequeues a raw NMEA sentence.
     *
     * Blocks if the queue is empty until an item is available.
     * @return The raw sentence.
     */
    QString dequeue() {
        QMutexLocker locker(&m_mutex);
        while (m_queue.isEmpty()) {
            m_cond.wait(&m_mutex);
        }
        return m_queue.dequeue();
    }

    /**
     * @brief Checks if the queue is empty.
     * @return True if empty, false otherwise.
     */
    bool isEmpty() {
        QMutexLocker locker(&m_mutex);
        return m_queue.isEmpty();
    }

private:
    QQueue<QString> m_queue;
    QMutex m_mutex;
    QWaitCondition m_cond;
};

#endif // THREADSAFEQUEUE_H
