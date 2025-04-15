#include <fear/fear.h>
#include <assert.h>


void array_push_and_pop()
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

int main()
{
    FEAR_DEBUG("Starting fear tests!");

    array_push_and_pop();
}