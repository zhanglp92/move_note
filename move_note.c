/*************************************************************************
	> File Name: move_note.c
	> Author: zhanglp
	> Mail: 820221185@qq.com 
	> Created Time: 2014年03月06日 星期四 08时07分57秒
 ************************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

void remove_comment (char *buf, size_t size)
{
    char    *p, *end, c;
    char    *xx_start = NULL, *xh_start = NULL;  // 斜线、星号的起始地址 
    char    *xxxh_start = NULL, *xxxh_end = NULL, *xxxx_start = NULL; // 斜线注释和星号注释的起始地址
    size_t  len;

    end = (p = buf) + size; 
    while (p < end) { 
        switch (*p) { 
            case '/': { 
                if (!xx_start) // 第一个出现的斜线 
                    xx_start = p; 
                else if (p-1 == xx_start && '/' == *xx_start) { // 斜线注释的起始地址 
                    xxxx_start = xx_start; 
                    char *q = strchr (xxxx_start+2, '\n'); 
                    if (q == 0) 
                        memset (xxxx_start, ' ', strlen (xxxh_start)); 
                    else 
                        memset (xxxx_start, ' ', q - xxxx_start); 
                    xxxx_start = xx_start = NULL;

                }
                else if (xxxh_start && '*' == *(p-1) && xxxh_start+1 < p-1) { // 星号注释的结束地址 
                    xxxh_end = p; 
                    memset (xxxh_start, ' ', xxxh_end-xxxh_start+1); 
                    xx_start = xh_start = xxxh_start = xxxh_end = NULL; 
                } 
            }break;

            case '*': { 
                if (p-1 == xx_start) // 星号注释的起始地址 
                    xxxh_start = xx_start; 
            }break; 
        }
        p++;
    } 
}

int main (void)
{
    int     fd, n;
    char    buf[102400];

    /* 为了方便测试，固定处理下面的文件（可以自己存主函数的行参中传入文件名） */
    if ((fd = open ("./fuzhi.c", O_RDONLY, 0)) == -1) 
        return -1;

    n = read (fd, buf, sizeof (buf));
    if (n == -1 || !n) {
        close (fd);
        return -1;
    }

    remove_comment (buf, n);
    /* 处理过的字符串，可自行处理（打印或存储别的文件） */
    *(buf+n) = 0;
    printf ("%s", buf);
    close (fd);

    return 0;
}
