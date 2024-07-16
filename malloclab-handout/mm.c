/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "SA23225194",
    /* First member's full name */
    "Cui Zhi Wei",
    /* First member's email address */
    "suinis@yeah.net",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

#define WSIZE 4 /*单字：4字节*/
#define DSIZE 8 /*双字：8字节*/
#define CHUNKSIZE (1<<12) /* 扩展堆空间的单位4KB */

#define MAX(x, y) ((x) > (y) ? (x) : (y))

/*将大小和分配位绑定为一个字：书本设定的块格式，头部脚部*/
#define PACK(size, alloc) ((size) | (alloc)) 

/*向指定位置写入或读出一个字*/
#define GET(p) (*(unsigned int*)(p))
#define PUT(p, val) (*(unsigned int*)(p) = (val))

/*传入头部/脚部指针，获取块大小，分配位*/
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

/*传入当前块块指针（当前块有效载荷指针），获取当前块的头部指针，脚部指针*/
#define HDRP(bp) ((char*)(bp) - WSIZE)
#define FTRP(bp) ((char*)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/*传入当前块块指针（当前块有效载荷指针），获取当前块的下一个块块指针，上一个块块指针*/
#define NEXT_BLKP(bp) ((char*)(bp) + GET_SIZE(((char*)(bp) - WSIZE)))
#define PREV_BLKP(bp) ((char*)(bp) - GET_SIZE(((char*)(bp) - DSIZE)))

/*指向序言块的脚部*/
static char* heap_listp;

/* 4种可能的合并操作 */
static void* coalesce(void *bp) {
    size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));//获取前一个块的分配状态
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));// 获取后一个块的分配状态
    size_t size = GET_SIZE(HDRP(bp));//获取当前块的大小
    
    // 根据前后块的分配状态，进行四种合并操作
    // 1.上下块都已分配
    if (prev_alloc && next_alloc) {
        return bp;
    }
    // 2.前一块已分配，后一块未分配
    else if (prev_alloc && !next_alloc) {
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
    }
    // 3.前一块未分配，后一块已分配
    else if (!prev_alloc && next_alloc) {
        size += GET_SIZE(HDRP(PREV_BLKP(bp)));
        PUT(FTRP(bp), PACK(size, 0));        
        PUT(HDRP(PREV_BLKP(bp)),PACK(size, 0));
        bp = PREV_BLKP(bp);
    }
    // 4.前后块都未分配
    else {
        size += GET_SIZE(HDRP(PREV_BLKP(bp))) + GET_SIZE(FTRP(NEXT_BLKP(bp)));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
    }
    return bp;
}

/* 仅在堆初始化/无法找到合适空闲块时调用 */
static void* extend_heap(size_t words) {
    char *bp;
    size_t size;

    /* 保证2字对齐 */
    size = (words % 2) ? (words + 1) * WSIZE : words * WSIZE;

    if ((long)(bp = mem_sbrk(size)) == -1){
        return NULL;
    }

    /* 新分配块在结尾块上面，所以需要将本次分配的最后一个字作为结尾块 */
    /* 头部脚部设定为 size/0 */
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1)); 
    
    /* 新申请的块也需要进行合并检查，防止产生伪空闲块 */
    return coalesce(bp);
}

/* 首次适配原则 */
static void* find_fit(size_t asize)
{
    void *bp;
    /* 遍历空闲链表，寻找合适大小的空闲块 */
    for(bp = heap_listp; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp)) {
        /* 如果找到合适大小的空闲块：未分配&&大小符合*/
        if(!GET_ALLOC(HDRP(bp)) && (asize <= GET_SIZE(HDRP(bp)))) {
            return bp;
        }
    }
    return NULL;
}

// /* 最佳适配原则 */
// static void* best_fit(size_t asize){
    
// }

static void place(void *bp, size_t asize)
{
    size_t csize = GET_SIZE(HDRP(bp));
    if((csize - asize) >= (2 * DSIZE)){ /* 保证空闲块最小 > 4个字，也即空闲块至少能存储1个B */
        PUT(HDRP(bp), PACK(asize, 1));
        PUT(FTRP(bp), PACK(asize, 1));
        bp = NEXT_BLKP(bp);
        PUT(HDRP(bp), PACK(csize - asize, 0));
        PUT(FTRP(bp), PACK(csize - asize, 0));
    }else{ /* 剩下空间不够4字，全部分配给对应请求 */
        PUT(HDRP(bp), PACK(csize, 1));
        PUT(FTRP(bp), PACK(csize, 1));
    }
}

