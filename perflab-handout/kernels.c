/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

#define BLOCK_SIZE 32

/*
 * Please fill in the following team struct
 */
team_t team = {
    "SA23225194", /* Team name */

    "Cui Zhi Wei",     /* First member full name */
    "suinis@yeah.net", /* First member email address */

    "", /* Second member full name (leave blank if none) */
    ""  /* Second member email addr (leave blank if none) */
};

/***************
 * ROTATE KERNEL
 ***************/

/******************************************************
 * Your different versions of the rotate kernel go here
 ******************************************************/

/*
 * naive_rotate - The naive baseline version of rotate
 */
char naive_rotate_descr[] = "naive_rotate: Naive baseline implementation";
void naive_rotate(int dim, pixel *src, pixel *dst)
{
    int i, j;

    for (i = 0; i < dim; i++)
        for (j = 0; j < dim; j++)
            dst[RIDX(dim - 1 - j, i, dim)] = src[RIDX(i, j, dim)];
}

/*
 * rotate - Your current working version of rotate
 * IMPORTANT: This is the version you will be graded on
 */
char rotate_descr[] = "rotate1 : change index";
void rotate(int dim, pixel *src, pixel *dst)
{
    int i, j, new_j;

    // for (j = 0; j < dim; j++)
    // for (i = 0; i < dim; i++)
    //     dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];

    for (i = 0; i < dim; ++i)
    {
        new_j = dim - i - 1;
        for (j = 0; j < dim; ++j)
        {
            dst[RIDX(i, j, dim)] = src[RIDX(j, new_j, dim)];
        }
    }
}

char rotate_descr2[] = "rotate2 : set block";
void rotate2(int dim, pixel *src, pixel *dst)
{
    int i, j, ii, jj;

    for (i = 0; i < dim; i += BLOCK_SIZE)
    {
        for (j = 0; j < dim; j += BLOCK_SIZE)
        {
            for (ii = i; ii < i + BLOCK_SIZE && ii < dim; ++ii)
            {
                int new_jj = dim - 1 - ii;
                for (jj = j; jj < j + BLOCK_SIZE && jj < dim; ++jj)
                {
                    dst[RIDX(ii, jj, dim)] = src[RIDX(jj, new_jj, dim)];
                }
            }
        }
    }
}

char rotate_descr3[] = "rotate3: set block, loop unrolling";
void rotate3(int dim, pixel *src, pixel *dst)
{
    int i, j;
    dst += (dim - 1) * dim; ////定位到src[0][0]逆时针旋转后，dst该在的位置=dst[n-1][0]
    for (i = 0; i < dim; i += BLOCK_SIZE)
    {
        for (j = 0; j < dim; ++j)
        {
            dst[0] = src[0];
            dst[1] = src[dim];
            dst[2] = src[2 * dim];
            dst[3] = src[3 * dim];
            dst[4] = src[4 * dim];
            dst[5] = src[5 * dim];
            dst[6] = src[6 * dim];
            dst[7] = src[7 * dim];
            dst[8] = src[8 * dim];
            dst[9] = src[9 * dim];
            dst[10] = src[10 * dim];
            dst[11] = src[11 * dim];
            dst[12] = src[12 * dim];
            dst[13] = src[13 * dim];
            dst[14] = src[14 * dim];
            dst[15] = src[15 * dim];
            dst[16] = src[16 * dim];
            dst[17] = src[17 * dim];
            dst[18] = src[18 * dim];
            dst[19] = src[19 * dim];
            dst[20] = src[20 * dim];
            dst[21] = src[21 * dim];
            dst[22] = src[22 * dim];
            dst[23] = src[23 * dim];
            dst[24] = src[24 * dim];
            dst[25] = src[25 * dim];
            dst[26] = src[26 * dim];
            dst[27] = src[27 * dim];
            dst[28] = src[28 * dim];
            dst[29] = src[29 * dim];
            dst[30] = src[30 * dim];
            dst[31] = src[31 * dim];
            src++;      // 读取按列读取，已经读取完第一列32个像素，直接src++，指向第二列
            dst -= dim; // 写入按行写入，src的第一列对应dst最后一行，src++指向第二列后，dst往前推（内存中地址减）一行的个数也即dim
        }
        src += 31 * dim;       // src前32行读取完毕，src应指向下一个32行行首地址（前面已经执行dim次++，当前指向src[1][0]，故再往后推31*dim）
        dst += dim * dim + 32; // dst前32行写入完毕，dst应指向下一个32行行首地址（前面已经执行dim次-=dim，还原并将dst移动到新的32*dim元素的目标位置）
    }
}

