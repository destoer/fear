#include <fear/fear.h>
#include <assert.h>
#include <stdlib.h>


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

void test_fear_format()
{
    struct String str = fear_format("Hello my name is %s i am %d(%x)\n","John",20,20);
    fear_write_str(str);

    assert(fear_str_equal(str,fear_make_str("Hello my name is John i am 20(14)\n")));

    fear_destroy_heap_str(&str);

    FEAR_DEBUG("Fear format: pass");
}

int main()
{
    // Setup our heap
    size_t heap_size = 4 * 1024 * 1024;
    void* heap = malloc(heap_size);
    assert(heap);

    fear_init_context(heap,heap_size);

    FEAR_DEBUG("Starting fear tests!");

    test_array_push_and_pop();
    test_fear_format();

    FEAR_DEBUG("Heap in use: %d",fear_context.heap.in_use);
    assert(fear_context.heap.in_use == 0);
}