/*
 *Author:GeneralSandman
 *Code:https://github.com/GeneralSandman/TinyWeb
 *E-mail:generalsandman@163.com
 *Web:www.dissigil.cn
 */

/*---XXX---
 *
 ****************************************
 *
 */

#include <tiny_base/memorypool.h>

#include <iostream>
#include <string.h>
#include <string>
#include <vector>

using namespace std;

void basicShow()
{
    std::cout << "chain_t size:" << sizeof(chain_t) << std::endl;
    std::cout << "buffer_t size:" << sizeof(buffer_t) << std::endl;
    std::cout << "block_t size:" << sizeof(block_t) << std::endl;
}

void test1()
{
    MemoryPool pool;

    std::vector<void*> alloc16;
    std::vector<void*> alloc32;
    std::vector<void*> alloc48;
    std::vector<void*> alloc56;
    std::vector<void*> alloc88;
    std::vector<void*> alloc128;
    for (int i = 0; i < 10; i++) {
        alloc16.push_back(pool.allocate(16));
        alloc32.push_back(pool.allocate(32));
        alloc48.push_back(pool.allocate(48));
        alloc56.push_back(pool.allocate(56));
        alloc88.push_back(pool.allocate(88));
        alloc128.push_back(pool.allocate(128));
    }
    cout << "allocatedSpace:" << pool.allocatedSpace() << endl;
    for (int i = 0; i < 10; i++) {
        pool.deallocate(alloc16[i], 16);
        pool.deallocate(alloc32[i], 32);
        pool.deallocate(alloc48[i], 48);
        pool.deallocate(alloc56[i], 56);
        pool.deallocate(alloc88[i], 88);
        pool.deallocate(alloc128[i], 128);
    }
    cout << "allocatedSpace:" << pool.allocatedSpace() << endl;
}

void test2()
{
    MemoryPool pool;

    chain_t* chain1 = nullptr;
    chain_t* chain2 = nullptr;
    unsigned int allSize = 1024;

    chain1 = pool.getNewChain(8);
    chain2 = pool.getNewChain(8);

    pool.mallocSpace(chain1, allSize);
    pool.mallocSpace(chain2, allSize);

    std::cout << "chain1 size:" << countChain(chain1) << std::endl;
    std::cout << "chain2 size:" << countChain(chain2) << std::endl;

    chain_t* tmp = chain1;
    while (nullptr != tmp) {
        if (tmp->buffer->end - tmp->buffer->begin != allSize) {
            std::cout << "not pass test" << std::endl;
        }

        tmp = tmp->next;
    }
    if (nullptr == tmp) {
        std::cout << "chain1 pass test\n";
    }

    tmp = chain2;
    while (nullptr != tmp) {
        if (tmp->buffer->end - tmp->buffer->begin != allSize) {
            std::cout << "not pass test" << std::endl;
        }

        tmp = tmp->next;
    }
    if (nullptr == tmp) {
        std::cout << "chain2 pass test\n";
    }

    pool.catChain(chain1, chain2);
    std::cout << "chain1 size:" << countChain(chain1) << std::endl;
    tmp = chain1;
    while (nullptr != tmp) {
        if (tmp->buffer->end - tmp->buffer->begin != allSize) {
            std::cout << "not pass test" << std::endl;
        }

        tmp = tmp->next;
    }
    if (nullptr == tmp) {
        std::cout << "chain1 pass test\n";
    }

    cout << "allocatedSpace:" << pool.allocatedSpace() << endl;
    cout << "allocatedLargeSpace:" << pool.allocatedLargeSpace() << endl;
}

void test3()
{
    MemoryPool pool;

    chain_t* chain1 = nullptr;
    unsigned int allSize = 1024;

    chain1 = pool.getNewChain(8);

    pool.mallocSpace(chain1, allSize);

    std::cout << "chain1 size:" << countChain(chain1) << std::endl;

    chain_t* tmp = chain1;
    while (nullptr != tmp) {
        if (tmp->buffer->end - tmp->buffer->begin != allSize) {
            std::cout << "not pass test" << std::endl;
        }

        tmp = tmp->next;
    }
    if (nullptr == tmp) {
        std::cout << "chain1 pass test" << std::endl;
    }

    std::string data;
    for (int i = 0; i < 1024; i++)
        data += 'a';

    unsigned int buff_size = 0;
    unsigned int used_size = 0;
    tmp = chain1;
    buffer_t* buffer;
    while (nullptr != tmp) {
        buffer = tmp->buffer;

        buff_size = buffer->end - buffer->begin;
        memset((void*)buffer->begin, 0, buff_size);
        memcpy((void*)buffer->begin, (void*)data.c_str(), data.size());
        buffer->used = buffer->begin + data.size();

        tmp = tmp->next;
    }

    tmp = chain1;
    while (nullptr != tmp) {
        buffer = tmp->buffer;

        buff_size = buffer->end - buffer->begin;
        used_size = buffer->used - buffer->begin;

        printf("buff-size(%u),data-size(%u)[%.*s]\n", buff_size, used_size, used_size, (char*)buffer->begin);

        tmp = tmp->next;
    }

    chain_t* chain2 = chain1;

    pool.catChain(chain1, chain2);

    tmp = chain1;
    while (nullptr != tmp) {
        buffer = tmp->buffer;

        buff_size = buffer->end - buffer->begin;
        used_size = buffer->used - buffer->begin;

        printf("buff-size(%u),data-size(%u)[%.*s]\n", buff_size, used_size, used_size, (char*)buffer->begin);

        tmp = tmp->next;
    }
}

int main()
{
    basicShow();
    // test1();
    // test2();
    //
    test3();
}