char rotate_descr4[] = "rotate4: set block for each dim";
void rotate4(int dim, pixel *src, pixel *dst)
{
    int i, j, k;
    dst += (dim - 1) * dim; // 定位到 src[0][0] 逆时针旋转后 dst 该在的位置 = dst[n-1][0]

    int blocksize = 4; // 默认的块大小

    // 根据不同的 dim 值选择合适的块大小
    if (dim <= 64)
    {
        blocksize = 64;
    }
    else if (dim <= 128)
    {
        blocksize = 32;
    }
    else if (dim <= 256)
    {
        blocksize = 16;
    }
    else if (dim <= 512)
    {
        blocksize = 8;
    } // 对于大于 512 的情况，默认使用 BLOCK_SIZE = 4

    for (i = 0; i < dim; i += blocksize)
    {
        for (j = 0; j < dim; ++j)
        {
            for (k = 0; k < blocksize; ++k)
            {
                if (i + k < dim)
                { // 检查是否越界
                    dst[k] = src[k * dim];
                }
            }
            src++;
            dst -= dim;
        }
        src += (blocksize - 1) * dim;
        dst += dim * dim + blocksize;
    }
}

/*********************************************************************
 * register_rotate_functions - Register all of your different versions
 *     of the rotate kernel with the driver by calling the
 *     add_rotate_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.
 *********************************************************************/

void register_rotate_functions()
{
    add_rotate_function(&naive_rotate, naive_rotate_descr);
    add_rotate_function(&rotate, rotate_descr);
    add_rotate_function(&rotate2, rotate_descr2);
    add_rotate_function(&rotate3, rotate_descr3);
    add_rotate_function(&rotate4, rotate_descr4);
    /* ... Register additional test functions here */
}

/***************
 * SMOOTH KERNEL
 **************/

/***************************************************************
 * Various typedefs and helper functions for the smooth function
 * You may modify these any way you like.
 **************************************************************/

/* A struct used to compute averaged pixel value */
typedef struct
{
    int red;
    int green;
    int blue;
    int num;
} pixel_sum;

/* Compute min and max of two integers, respectively */
static int min(int a, int b) { return (a < b ? a : b); }
static int max(int a, int b) { return (a > b ? a : b); }

/*
 * initialize_pixel_sum - Initializes all fields of sum to 0
 */
static void initialize_pixel_sum(pixel_sum *sum)
{
    sum->red = sum->green = sum->blue = 0;
    sum->num = 0;
    return;
}

/*
 * accumulate_sum - Accumulates field values of p in corresponding
 * fields of sum
 */
static void accumulate_sum(pixel_sum *sum, pixel p)
{
    sum->red += (int)p.red;
    sum->green += (int)p.green;
    sum->blue += (int)p.blue;
    sum->num++;
    return;
}

/*
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum)
{
    current_pixel->red = (unsigned short)(sum.red / sum.num);
    current_pixel->green = (unsigned short)(sum.green / sum.num);
    current_pixel->blue = (unsigned short)(sum.blue / sum.num);
    return;
}

/*
 * avg - Returns averaged pixel value at (i,j)
 */
static pixel avg(int dim, int i, int j, pixel *src)
{
    int ii, jj;
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for (ii = max(i - 1, 0); ii <= min(i + 1, dim - 1); ii++)
        for (jj = max(j - 1, 0); jj <= min(j + 1, dim - 1); jj++)
            accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);

    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}

// static pixel avg_optim(int dim, int i, int j, pixel *src)
// {
//     int ii, jj, ii_max = min(i + 1, dim - 1), jj_max = min(j + 1, dim - 1);
//     pixel_sum sum;
//     pixel current_pixel;

//     initialize_pixel_sum(&sum);
//     for (ii = max(i - 1, 0); ii <= ii_max; ii++)
//         for (jj = max(j - 1, 0); jj <= jj_max; jj++)
//             accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);

