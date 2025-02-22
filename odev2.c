#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// G�nl�k bilgilerini depolayan g�nl�k d�g�mleri i�in yapi
// Her d�g�m�n bir zaman damgasi, seviyesi, mesaji ve bir sonraki d�g�me bir isaret�isi vardir
typedef struct LogNode {
    char timestamp[25];  // G�nl�g�n olusturuldugu zaman
    char level[10];      // G�nl�k seviyesi (INFO, WARNING, ERROR, vb.)
    char message[256];   // G�nl�k mesaji
    struct LogNode* next; // Bagli listedeki bir sonraki d�g�me isaret�i
} LogNode;

// G�nl�kleri FIFO seklinde saklayan dairesel bagli liste yapisi
typedef struct {
    LogNode* head;  // Listedeki ilk elemana isaret�i
    LogNode* tail;  // Listedeki son �geye isaret�i
    int size;       // Ge�erli listedeki �ge sayisi
    int maxSize;    // �zerine yazmadan �nce listenin maksimum kapasitesi
} CircularList;

// Dairesel bagli listeyi baslatma islevi
CircularList* createList(int maxSize) {
    CircularList* list = (CircularList*)malloc(sizeof(CircularList)); // Listeler i�in bellek tahsisi
    if (list == NULL) {
        printf("Failed to allocate memory for the list\n");
        exit(1); // Basarisiz olursa programdan �ikin
    }
    list->head = NULL;  // Basligi NULL olarak baslatin
    list->tail = NULL;  // Kuyrugu NULL olarak baslatin
    list->size = 0;     // Liste boyutunu 0 olarak baslatin
    list->maxSize = maxSize; // Maksimum kapasite listesini kaydetme
    return list;
}

// Dairesel baglantili listeye yeni bir g�nl�k ekleme islevi
void addLog(CircularList* list, const char* timestamp, const char* level, const char* message) {
    LogNode* newNode = (LogNode*)malloc(sizeof(LogNode)); // Yeni d�g�mler i�in bellek tahsisi
    if (newNode == NULL) {
        printf("Failed to allocate memory for the new node\n");
        return;
    }

    // Tasmayi �nlemek i�in tampon boyut kisitlamasiyla yeni bir d�g�me veri kopyalama
    strncpy(newNode->timestamp, timestamp, 24);
    newNode->timestamp[24] = '\0';
    strncpy(newNode->level, level, 9);
    newNode->level[9] = '\0';
    strncpy(newNode->message, message, 255);
    newNode->message[255] = '\0';

    if (list->size == 0) {
        // Liste bossa, yeni d�g�m listedeki tek �ge olur
        newNode->next = newNode;  // D�g�m kendisine isaret eder
        list->head = newNode;
        list->tail = newNode;
        list->size = 1;
    } else if (list->size < list->maxSize) {
        // Liste dolu degilse, d�g�m� listenin sonuna ekleyin
        newNode->next = list->head; // Kafaya baglayin
        list->tail->next = newNode; // Son d�g�m g�ncellemesi
        list->tail = newNode; // G�ncelleme kuyrugu
        list->size++;
    } else {
        // Liste doluysa, yeni d�g�mlere yer a�mak i�in en eski d�g�m� (bas) kaldirin
        LogNode* oldHead = list->head;
        list->head = list->head->next; // Basi bir sonraki �geye tasi
        free(oldHead); // Eski d�g�mlerden bellek bosaltma
        
        // Listenin sonuna yeni bir d�g�m ekleyin
        newNode->next = list->head;
        list->tail->next = newNode;
        list->tail = newNode;
    }
}

// T�m g�nl�kleri bir listede g�r�nt�leme islevi
void displayLogs(CircularList* list) {
    if (list->size == 0) {
        printf("Empty log\n");
        return;
    }

    LogNode* current = list->head;
    int count = 0;

    printf("\nLog List:\n");
    do {
        printf("[%s] %s: %s\n", 
               current->timestamp,
               current->level,
               current->message);
        current = current->next;
        count++;
    } while (current != list->head && count < list->size);
}

// G�nl�kleri seviyeye g�re arama islevi
void findLogsByLevel(CircularList* list, const char* level) {
    if (list->size == 0) {
        printf("Empty log\n");
        return;
    }

    LogNode* current = list->head;
    int count = 0;
    int found = 0;

    printf("\nLog with level %s:\n", level);
    do {
        if (strcmp(current->level, level) == 0) {
            printf("[%s]: %s\n", 
                   current->timestamp,
                   current->message);
            found = 1;
        }
        current = current->next;
        count++;
    } while (current != list->head && count < list->size);

    if (!found) {
        printf("No logs found with level %s\n", level);
    }
}

// Kullanimdan sonra bellek listesini temizleme islevi
void freeList(CircularList* list) {
    if (list->size == 0) {
        free(list);
        return;
    }

    LogNode* current = list->head;
    LogNode* next;
    int count = 0;

    do {
        next = current->next;
        free(current);
        current = next;
        count++;
    } while (current != list->head && count < list->size);

    free(list);
}

int main() {
    // Maksimum 5 g�nl�k kapasiteli dairesel bir liste olusturun
    CircularList* logList = createList(5);

    // Bazi g�nl�kler eklendi
    addLog(logList, "2025-02-22 10:00:00", "ERROR", "Database connection failed");
    addLog(logList, "2025-02-22 10:01:00", "INFO", "Service started");
    addLog(logList, "2025-02-22 10:02:00", "WARNING", "High memory usage");
    addLog(logList, "2025-02-22 10:03:00", "ERROR", "Authentication failed");
    addLog(logList, "2025-02-22 10:04:00", "INFO", "Backup completed");

    // T�m g�nl�kleri g�r�nt�le
    printf("Display all logs after initial addition:\n");
    displayLogs(logList);

    // Belirli bir seviyeye sahip g�nl�kleri arama
    findLogsByLevel(logList, "ERROR");

    // Liste doldugunda yeni g�nl�kler ekleyin
    printf("\nAdd a new log when the list is full (will delete the oldest log):\n");
    addLog(logList, "2025-02-22 10:05:00", "WARNING", "Disk space low");
    displayLogs(logList);

    // Hafizanin temizlenmesi
    freeList(logList);

    return 0;
}
