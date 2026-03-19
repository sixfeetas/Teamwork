#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LEN 4096

/* Replace all occurrences of `word` in `src` with stars, write result to `dst`.
 * `dst_size` is the size of the dst buffer.
 * Returns the number of replacements made, or -1 on buffer overflow. */
static int replace_word(const char *src, char *dst, size_t dst_size, const char *word) {
    int count = 0;
    size_t word_len = strlen(word);
    size_t dst_pos = 0;
    const char *p = src;

    while (*p) {
        if (strncmp(p, word, word_len) == 0) {
            if (dst_pos + word_len >= dst_size) return -1;
            for (size_t i = 0; i < word_len; i++) {
                dst[dst_pos++] = '*';
            }
            p += word_len;
            count++;
        } else {
            if (dst_pos + 1 >= dst_size) return -1;
            dst[dst_pos++] = *p++;
        }
    }
    dst[dst_pos] = '\0';
    return count;
}

/* Filter sensitive words in `text` using the given `words` list (size `n`).
 * Returns a newly allocated string with all sensitive words replaced by stars.
 * Logs each replaced word to stdout. Caller must free the returned string. */
char *filter_sensitive_words(const char *text, const char **words, int n) {
    char *buf1 = malloc(MAX_LEN);
    char *buf2 = malloc(MAX_LEN);
    if (!buf1 || !buf2) {
        free(buf1);
        free(buf2);
        return NULL;
    }

    strncpy(buf1, text, MAX_LEN - 1);
    buf1[MAX_LEN - 1] = '\0';

    for (int i = 0; i < n; i++) {
        int count = replace_word(buf1, buf2, MAX_LEN, words[i]);
        if (count < 0) {
            fprintf(stderr, "[ERROR] 缓冲区溢出，文本过长\n");
            free(buf1);
            free(buf2);
            return NULL;
        }
        if (count > 0) {
            printf("[LOG] 替换敏感词 \"%s\"，共替换 %d 次\n", words[i], count);
        }
        /* swap buffers */
        char *tmp = buf1;
        buf1 = buf2;
        buf2 = tmp;
    }

    free(buf2);
    return buf1;
}

int main(void) {
    const char *text = "这个坏人真的很坏，他是个骗子，还是个傻瓜。";
    const char *words[] = {"坏人", "骗子", "傻瓜"};
    int n = sizeof(words) / sizeof(words[0]);

    printf("原始文本：%s\n", text);

    char *result = filter_sensitive_words(text, words, n);
    if (result) {
        printf("过滤结果：%s\n", result);
        free(result);
    }

    return 0;
}
