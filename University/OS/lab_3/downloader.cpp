#include <iostream>
#include <string>
#include <thread> // Многопоточность
#include <atomic> // Атомарные операции 
#include <cstdlib>  // Для функции exit()
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h> // Для ftruncate
#include <sys/mman.h> // Для отображения файла в память
#include <semaphore.h>

class DownloadManager {
public:
    DownloadManager() : log_fd(-1), shm_fd(-1), counter(nullptr) {
        initialize_resources();
    }

    ~DownloadManager() {
        cleanup_resources();
    }

    void download(const std::string& url) {
        std::thread(&DownloadManager::worker, this, url).detach();
    }

private:
    int log_fd;
    int shm_fd;
    std::atomic<int>* counter;
    sem_t* semaphore;

    void initialize_resources() {
        // Создание файла лога
        log_fd = open("download_log.txt", O_CREAT | O_WRONLY | O_APPEND, 0644);
        if (log_fd < 0) {
            perror("Failed to open log file");
            exit(1);
        }

        // Создание разделяемой памяти для счетчика
        shm_fd = shm_open("/download_counter", O_CREAT | O_RDWR, 0666);
        if (shm_fd < 0) {
            perror("Failed to create shared memory");
            close(log_fd);
            exit(1);
        }

        ftruncate(shm_fd, sizeof(std::atomic<int>)); // Задание размера разделяемой памяти в виде атомарного int
        counter = static_cast<std::atomic<int>*>(
            mmap(nullptr, sizeof(std::atomic<int>), 
                 PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0)); // static_cast преобразование функции mmap в указатель на атомарный тип данных
        new (counter) std::atomic<int>(0); 

        // Инициализация именованного семафора
        semaphore = sem_open("/download_sem", O_CREAT, 0666, 1);
        if (semaphore == SEM_FAILED) {
            perror("Failed to create semaphore");
            cleanup_resources();
            exit(1);
        }
    }

    void cleanup_resources() {
        if (log_fd >= 0) close(log_fd);
        if (counter) {
            counter->~atomic();
            munmap(counter, sizeof(std::atomic<int>));
        }
        if (shm_fd >= 0) {
            shm_unlink("/download_counter");
            close(shm_fd);
        }
        if (semaphore != SEM_FAILED) {
            sem_close(semaphore);
            sem_unlink("/download_sem");
        }
    }

    void worker(const std::string& url) {
        int download_id = counter->fetch_add(1) + 1;

        log_message("Download " + std::to_string(download_id) + " started for: " + url);

        // Генерация имени файла из URL
        std::string filename = "image_" + std::to_string(download_id) + 
                               url.substr(url.find_last_of("."));

        // Выполнение загрузки с помощью curl
        std::string command = "curl -s -o " + filename + " \"" + url + "\"";
        int result = system(command.c_str());

        if (result == 0) {
            log_message("Download " + std::to_string(download_id) + 
                       " completed: " + filename);
        } else {
            log_message("Download " + std::to_string(download_id) + 
                       " failed for: " + url);
        }
    }

    void log_message(const std::string& message) {
        sem_wait(semaphore);
        time_t now = time(nullptr);
        std::string timestamp = ctime(&now);
        timestamp.pop_back(); // Удаление символа новой строки
        std::string log_entry = "[" + timestamp + "] " + message + "\n";
        write(log_fd, log_entry.c_str(), log_entry.size());
        fsync(log_fd);
        sem_post(semaphore);
    }
};

int main() {
    DownloadManager manager;
    std::string url;

    std::cout << "Enter image URLs (one per line, 'exit' to quit):" << std::endl;

    while (true) {
        std::getline(std::cin, url);
        if (url == "exit") break;
        if (!url.empty()) {
            manager.download(url);
        }
    }

    return 0;
}