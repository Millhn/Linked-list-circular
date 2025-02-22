# Circular Syslog Logger

Bu proje, bir dairesel bağlı liste (circular linked list) kullanarak sistem günlüklerini (logs) saklayan bir uygulamadır. FIFO (First-In-First-Out) mantığıyla çalışan bu yapı, belirlenen maksimum kapasiteye ulaştığında en eski veriyi silerek yeni veriye yer açar.

## Özellikler
- **Dairesel Bağlı Liste (Circular Linked List)** kullanımı
- **FIFO (First-In-First-Out)** prensibine göre log saklama
- **Dinamik Bellek Yönetimi** ile log ekleme ve kaldırma
- **Seviyeye Göre Filtreleme** özelliği
- **Belleği Temizleme** fonksiyonu

## Kullanılan Değişkenler

### Yapılar (Structs)

#### `LogNode`
Bir log kaydını temsil eden düğüm yapısıdır.
- `timestamp` (**char[25]**): Log’un oluşturulduğu zaman damgası.
- `level` (**char[10]**): Log seviyesi (**INFO, WARNING, ERROR** vb.).
- `message` (**char[256]**): Log mesajı.
- `next` (**LogNode***): Bir sonraki düğümü işaret eden gösterici.

#### `CircularList`
Dairesel bağlı listeyi temsil eden yapı.
- `head` (**LogNode***): Listenin başını işaret eder.
- `tail` (**LogNode***): Listenin sonunu işaret eder.
- `size` (**int**): Mevcut eleman sayısını saklar.
- `maxSize` (**int**): Maksimum kapasiteyi belirler.

## Fonksiyonlar

### `CircularList* createList(int maxSize)`
Dairesel bağlı listeyi oluşturur ve maksimum kapasitesini belirler.

### `void addLog(CircularList* list, const char* timestamp, const char* level, const char* message)`
Yeni bir log ekler. Eğer liste doluysa en eski log'u silerek yenisine yer açar.

### `void displayLogs(CircularList* list)`
Listede bulunan tüm logları ekrana yazdırır.

### `void findLogsByLevel(CircularList* list, const char* level)`
Belirtilen seviyeye sahip logları arar ve ekrana yazdırır.

### `void freeList(CircularList* list)`
Bellekte ayrılan tüm logları temizleyerek listeyi serbest bırakır.

## Kullanım

```c
int main() {
    CircularList* logList = createList(5);

    addLog(logList, "2025-02-22 10:00:00", "ERROR", "Database connection failed");
    addLog(logList, "2025-02-22 10:01:00", "INFO", "Service started");
    addLog(logList, "2025-02-22 10:02:00", "WARNING", "High memory usage");
    addLog(logList, "2025-02-22 10:03:00", "ERROR", "Authentication failed");
    addLog(logList, "2025-02-22 10:04:00", "INFO", "Backup completed");

    displayLogs(logList);
    findLogsByLevel(logList, "ERROR");

    addLog(logList, "2025-02-22 10:05:00", "WARNING", "Disk space low");
    displayLogs(logList);

    freeList(logList);
    return 0;
}
```

## Gereksinimler
- C derleyicisi (GCC, Clang vb.)
- Linux veya Windows işletim sistemi

## Derleme ve Çalıştırma

```sh
gcc -o circular_syslog circular_syslog.c
./circular_syslog
```

## Lisans
Bu proje MIT Lisansı ile lisanslanmıştır.

