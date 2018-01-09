# CVector
C版的Vector。

在c_vector的基础上做了改动，修复了问题，vector可存放N个buffer的指针。

使用说明：

1. New：vector vector_new(size_t size, void (*freefn)(void *));
    size: buffer指针长度，如sizeof(char *)
    freefn: 释放buffer内存的方法，若为NULL，则buffer不会释放

2. Add：bool vector_add(vector v, void *elem);
    v：vector对象
    elem：新添加的buffer指针

3. Get：bool vector_get(vector v, size_t i, void **elem);
    v：vector对象
    elem：存放buffer指针的指针

4. Remove：bool vector_remove(vector v, size_t i);
    v：vector对象
    i：要删除buffer的索引

5. Free：void vector_free(vector v);

备注：
    void vector_def_free_func(void *obj); 通用buffer释放方法