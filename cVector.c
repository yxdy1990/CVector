#include "cVector.h"

#define ELEM(i)     ((char *) (v->elem) + (i) * (v->size))

static bool grow(vector v);

vector vector_new(size_t size, void (*freefn)(void *)) {
    vector v = malloc(sizeof(*v));
    memset(v, 0, sizeof(*v));
    if (!v)
        return NULL;
    //v->plen = hint ? hint : 16;
    v->llen = 0;
    v->size = size ? size : sizeof(char *);
    v->free = freefn;
    v->elem = malloc(v->size);
    memset(v->elem, 0, v->size);
    if (!v->elem) {
        free(v);
        return NULL;
    }
    return v;
}

void vector_free(vector v) {
#ifdef DEBUG_MODE
    printf("vector_free: Count[%d]\n", v->llen);
#endif
    if (!v)
        return;
    if (v->free) {
        size_t i = 0;
        for (; i < v->llen; i++)
            v->free(*(void **) (ELEM(i)));
    }
    free(v->elem);
    v->elem = NULL;
    free(v);
    v = NULL;
}

size_t vector_size(vector v) {
    if (!v)
        return 0;
    return v->llen;
}

bool vector_empty(vector v) {
    if (!v)
        return true;
    return v->llen ? false : true;
}

bool vector_clear(vector v) {
    if (!v)
        return false;
    if (v->free) {
        size_t i = 0;
        for (; i < v->llen; i++)
            v->free(*(void **) (ELEM(i)));
    }
    v->llen = 0;
    return true;
}

bool vector_get(vector v, size_t i, void **elem) {
    if (!v || i >= v->llen || !elem)
        return false;
    *elem = *(void **) (ELEM(i));
    return true;
}

bool vector_remove(vector v, size_t i) {
#ifdef DEBUG_MODE
    printf("vector_remove: Count[%d] Index[%d]\n", v->llen, i);
#endif
    if (!v || i >= v->llen)
        return false;
    if (v->free)
        v->free(*(void **) (ELEM(i)));
    if (i == v->llen - 1)
        v->llen--;
    else {
        int it = i;
        for (; it < v->llen - 1; it++)
            *(void **) (ELEM(i)) = *(void **) (ELEM(i + 1));
        v->llen--;
    }
    return true;
}

bool vector_add(vector v, void *elem) {
    if (!v || !elem)
        return false;
    if (v->llen != 0)
        if (!grow(v))
            return false;
    v->llen++;
    *(void **) (ELEM(v->llen - 1)) = elem;
#ifdef DEBUG_MODE
    printf("vector_add: Vector elem has been add.\n");
#endif
    return true;
}

bool vector_insert(vector v, size_t i, void *elem) {
    if (!v || i > v->llen)
        return false;
    if (i == v->llen)
        return vector_add(v, elem);
    if (!grow(v))
        return false;
    v->llen++;
    int it = v->llen - 1;
    for (; it > i; it--)
        *(void **) (ELEM(it)) = *(void **) (ELEM(it - 1));
    *(void **) (ELEM(i)) = elem;

    return true;
}

static bool grow(vector v) {
    size_t more = v->llen + 1; //v->plen * 2;
    void *temp = realloc(v->elem, more * v->size);
    if (!temp)
        return false;
    v->elem = temp;
    return true;
}

void vector_def_free_func(void *obj) {
    if (obj) {
        free(obj);
        obj = NULL;
#ifdef DEBUG_MODE
        printf("c_vector: Vector elem has been free !\n");
#endif
    }
}
