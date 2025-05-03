#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>

#define SHM_NAME   "/rushgo_shm"

typedef struct {
    char name[50];
    char address[100];
    char type;  
    int  delivered; 
    char agent[20];
} Order;

void log_delivery(const char *agent, const Order *o) {
    FILE *f = fopen("delivery.log", "a");
    if (!f) return;
    time_t t = time(NULL);
    struct tm tm;
    localtime_r(&t, &tm);
    char buf[64];
    strftime(buf, sizeof(buf), "[%d/%m/%Y %H:%M:%S]", &tm);
    if (o->type == 'R') {
        fprintf(f, "%s [%s] Reguler package delivered to %s in %s\n",
                buf, agent, o->name, o->address);
    }
    fclose(f);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s -deliver <Nama> | -status <Nama> | -list\n", argv[0]);
        return 1;
    }

    int fd = shm_open(SHM_NAME, O_RDWR, 0);
    if (fd < 0) { perror("shm_open"); return 1; }

    struct stat sb;
    fstat(fd, &sb);
    size_t shm_size = sb.st_size;
    void *ptr = mmap(NULL, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) { perror("mmap"); return 1; }

    int *num_orders = ptr;
    Order *orders = ptr + sizeof(int);

    if (strcmp(argv[1], "-deliver") == 0) {
        if (argc != 3) {
            fprintf(stderr, "Usage: %s -deliver <Nama>\n", argv[0]);
            return 1;
        }
        const char *who = getenv("USER");
        char agent_str[30];
        snprintf(agent_str, sizeof(agent_str), "AGENT %s", who ? who : "UNKNOWN");
        const char *target = argv[2];
        int found = 0;
        for (int i = 0; i < *num_orders; i++) {
            if (orders[i].type == 'R'
             && strcmp(orders[i].name, target) == 0
             && orders[i].delivered == 0) {
                orders[i].delivered = 1;
                strcpy(orders[i].agent, agent_str);
                log_delivery(agent_str, &orders[i]);
                printf("Delivered Reguler package to %s by %s\n", target, agent_str);
                found = 1;
                break;
            }
        }
        if (!found) {
            printf("Order '%s' not found or already delivered.\n", target);
        }

    } else if (strcmp(argv[1], "-status") == 0) {
        if (argc != 3) {
            fprintf(stderr, "Usage: %s -status <Nama>\n", argv[0]);
            return 1;
        }
        const char *target = argv[2];
        int found = 0;
        for (int i = 0; i < *num_orders; i++) {
            if (strcmp(orders[i].name, target) == 0) {
                found = 1;
                if (orders[i].delivered) {
                    printf("Status for %s: Delivered by %s\n",
                           target, orders[i].agent);
                } else {
                    printf("Status for %s: Pending\n", target);
                }
                break;
            }
        }
        if (!found) {
            printf("Order '%s' not found.\n", target);
        }

    } else if (strcmp(argv[1], "-list") == 0) {
        printf("Daftar semua pesanan:\n");
        for (int i = 0; i < *num_orders; i++) {
            const char *type = (orders[i].type == 'E') ? "Express" : "Reguler";
            if (orders[i].delivered) {
                printf("- %s (%s): Delivered by %s\n",
                       orders[i].name, type, orders[i].agent);
            } else {
                printf("- %s (%s): Pending\n",
                       orders[i].name, type);
            }
        }

    } else {
        fprintf(stderr, "Unknown command '%s'\n", argv[1]);
    }

    munmap(ptr, shm_size);
    close(fd);
    return 0;
}