/* 
 * m    _init - initialize the malloc package.
 */
int mm_init(void)
{
    if ((heap_listp = mem_sbrk(4 * WSIZE)) == (void*)-1) {
        return -1;
    }
    PUT(heap_listp, 0);                            /* 双字边界对齐需要：全0填充保证2字对齐 */
    
    /* 序言块和结尾块都置为1：已分配，这样可以在coalesce合并操作时不必考虑两边边界的特殊情况，也即如果设为0：未分配，则合并时会将序言块/结尾块合并，破坏堆结构 */
    PUT(heap_listp + (1 * WSIZE), PACK(DSIZE, 1)); /* 序言块头部 块大小/分配位：8/1 */
    PUT(heap_listp + (2 * WSIZE), PACK(DSIZE, 1)); /* 序言块脚部 块大小/分配位：8/1 */
    PUT(heap_listp + (3 * WSIZE), PACK(0, 1));     /* 结尾块 块大小/分配位：0/1 */
    heap_listp += (2 * WSIZE);                     /* 定位到序言快脚部位置*/

    /* 先分配4KB堆区空间：1K个字 */
    if (extend_heap(CHUNKSIZE / WSIZE) == NULL) {
        return -1;
    }
    return 0;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
// void *mm_malloc(size_t size)
// {
//     int newsize = ALIGN(size + SIZE_T_SIZE);
//     void *p = mem_sbrk(newsize);
//     if (p == (void *)-1)
// 	return NULL;
//     else {
//         *(size_t *)p = size;
//         return (void *)((char *)p + SIZE_T_SIZE);
//     }
// }

void *mm_malloc(size_t size)
{
    size_t asize;   /* 2字对齐后的大小（单位：B） */
    size_t extendsize;
    char *bp;

    if(size == 0){
        return NULL;
    }
    
    /* 因为2字对齐，且必须包含头部和脚部，又必须申请>1的size，故至少占用4个字 */
    if(size <= DSIZE){
        asize = 2 * DSIZE;
    } else {
        /* 看为三部分：整除8B部分 + 头部尾部8B + 未能整除8B部分（1~7）分配8B */
        asize = DSIZE * ((size + (DSIZE) + (DSIZE - 1)) / DSIZE); 
    }

    /* 找到符合空闲块 */
    if((bp = find_fit(asize)) != NULL) {
        place(bp, asize);  // 将空闲块分配给请求
        return bp;  // 返回指向分配的块指针
    }
    /* 未找到，申请空间比默认CHUNKSIZE还要大时按照需求取 */
    extendsize = MAX(asize, CHUNKSIZE);
    if((bp = extend_heap(extendsize / WSIZE)) == NULL) {
        return NULL;
    }
    place(bp, asize);
    return bp;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *bp)
{
    size_t size = GET_SIZE(HDRP(bp));
    /*将要释放块的头部脚部的分配位置为0：标记为未分配*/
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    /* 释放之后需要考虑同上下两个块中未分配块的合并 */
    coalesce(bp);
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *oldptr = ptr;
    void *newptr;
    size_t copySize;
    
    newptr = mm_malloc(size);
    if (newptr == NULL)
      return NULL;
    copySize = *(size_t *)((char *)oldptr - SIZE_T_SIZE);
    if (size < copySize)
      copySize = size;
    memcpy(newptr, oldptr, copySize);
    mm_free(oldptr);
    // return newptr;
}

// void *mm_realloc(void *ptr, size_t size) {
//     size_t oldsize;
//     void *newptr;
    
//     if (ptr == NULL) {
//         return mm_malloc(size);
//     }

//     if (size == 0) {
//         mm_free(ptr);
//         return 0;
//     }

//     newptr = mm_malloc(size);

//     /* If realloc() fails the original block is left untouched  */
//     if(!newptr) {
//         return 0;
//     }

//     /* Copy the old data. */
//     oldsize = GET_SIZE(HDRP(ptr));
//     if(size < oldsize) {
//         oldsize = size;
//     }
//     memcpy(newptr, ptr, oldsize);

//     /* Free the old block. */
//     mm_free(ptr);

//     return newptr;
// }