//     assign_sum_to_pixel(&current_pixel, sum);
//     return current_pixel;
// }

/******************************************************
 * Your different versions of the smooth kernel go here
 ******************************************************/

/*
 * naive_smooth - The naive baseline version of smooth
 */
char naive_smooth_descr[] = "naive_smooth: Naive baseline implementation";
void naive_smooth(int dim, pixel *src, pixel *dst)
{
    int i, j;

    for (i = 0; i < dim; i++)
        for (j = 0; j < dim; j++)
            dst[RIDX(i, j, dim)] = avg(dim, i, j, src);
}

/*
 * smooth - Your current working version of smooth.
 * IMPORTANT: This is the version you will be graded on
 */
char smooth_descr[] = "smooth: divide into three classes";
void smooth(int dim, pixel *src, pixel *dst)
{
    smooth2(dim, src, dst);
}

char smooth_descr2[] = "smooth2: divide into three classes";
void smooth2(int dim, pixel *src, pixel *dst)
{
    int i, j, midmax = dim - 1;
    // 中间，9元素取均值
    for (i = 1; i < midmax; ++i)
    {
        int index_row1 = (i - 1) * dim;
        int index_row2 = i * dim;
        int index_row3 = (i + 1) * dim;
        for (j = 1; j < midmax; ++j)
        {
            int blue = 0, red = 0, green = 0;
            int index1_1 = index_row1 + j - 1;
            int index1_2 = index1_1 + 1;
            int index1_3 = index1_2 + 1;

            int index2_1 = index_row2 + j - 1;
            int index2_2 = index2_1 + 1;
            int index2_3 = index2_2 + 1;

            int index3_1 = index_row3 + j - 1;
            int index3_2 = index3_1 + 1;
            int index3_3 = index3_2 + 1;
            blue += src[index1_1].blue;
            red += src[index1_1].red;
            green += src[index1_1].green;

            blue += src[index1_2].blue;
            red += src[index1_2].red;
            green += src[index1_2].green;

            blue += src[index1_3].blue;
            red += src[index1_3].red;
            green += src[index1_3].green;

            blue += src[index2_1].blue;
            red += src[index2_1].red;
            green += src[index2_1].green;

            blue += src[index2_2].blue;
            red += src[index2_2].red;
            green += src[index2_2].green;

            blue += src[index2_3].blue;
            red += src[index2_3].red;
            green += src[index2_3].green;

            blue += src[index3_1].blue;
            red += src[index3_1].red;
            green += src[index3_1].green;

            blue += src[index3_2].blue;
            red += src[index3_2].red;
            green += src[index3_2].green;

            blue += src[index3_3].blue;
            red += src[index3_3].red;
            green += src[index3_3].green;

            dst[index2_2].blue = (unsigned short)(blue / 9);
            dst[index2_2].red = (unsigned short)(red / 9);
            dst[index2_2].green = (unsigned short)(green / 9);
        }
    }

    // 上边，6元素取均值
    for (j = 1; j < midmax; ++j)
    {
        int red = 0, green = 0, blue = 0;
        int index1_1 = j - 1;
        int index1_2 = j;
        int index1_3 = j + 1;
        int index2_1 = dim + j - 1;
        int index2_2 = index2_1 + 1;
        int index2_3 = index2_2 + 1;
        // src[RIDX(0, j - 1, dim)] + src[RIDX(0, j, dim)] + src[RIDX(0, j + 1, dim)]
        // src[RIDX(1, j - 1, dim)] + src[RIDX(1, j, dim)] + src[RIDX(1, j + 1, dim)]
        red += src[index1_1].red;
        green += src[index1_1].green;
        blue += src[index1_1].blue;

        red += src[index1_2].red;
        green += src[index1_2].green;
        blue += src[index1_2].blue;

        red += src[index1_3].red;
        green += src[index1_3].green;
        blue += src[index1_3].blue;

        red += src[index2_1].red;
        green += src[index2_1].green;
        blue += src[index2_1].blue;

        red += src[index2_2].red;
        green += src[index2_2].green;
        blue += src[index2_2].blue;

        red += src[index2_3].red;
        green += src[index2_3].green;
        blue += src[index2_3].blue;

        dst[index1_2].red = (unsigned short)(red / 6);
        dst[index1_2].green = (unsigned short)(green / 6);
        dst[index1_2].blue = (unsigned short)(blue / 6);
    }

    // 下边，6元素取均值
    for (j = 1; j < midmax; ++j)
    {
        int red = 0, green = 0, blue = 0;
        int index1_1 = (dim - 2) * dim + j - 1;
        int index1_2 = index1_1 + 1;
        int index1_3 = index1_2 + 1;
        int index2_1 = (dim - 1) * dim + j - 1;
        int index2_2 = index2_1 + 1;
        int index2_3 = index2_2 + 1;
        // src[RIDX(dim - 2, j - 1, dim)] + src[RIDX(dim - 2, j, dim)] + src[RIDX(dim - 2, j + 1, dim)]
        // src[RIDX(dim - 1, j - 1, dim)] + src[RIDX(dim - 1, j, dim)] + src[RIDX(dim - 1, j + 1, dim)]
        red += src[index1_1].red;
        green += src[index1_1].green;
        blue += src[index1_1].blue;

        red += src[index1_2].red;
        green += src[index1_2].green;
        blue += src[index1_2].blue;

        red += src[index1_3].red;
        green += src[index1_3].green;
        blue += src[index1_3].blue;

        red += src[index2_1].red;
        green += src[index2_1].green;
        blue += src[index2_1].blue;

        red += src[index2_2].red;
        green += src[index2_2].green;
        blue += src[index2_2].blue;

        red += src[index2_3].red;
        green += src[index2_3].green;
        blue += src[index2_3].blue;

        dst[index2_2].red = (unsigned short)(red / 6);
        dst[index2_2].green = (unsigned short)(green / 6);
        dst[index2_2].blue = (unsigned short)(blue / 6);
    }

    // 左边，6元素取均值
    for (i = 1; i < midmax; ++i)
    {
        int red = 0, green = 0, blue = 0;
        int index1_1 = (i - 1) * dim;
        int index1_2 = index1_1 + 1;
        int index2_1 = (i * dim);
        int index2_2 = index2_1 + 1;
        int index3_1 = (i + 1) * dim;
        int index3_2 = index3_1 + 1;
        // src[RIDX(i - 1, 0, dim)] + src[RIDX(i - 1, 1, dim)]
        // src[RIDX(i, 0, dim)] + src[RIDX(i, 1, dim)]
        // src[RIDX(i + 1, 0, dim)] + src[RIDX(i + 1, 1, dim)]
        red += src[index1_1].red;
        green += src[index1_1].green;
        blue += src[index1_1].blue;

        red += src[index1_2].red;
        green += src[index1_2].green;
        blue += src[index1_2].blue;

        red += src[index2_1].red;
        green += src[index2_1].green;
        blue += src[index2_1].blue;

        red += src[index2_2].red;
        green += src[index2_2].green;
        blue += src[index2_2].blue;

        red += src[index3_1].red;
        green += src[index3_1].green;
        blue += src[index3_1].blue;

        red += src[index3_2].red;
        green += src[index3_2].green;
        blue += src[index3_2].blue;

        dst[index2_1].red = (unsigned short)(red / 6);
        dst[index2_1].green = (unsigned short)(green / 6);
        dst[index2_1].blue = (unsigned short)(blue / 6);
    }

    // 右边，6元素取均值
    for (i = 1; i < midmax; ++i)
    {
        int red = 0, green = 0, blue = 0;
        int index1_1 = (i - 1) * dim + dim - 2;
        int index1_2 = index1_1 + 1;
        int index2_1 = (i * dim) + dim - 2;
        int index2_2 = index2_1 + 1;
        int index3_1 = (i + 1) * dim + dim - 2;
        int index3_2 = index3_1 + 1;
        // src[RIDX(i - 1, dim - 2, dim)] + src[RIDX(i - 1, dim - 1, dim)]
        // src[RIDX(i, dim - 2, dim)] + src[RIDX(i, dim - 1, dim)]
        // src[RIDX(i + 1, dim - 2, dim)] + src[RIDX(i + 1, dim - 1, dim)]
        red += src[index1_1].red;
        green += src[index1_1].green;
        blue += src[index1_1].blue;

        red += src[index1_2].red;
        green += src[index1_2].green;
        blue += src[index1_2].blue;

        red += src[index2_1].red;
        green += src[index2_1].green;
        blue += src[index2_1].blue;

        red += src[index2_2].red;
        green += src[index2_2].green;
        blue += src[index2_2].blue;

        red += src[index3_1].red;
        green += src[index3_1].green;
        blue += src[index3_1].blue;

        red += src[index3_2].red;
        green += src[index3_2].green;
        blue += src[index3_2].blue;

        dst[index2_2].red = (unsigned short)(red / 6);
        dst[index2_2].green = (unsigned short)(green / 6);
        dst[index2_2].blue = (unsigned short)(blue / 6);
    }

    // 四角，4元素取均值
    //  src[RIDX(0, 0, dim)] + src[RIDX(0, 1, dim)]
    //  src[RIDX(1, 0, dim)] + src[RIDX(1, 1, dim)]
    dst[0].red = (unsigned short)((src[0].red + src[1].red + src[dim].red + src[dim + 1].red) / 4);
    dst[0].green = (unsigned short)((src[0].green + src[1].green + src[dim].green + src[dim + 1].green) / 4);
    dst[0].blue = (unsigned short)((src[0].blue + src[1].blue + src[dim].blue + src[dim + 1].blue) / 4);

    // src[RIDX(0, dim - 2, dim)] + src[RIDX(0, dim - 1, dim)]
    // src[RIDX(1, dim - 2, dim)] + src[RIDX(1, dim - 1, dim)]
    dst[dim - 1].red = (unsigned short)((src[dim - 2].red + src[dim - 1].red + src[2 * dim - 2].red + src[2 * dim - 1].red) / 4);
    dst[dim - 1].green = (unsigned short)((src[dim - 2].green + src[dim - 1].green + src[2 * dim - 2].green + src[2 * dim - 1].green) / 4);
    dst[dim - 1].blue = (unsigned short)((src[dim - 2].blue + src[dim - 1].blue + src[2 * dim - 2].blue + src[2 * dim - 1].blue) / 4);

    // src[RIDX(dim - 2, 0, dim)] + src[RIDX(dim - 2, 1, dim)]
    // src[RIDX(dim - 1, 0, dim)] + src[RIDX(dim - 1, 1, dim)]
    dst[(dim - 1) * dim].red = (unsigned short)((src[(dim - 2) * dim].red + src[(dim - 2) * dim + 1].red + src[(dim - 1) * dim].red + src[(dim - 1) * dim + 1].red) / 4);
    dst[(dim - 1) * dim].green = (unsigned short)((src[(dim - 2) * dim].green + src[(dim - 2) * dim + 1].green + src[(dim - 1) * dim].green + src[(dim - 1) * dim + 1].green) / 4);
    dst[(dim - 1) * dim].blue = (unsigned short)((src[(dim - 2) * dim].blue + src[(dim - 2) * dim + 1].blue + src[(dim - 1) * dim].blue + src[(dim - 1) * dim + 1].blue) / 4);

    // src[RIDX(dim - 2, dim - 2, dim)] + src[RIDX(dim - 2, dim - 1, dim)]
    // src[RIDX(dim - 1, dim - 2, dim)] + src[RIDX(dim - 1, dim - 1, dim)]
    dst[(dim - 1) * dim + dim - 1].red = (unsigned short)((src[(dim - 2) * dim + dim - 2].red + src[(dim - 2) * dim + dim - 1].red + src[(dim - 1) * dim + dim - 2].red + src[(dim - 1) * dim + dim - 1].red) / 4);
    dst[(dim - 1) * dim + dim - 1].green = (unsigned short)((src[(dim - 2) * dim + dim - 2].green + src[(dim - 2) * dim + dim - 1].green + src[(dim - 1) * dim + dim - 2].green + src[(dim - 1) * dim + dim - 1].green) / 4);
    dst[(dim - 1) * dim + dim - 1].blue = (unsigned short)((src[(dim - 2) * dim + dim - 2].blue + src[(dim - 2) * dim + dim - 1].blue + src[(dim - 1) * dim + dim - 2].blue + src[(dim - 1) * dim + dim - 1].blue) / 4);
}

