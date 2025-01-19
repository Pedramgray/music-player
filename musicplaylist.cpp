#include <iostream>
#include <string>
using namespace std;

// ساختار هر نود در لیست پیوندی
struct SongNode {
    string songName;
    string songArtist;
    string songGenre;
    int songDuration; // مدت زمان به ثانیه
    SongNode* next;
    SongNode* prev;

    SongNode(string name, string artist, string genre, int duration) 
        : songName(name), songArtist(artist), songGenre(genre), songDuration(duration), next(nullptr), prev(nullptr) {}
};

// کلاس مدیریت لیست پخش
class Playlist {
private:
    SongNode* head; // اشاره به اولین نود لیست

public:
    Playlist() : head(nullptr) {}

    // افزودن آهنگ به انتهای لیست
    void addSong(string songName, string songArtist, string songGenre, int songDuration) {
        SongNode* newSong = new SongNode(songName, songArtist, songGenre, songDuration);
        if (!head) {  // اگر لیست خالی باشد
            head = newSong;
        } else {  // اگر لیست خالی نباشد
            SongNode* current = head;
            while (current->next) { // پیمایش تا آخرین نود
                current = current->next;
            }
            current->next = newSong; // اتصال نود جدید به انتهای لیست
            newSong->prev = current; // اتصال نود جدید به نود قبلی
        }
        cout << "Song added: " << songName << endl;
    }

    // نمایش لیست پخش
    void displayPlaylist() {
        if (!head) {
            cout << "Playlist is empty!" << endl;
            return;
        }
        SongNode* current = head;
        while (current) {
            cout << "Name: " << current->songName 
                 << ", Artist: " << current->songArtist 
                 << ", Genre: " << current->songGenre 
                 << ", Duration: " << current->songDuration << "s" << endl;
            current = current->next;
        }
    }

    // جستجوی آهنگ
    void searchSong(string key, string type) {
        SongNode* current = head;
        bool found = false;
        while (current) {
            if ((type == "name" && current->songName == key) ||
                (type == "artist" && current->songArtist == key) ||
                (type == "genre" && current->songGenre == key)) {
                cout << "Found: " << current->songName << " by " << current->songArtist << endl;
                found = true;
            }
            current = current->next;
        }
        if (!found) cout << "No song found for the given " << type << ": " << key << endl;
    }

    // حذف آهنگ
    void deleteSong(string key, string type) {
        SongNode* current = head;
        while (current) {
            if ((type == "name" && current->songName == key) ||
                (type == "artist" && current->songArtist == key) ||
                (type == "genre" && current->songGenre == key)) {
                if (current->prev) current->prev->next = current->next;
                if (current->next) current->next->prev = current->prev;
                if (current == head) head = current->next;
                delete current;
                cout << "Deleted: " << key << endl;
                return;
            }
            current = current->next;
        }
        cout << "No song found to delete with " << type << ": " << key << endl;
    }

    // مرتب‌سازی لیست
    void sortPlaylist(string type) {
        if (!head || !head->next) return;
        for (SongNode* i = head; i->next; i = i->next) {
            for (SongNode* j = i->next; j; j = j->next) {
                if ((type == "name" && i->songName > j->songName) ||
                    (type == "artist" && i->songArtist > j->songArtist)) {
                    swap(i->songName, j->songName);
                    swap(i->songArtist, j->songArtist);
                    swap(i->songGenre, j->songGenre);
                    swap(i->songDuration, j->songDuration);
                }
            }
        }
        cout << "Playlist sorted by " << type << endl;
    }
    // حذف آهنگ‌های تکراری
    void removeDuplicates() {
        SongNode* current = head;
        while (current) {
            SongNode* runner = current->next;
            while (runner) {
                if (runner->songName == current->songName && runner->songArtist == current->songArtist) {
                    SongNode* duplicate = runner;
                    runner = runner->next;
                    if (duplicate->prev) duplicate->prev->next = duplicate->next;
                    if (duplicate->next) duplicate->next->prev = duplicate->prev;
                    delete duplicate;
                } else {
                    runner = runner->next;
                }
            }
            current = current->next;
        }
        cout << "Duplicates removed!" << endl;
    }

    // پخش تصادفی
    void shufflePlaylist() {
        if (!head) {
            cout << "Playlist is empty!" << endl;
            return;
        }
        int size = 0;
        SongNode* current = head;
        while (current) {
            size++;
            current = current->next;
        }
        SongNode* songs[size];
        current = head;
        for (int i = 0; i < size; i++) {
            songs[i] = current;
            current = current->next;
        }
        srand(time(0));
        for (int i = size - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            swap(songs[i], songs[j]);
        }
        cout << "Shuffled Playlist:" << endl;
        for (int i = 0; i < size; i++) {
            cout << "Name: " << songs[i]->songName << ", Artist: " << songs[i]->songArtist << endl;
        }
    }
};

int main() {
    Playlist playlist;
    int choice;
    do {
        cout << "\n--- Playlist Manager ---" << endl;
        cout << "1. Add Song" << endl;
        cout << "2. Display Playlist" << endl;
        cout << "3. Search Song" << endl;
        cout << "4. Delete Song" << endl;
        cout << "5. Sort Playlist" << endl;
        cout << "6. Remove Duplicates" << endl;
        cout << "7. Shuffle Playlist" << endl;
        cout << "8. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        cin.ignore(); // پاک کردن ورودی‌های اضافی

        if (choice == 1) {
            string songName, songArtist, songGenre;
            int songDuration;
            cout << "Enter song name: ";
            getline(cin, songName);
            cout << "Enter artist name: ";
            getline(cin, songArtist);
            cout << "Enter genre: ";
            getline(cin, songGenre);
            cout << "Enter duration (in seconds): ";
            cin >> songDuration;
            playlist.addSong(songName, songArtist, songGenre, songDuration);
        } else if (choice == 2) {
            playlist.displayPlaylist();
        } else if (choice == 3) {
            string key, type;
            cout << "Search by (name/artist/genre): ";
            cin >> type;
            cin.ignore();
            cout << "Enter " << type << ": ";
            getline(cin, key);
            playlist.searchSong(key, type);
        } else if (choice == 4) {
            string key, type;
            cout << "Delete by (name/artist/genre): ";
            cin >> type;
            cin.ignore();
            cout << "Enter " << type << ": ";
            getline(cin, key);
            playlist.deleteSong(key, type);
        } else if (choice == 5) {
            string type;
            cout << "Sort by (name/artist): ";
            cin >> type;
            playlist.sortPlaylist(type);
        } else if (choice == 6) {
            playlist.removeDuplicates();
        } else if (choice == 7) {
            playlist.shufflePlaylist();
        } else if (choice != 8) {
            cout << "Invalid choice! Try again." << endl;
        }
    } while (choice != 8);

    cout << "Exiting the program. Goodbye!" << endl;
    return 0;
}