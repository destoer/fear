#include <fear/fear.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>


void test_array_push_and_pop()
{
    struct Array array = fear_make_array(sizeof(u32));

    for(u32 i = 0; i < 5; i++)
    {
        fear_push_var(&array,&i);
    }

    const u32 expected[] = {4,3,2,1,0};

    const u32 size = fear_array_count(&array);
    for(u32 i = 0; i < size; i++)
    {
        u32 value = 0xdeadbeef;
        fear_pop_var(&array,&value);
        assert(expected[i] == value);
    }

    assert(fear_array_count(&array) == 0);
    fear_destroy_array(&array);

    FEAR_DEBUG("Array push and pop: PASS");
}

#define TEST_HEAP_ARR_SIZE 20

void test_heap_stress()
{
    const u32 base = fear_context.heap.in_use;

    u32 pattern[TEST_HEAP_ARR_SIZE] = 
    {
        0,2,3,4,5,7,2,1,8,9,5,6,6,2,1,9,8,3,4,0
    };

    void* arr[TEST_HEAP_ARR_SIZE];

    for(u32 i = 0; i < TEST_HEAP_ARR_SIZE; i++)
    {
        arr[i] = fear_alloc(1,55);
    }

    for (u32 i = 0; i < TEST_HEAP_ARR_SIZE; i++)
    {
        const u32 index = pattern[i];

        if(arr[index])
        {
            arr[index] = fear_free(arr[index]);
        }

        else
        {
            arr[index] = fear_alloc(1,index * 128);
        }
    }

    for(u32 i = 0; i < TEST_HEAP_ARR_SIZE; i++)
    {
        arr[i] = fear_free(arr[i]);
    }

    assert(fear_context.heap.in_use == base);
    FEAR_DEBUG("Heap stress: pass");
}

void test_fear_format()
{
    u64 long_value = 0xdeadbeefcafebabe;
    struct String str = fear_format("Hello my name is %s i am %d(%x) %lx\n","John",20,20,long_value);
    fear_write_str(str);

    assert(fear_str_equal(str,fear_make_str("Hello my name is John i am 20(14) DEADBEEFCAFEBABE\n")));

    fear_destroy_heap_str(&str);

    FEAR_DEBUG("Fear format: pass");
}

int main()
{
    // Setup our heap
    fear_init_context();

    const u32 base = fear_context.heap.in_use;

    FEAR_DEBUG("Starting fear tests!");

    test_array_push_and_pop();
    test_fear_format();

    FEAR_DEBUG("Heap in use: %zd",fear_context.heap.in_use);
    assert(fear_context.heap.in_use == base);

    test_heap_stress();

    fear_destroy_context();
}