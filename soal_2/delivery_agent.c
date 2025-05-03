#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define SHM_NAME   "/rushgo_shm"
#define MAX_ORDERS 1000

typedef struct {
    char name[50];
    char address[100];
    char type;      
    int  delivered;  
    char agent[20];  
} Order;

static Order *orders = NULL;
static int    *num_orders = NULL;
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static int next_agent = 0;
const char *agent_names[3] = { "AGENT A", "AGENT B", "AGENT C" };

void log_delivery(const char *agent, const Order *o) {
    FILE *f = fopen("delivery.log", "a");
    if (!f) return;
    time_t t = time(NULL);
    struct tm tm;
    localtime_r(&t, &tm);
    char buf[64];
    strftime(buf, sizeof(buf), "[%d/%m/%Y %H:%M:%S]", &tm);
    if (o->type == 'E') {
        fprintf(f, "%s [%s] Express package delivered to %s in %s\n",
                buf, agent, o->name, o->address);
    }
    fclose(f);
}

void *agent_thread(void *arg) {
    int id = *(int*)arg;
    free(arg);
    while (1) {
        pthread_mutex_lock(&mtx);
        int idx = -1;
        for (int i = 0; i < *num_orders; i++) {
            if (orders[i].type == 'E' && orders[i].delivered == 0) {
                idx = i;
                break;
            }
        }
        if (idx < 0) {
            pthread_mutex_unlock(&mtx);
            break;
        }
        orders[idx].delivered = 1;
        strcpy(orders[idx].agent, agent_names[next_agent]);
        next_agent = (next_agent + 1) % 3;
        pthread_mutex_unlock(&mtx);
        log_delivery(orders[idx].agent, &orders[idx]);
    }
    return NULL;
}

int main() {
    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (fd < 0) { perror("shm_open"); exit(1); }
    FILE *csv = fopen("delivery_order.csv", "r");
    if (!csv) { perror("fopen CSV"); exit(1); }
    char line[256];
    Order temp[MAX_ORDERS];
    int count = 0;
    if (fgets(line, sizeof(line), csv) && strchr(line, ',')) {
    } else {
        rewind(csv);
    }
    while (fgets(line, sizeof(line), csv) && count < MAX_ORDERS) {
        char *p = strtok(line, ",");
        if (!p) continue;
        strncpy(temp[count].name, p, 49);
        p = strtok(NULL, ",");
        strncpy(temp[count].address, p ? p : "", 99);
        p = strtok(NULL, ",");
        temp[count].type = p ? p[0] : 'R';
        temp[count].delivered = 0;
        temp[count].agent[0] = '\0';
        count++;
    }
    fclose(csv);

    size_t shm_size = sizeof(int) + sizeof(Order) * count;
    if (ftruncate(fd, shm_size) < 0) { perror("ftruncate"); exit(1); }

    void *ptr = mmap(NULL, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) { perror("mmap"); exit(1); }
    num_orders = ptr;
    orders     = ptr + sizeof(int);

    *num_orders = count;
    memcpy(orders, temp, sizeof(Order) * count);

    printf("✅ Berhasil menyimpan %d order ke shared memory.\n", count);
  
    pthread_t th[3];
    for (int i = 0; i < 3; i++) {
        int *arg = malloc(sizeof(int));
        *arg = i;
        pthread_create(&th[i], NULL, agent_thread, arg);
    }
    for (int i = 0; i < 3; i++) {
        pthread_join(th[i], NULL);
    }
  
    munmap(ptr, shm_size);
    close(fd);
    printf("Semua pesanan Express telah diproses.\n");
    return 0;
}