char smooth_descr3[] = "smooth: Current working version";
void smooth3(int dim, pixel *src, pixel *dst) 
{
    int i,j;
    //no using avg()

    //corners
    dst[0].red=(src[0].red+src[dim].red+src[1].red+src[dim+1].red)>>2;
    dst[0].blue=(src[0].blue+src[dim].blue+src[1].blue+src[dim+1].blue)>>2;
    dst[0].green=(src[0].green+src[dim].green+src[1].green+src[dim+1].green)>>2;

    dst[RIDX(0,dim-1,dim)].red=(src[RIDX(0,dim-1,dim)].red+src[RIDX(1,dim-1,dim)].red+src[RIDX(0,dim-2,dim)].red+src[RIDX(1,dim-2,dim)].red)>>2;
    dst[RIDX(0,dim-1,dim)].blue=(src[RIDX(0,dim-1,dim)].blue+src[RIDX(1,dim-1,dim)].blue+src[RIDX(0,dim-2,dim)].blue+src[RIDX(1,dim-2,dim)].blue)>>2;
    dst[RIDX(0,dim-1,dim)].green=(src[RIDX(0,dim-1,dim)].green+src[RIDX(1,dim-1,dim)].green+src[RIDX(0,dim-2,dim)].green+src[RIDX(1,dim-2,dim)].green)>>2;

    dst[RIDX(dim-1,0,dim)].red=(src[RIDX(dim-1,0,dim)].red+src[RIDX(dim-2,0,dim)].red+src[RIDX(dim-1,1,dim)].red+src[RIDX(dim-2,1,dim)].red)>>2;
    dst[RIDX(dim-1,0,dim)].blue=(src[RIDX(dim-1,0,dim)].blue+src[RIDX(dim-2,0,dim)].blue+src[RIDX(dim-1,1,dim)].blue+src[RIDX(dim-2,1,dim)].blue)>>2;
    dst[RIDX(dim-1,0,dim)].green=(src[RIDX(dim-1,0,dim)].green+src[RIDX(dim-2,0,dim)].green+src[RIDX(dim-1,1,dim)].green+src[RIDX(dim-2,1,dim)].green)>>2;

    dst[RIDX(dim-1,dim-1,dim)].red=(src[RIDX(dim-1,dim-1,dim)].red+src[RIDX(dim-1,dim-2,dim)].red+src[RIDX(dim-2,dim-1,dim)].red+src[RIDX(dim-2,dim-2,dim)].red)>>2;
    dst[RIDX(dim-1,dim-1,dim)].blue=(src[RIDX(dim-1,dim-1,dim)].blue+src[RIDX(dim-1,dim-2,dim)].blue+src[RIDX(dim-2,dim-1,dim)].blue+src[RIDX(dim-2,dim-2,dim)].blue)>>2;
    dst[RIDX(dim-1,dim-1,dim)].green=(src[RIDX(dim-1,dim-1,dim)].green+src[RIDX(dim-1,dim-2,dim)].green+src[RIDX(dim-2,dim-1,dim)].green+src[RIDX(dim-2,dim-2,dim)].green)>>2;

    //boarder
    for(i=1;i<dim-1;i++){
        int pos=i*dim;
        dst[pos].red=(src[pos].red+src[pos-dim].red+src[pos-dim+1].red+src[pos+1].red+src[pos+dim].red+src[pos+dim+1].red)/6;
        dst[pos].blue=(src[pos].blue+src[pos-dim].blue+src[pos-dim+1].blue+src[pos+1].blue+src[pos+dim].blue+src[pos+dim+1].blue)/6;
        dst[pos].green=(src[pos].green+src[pos-dim].green+src[pos-dim+1].green+src[pos+1].green+src[pos+dim].green+src[pos+dim+1].green)/6;
    }

    for(i=1;i<dim-1;i++){
        int pos=i*dim;
        dst[pos+dim-1].red=(src[pos+dim-1].red+src[pos-1].red+src[pos-2].red+src[pos-2+dim].red+src[pos+dim+dim-1].red+src[pos+dim+dim-2].red)/6;
        dst[pos+dim-1].blue=(src[pos+dim-1].blue+src[pos-1].blue+src[pos-2].blue+src[pos-2+dim].blue+src[pos+dim+dim-1].blue+src[pos+dim+dim-2].blue)/6;
        dst[pos+dim-1].green=(src[pos+dim-1].green+src[pos-1].green+src[pos-2].green+src[pos-2+dim].green+src[pos+dim+dim-1].green+src[pos+dim+dim-2].green)/6;
    }

    for(j=1;j<dim-1;j++){
        int pos=j;
        dst[pos].red=(src[pos].red+src[pos-1].red+src[RIDX(1,j-1,dim)].red+src[RIDX(1,j,dim)].red+src[RIDX(0,j+1,dim)].red+src[RIDX(1,j+1,dim)].red)/6;
        dst[pos].blue=(src[pos].blue+src[pos-1].blue+src[RIDX(1,j-1,dim)].blue+src[RIDX(1,j,dim)].blue+src[RIDX(0,j+1,dim)].blue+src[RIDX(1,j+1,dim)].blue)/6;
        dst[pos].green=(src[pos].green+src[pos-1].green+src[RIDX(1,j-1,dim)].green+src[RIDX(1,j,dim)].green+src[RIDX(0,j+1,dim)].green+src[RIDX(1,j+1,dim)].green)/6;
    }

    for(j=1;j<dim-1;j++){
        int pos=j+dim*(dim-1);
        dst[pos].red=(src[pos].red+src[pos+1].red+src[pos-1].red+src[pos-dim].red+src[pos-dim+1].red+src[pos-dim-1].red)/6;
        dst[pos].blue=(src[pos].blue+src[pos+1].blue+src[pos-1].blue+src[pos-dim].blue+src[pos-dim+1].blue+src[pos-dim-1].blue)/6;
        dst[pos].green=(src[pos].green+src[pos+1].green+src[pos-1].green+src[pos-dim].green+src[pos-dim+1].green+src[pos-dim-1].green)/6;
    }

    //common
    for(i=1;i<dim-1;i+=2)
        for(j=1;j<dim-1;j++){
            int pos=i*dim+j;
            dst[pos].red=(src[pos].red+src[pos+dim].red+src[pos-dim].red+src[pos-1].red+src[pos+dim-1].red+src[pos-dim-1].red+src[pos+1].red+src[pos+1+dim].red+src[pos+1-dim].red)/9;
            dst[pos].blue=(src[pos].blue+src[pos+dim].blue+src[pos-dim].blue+src[pos-1].blue+src[pos+dim-1].blue+src[pos-dim-1].blue+src[pos+1].blue+src[pos+1+dim].blue+src[pos+1-dim].blue)/9;
            dst[pos].green=(src[pos].green+src[pos+dim].green+src[pos-dim].green+src[pos-1].green+src[pos+dim-1].green+src[pos-dim-1].green+src[pos+1].green+src[pos+1+dim].green+src[pos+1-dim].green)/9;

            pos+=dim;

            dst[pos].red=(src[pos].red+src[pos+dim].red+src[pos-dim].red+src[pos-1].red+src[pos+dim-1].red+src[pos-dim-1].red+src[pos+1].red+src[pos+1+dim].red+src[pos+1-dim].red)/9;
            dst[pos].blue=(src[pos].blue+src[pos+dim].blue+src[pos-dim].blue+src[pos-1].blue+src[pos+dim-1].blue+src[pos-dim-1].blue+src[pos+1].blue+src[pos+1+dim].blue+src[pos+1-dim].blue)/9;
            dst[pos].green=(src[pos].green+src[pos+dim].green+src[pos-dim].green+src[pos-1].green+src[pos+dim-1].green+src[pos-dim-1].green+src[pos+1].green+src[pos+1+dim].green+src[pos+1-dim].green)/9;
        }
}

/*********************************************************************
 * register_smooth_functions - Register all of your different versions
 *     of the smooth kernel with the driver by calling the
 *     add_smooth_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.
 *********************************************************************/

void register_smooth_functions()
{
    add_smooth_function(&naive_smooth, naive_smooth_descr);
    add_smooth_function(&smooth, smooth_descr);

    /* ... Register additional test functions here */
}
