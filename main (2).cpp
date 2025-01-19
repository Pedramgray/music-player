#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;

class SONG {  
public:
    string Name;  
    string artist;
    string duration; 
    string genre;
    SONG* next;  
    SONG* prev;  

    SONG(string N, string a, string d, string g) 
        : Name(N), artist(a), duration(d), genre(g), next(nullptr), prev(nullptr) {}
};

class myplaylist {  
private:
    SONG* F;  //first

public:
    myplaylist() : F(nullptr) {}  

    
    bool isValidDuration( string& duration) {
        size_t colonPos = duration.find(':');
        if (colonPos == string::npos || colonPos == 0 || colonPos == duration.length() - 1) {
            return false; 
        }

        string minutes = duration.substr(0, colonPos);
        string seconds = duration.substr(colonPos + 1);

        if (!all_of(minutes.begin(), minutes.end(), ::isdigit) || 
            !all_of(seconds.begin(), seconds.end(), ::isdigit)) {
            return false; 
        }

        int min = stoi(minutes);
        int sec = stoi(seconds);
        return (min >= 0 && sec >= 0 && sec < 60); 
    }

    void addSong(string Name, string artist, string duration, string genre) {  
        if (!isValidDuration(duration)) {
            cout << "Error: Invalid duration format '" << duration << "'. Expected format is MM:SS." << endl;
            return;
        }

        SONG* newSong = new SONG(Name, artist, duration, genre);  
        if (!F) {
            F = newSong;  
        } else {
            SONG* current = F;  
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newSong;
            newSong->prev = current;
        }
    }

    void removeSongByTitle(string Name) {  
        SONG* current = F;  
        while (current!=nullptr) {
            if (current->Name == Name) {  
                if (current->prev) 
                current->prev->next = current->next;
                if (current->next) 
                current->next->prev = current->prev;
                if (current == F) 
                F = current->next;  
                delete current;
                return;
            }
            current = current->next;
        }
    }
void removeSongsByArtist(string artist) {
    SONG* current = F;  
    while (current != nullptr) {
        if (current->artist == artist) {  
            if (current->prev) {
                current->prev->next = current->next;
            }
            if (current->next) {
                current->next->prev = current->prev;
            }
            
            if (current == F) { 
                F = current->next;  
            }
           
            current = current->next; 
            
            delete current; 
            return;
        } else {
           
            current = current->next;
        }
    }
}

void removeSongsByGenre(string genre) {
    SONG* current = F;  
    while (current != nullptr) {
        if (current->genre == genre) {
            if (current->prev) {
                current->prev->next = current->next;
            }
            if (current->next) {
                current->next->prev = current->prev;
            }
            if (current == F) { 
                F = current->next;  
            }
            current = current->next; 
          
            delete current;
            return;
        } else {
            
            current = current->next;
        }
    }
}


    void sortByArtist() {
        vector<SONG*> songs;  
        SONG* current = F; 
        while (current) {
            songs.push_back(current);
            current = current->next;
        }

        
        sort(songs.begin(), songs.end(), [](SONG* a, SONG* b) {  
            return a->artist < b->artist;
        });

      
        F = nullptr;  
        for (auto song : songs) {
            addSong(song->Name, song->artist, song->duration, song->genre); 
        }
    }

    
    void searchSong(string keyword) {
        SONG* current = F;  
        bool found = false;
        while (current) {
            if ((current->Name.find(keyword) != string::npos) ||  
                (current->artist.find(keyword) != string::npos) || 
                (current->genre.find(keyword) != string::npos)) {
                cout << "Found: " << current->Name << " by " << current->artist << " [" << current->duration << "]" << endl; 
                found = true;
            }
            current = current->next;
        }
        if (!found) {
            cout << "No songs found for: " << keyword << endl;
        }
    }

    
    void sortByTitle() {
        vector<SONG*> songs;  
        SONG* current = F;  
        while (current) {
            songs.push_back(current);
            current = current->next;
        }
        sort(songs.begin(), songs.end(), [](SONG* a, SONG* b) {  
            return a->Name < b->Name;  
        });
        F = nullptr;  // Changed head to F
        for (auto song : songs) {
            addSong(song->Name, song->artist, song->duration, song->genre); // Changed title to Name
        }
    }

    
    void displayPlaylist() {
        SONG* current = F;  
        while (current) {
            cout << current->Name << " by " << current->artist << " {" << current->duration << "} - Genre: " << current->genre << endl; 
            current = current->next;
        }
    }

    
    void removeDuplicates() {
        unordered_set<string> seen;
        SONG* current = F;  
        while (current) {
            
            string key = current->Name + "|" + current->artist + "|" + current->duration + "|" + current->genre; // Changed title to Name
            
            if (seen.find(key) != seen.end()) {
                removeSongByTitle(current->Name); 
            } else {
                seen.insert(key);
            }
            current = current->next;
        }
    }



  
    int durationToSeconds(const string& duration) {
        size_t colonPos = duration.find(':');
        int minutes = stoi(duration.substr(0, colonPos));
        int seconds = stoi(duration.substr(colonPos + 1));
        return minutes * 60 + seconds;
    }

    
    void sortByDuration() {
        vector<SONG*> songs;  
        SONG* current = F;  
        while (current) {
            songs.push_back(current);
            current = current->next;
        }

        sort(songs.begin(), songs.end(), [this](SONG* a, SONG* b) {  
            return durationToSeconds(a->duration) < durationToSeconds(b->duration);
        });

        
        F = nullptr; 
        for (auto song : songs) {
            addSong(song->Name, song->artist, song->duration, song->genre); 
        }
    }

