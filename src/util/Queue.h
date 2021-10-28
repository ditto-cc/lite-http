#ifndef _QUEUE_H
#define _QUEUE_H

namespace lite_http {

    template <typename T>
    class Queue {
    public:
        typedef struct Node {
            T data;
            Node* next;
        } Node;
        

    };
}

#endif