    void playRandom() {
        vector<SONG*> songs; 
        SONG* current = F;  
        while (current) {
            songs.push_back(current);
            current = current->next;
        }
        srand(static_cast<unsigned int>(time(0)));
        random_shuffle(songs.begin(), songs.end());
        for (auto song : songs) {
            cout << "Playing: " << song->Name << " by " << song->artist << endl;
        }
    }

    
    ~myplaylist() {  
        SONG* current = F; 
        while (current) {
            SONG* next = current->next; 
            delete current;
            current = next;
        }
    }
};

int main() {
    myplaylist playlist;  

    
    playlist.addSong("Song u turn", "Artist aaron", "3:50", "rock");
    playlist.addSong("Song sound of silence", "Artist disturbed", "4:20", "metal");
    playlist.addSong("Song heartless", "Artist kanye", "2:40", "rap");
    playlist.addSong("Song invalid", "Artist artist", "4:86", "idk"); 
    int choice;
    do {
        cout << "\nEnter:" << endl;
        cout << "1 for Show Playlist" << endl;
        cout << "2 for Search Song" << endl;
        cout << "3 for Remove Duplicates" << endl;
        cout << "4 for Sort by Title" << endl;
        cout << "5 for Play Random" << endl;
        cout << "6 to Exit" << endl;
        cout << "7 to Sort by Artist" << endl;
        cout << "8 to Move Up" << endl;
        cout << "9 to Move Down" << endl;
        cout << "10 to Remove a Specific Song" << endl;
        cout << "11 to Remove All Songs by an Artist" << endl;
        cout << "12 to Remove All Songs by a Genre" << endl;
        cout << "13 to Add a New Song" << endl; 
        cout << "14 to Sort by Duration" << endl; 
        cin >> choice;
        cin.ignore(); 

        switch (choice) {
            case 1:
                playlist.displayPlaylist();
                break;
            case 2: {
                string N;  
                cout << "Search: ";
                getline(cin, N); 
                playlist.searchSong(N);  
                break;
            }
            case 3:
                playlist.removeDuplicates();
                break;
            case 4:
                playlist.sortByTitle();
                break;
            case 5:
                playlist.playRandom();
                break;
            case 6:
                cout << "Exiting..." << endl;
                break;
            case 7:
                playlist.sortByArtist();
                break;
            
           
            
            case 10: { 
                string Name; 
                cout << "Enter the title of the song to remove: ";
                getline(cin, Name);  
                playlist.removeSongByTitle(Name);  
                break;
            }
            case 11: { 
                string artist;
                cout << "Enter the artist's name to remove all songs: ";
                getline(cin, artist);
                playlist.removeSongsByArtist(artist);
                break;
            }
            case 12: {
                string genre;
                cout << "Enter the genre to remove all songs: ";
                getline(cin, genre);
                playlist.removeSongsByGenre(genre);
                break;
            }
            case 13: { 
                string Name, artist, duration, genre;  
                cout << "Enter the title of the song: ";
                getline(cin, Name); 
                cout << "Enter the artist of the song: ";
                getline(cin, artist);
                cout << "Enter the duration of the song (MM:SS): ";
                getline(cin, duration);
                cout << "Enter the genre of the song: ";
                getline(cin, genre);
                playlist.addSong(Name, artist, duration, genre); 
                break;
            }
            case 14: {
                playlist.sortByDuration(); 
                break;
            }
            default:
                cout << "Invalid option. Please try again." << endl;
        }
    } while (choice != 6);

    return 0;
}
