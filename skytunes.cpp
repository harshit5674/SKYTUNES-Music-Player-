#include <bits/stdc++.h>
#include <windows.h>
#include <ctime>
#include <iostream>
#include <filesystem>
#include <fstream>
namespace fil = std::filesystem;

time_t t1, t2, previous_pause_time = 0;

using namespace std;
const int mxm = 1e9 + 7;
const int mx = 1e5 + 1;

string current;
string current_name;
bool check_validity(string s)
{
    ifstream f(s.c_str());
    return f.good();
}
string clean_path(string s)
{
    int t = s.size();
    string ans;
    for (int i = t - 1; i >= 0; i--)
    {
        if (s[i] == '\\')
        {
            break;
        }
        else
        {
            ans.push_back(s[i]);
        }
    }
    reverse(ans.begin(), ans.end());

    for (int i = 0; i < 4; i++)
    {
        ans.pop_back();
    }
    return ans;
}
bool playing = 0;
class Node
{
private:
    string mp3_path;
    string mp3_name;
    Node *ne;
    Node *pr;
    friend class DoublyLinkedList;
};
map<string, Node *> collections;
void play_music()
{
    string s = "open \"" + current;
    s += "\" type mpegvideo alias mp3";
    mciSendString(s.c_str(), NULL, 0, NULL);
    mciSendString("play mp3", NULL, 0, NULL);
}
class DoublyLinkedList
{
public:
    DoublyLinkedList()
    {
        h = new Node();
        t = new Node();
        h->ne = t;
        t->pr = h;
        h->mp3_path = "a";
        t->ne = NULL;
        h->pr = NULL;
        c = h;
    }
    void add_back(string s)
    {
        Node *w = new Node();
        w->mp3_path = s;
        Node *i = t->pr;
        i->ne = w;
        w->ne = t;
        w->pr = i;
        t->pr = w;
        w->mp3_name = clean_path(s);
        collections[w->mp3_name] = w;
    }
    void moveanywhere(string s1, string s2)
    {
        Node *t1 = collections[s1];
        Node *t2 = collections[s2];
        Node *t1_prev = t1->pr;
        Node *t1_next = t1->ne;
        Node *t2_prev = t2->pr;
        Node *t2_next = t2->ne;

        cout << "press 1 to add after " << s2 << endl;
        cout << "press 0 to add before " << s2 << endl;
        string key;
    label6:
        cin >> key;
        if (key == "1")
        {
            if(t2->ne==t1){
                return;
            }
        PATH7:

            t1_prev->ne = t1_next;
            t1_next->pr = t1_prev;
            t1->ne = t2_next;
            t1->pr = t2;
            t2->ne = t1;
            t2_next->pr = t1;
        }
        else if (key == "0")
        {
        PATH8:
            if(t2->pr==t1){
                return;
            }
            t1_prev->ne = t1_next;
            t1_next->pr = t1_prev;
            t1->ne = t2;
            t1->pr = t2_prev;
            t2_prev->ne = t1;
            t2->pr = t1;
        }
        else
        {
            cout << "Wrong Enter again\n";
            goto label6;
        }
    }
    void delete_song(string s)
    {
        if (s == c->mp3_name)
        {
            next_song();
            play_current();
            mciSendString("close mp3", NULL, 0, NULL);
            play_music();
        }
        Node *temp = collections[s];
        Node *t_next = temp->ne;
        Node *t_prev = temp->pr;
        t_prev->ne = t_next;
        t_next->pr = t_prev;
        delete (temp);
        cout << "                                                Your song has been deleted" << endl;
        collections.erase(s);
    }
    void next_song()
    {
        if (c == t->pr)
        {
            c = h;
        }
        c = c->ne;
    }
    void previous_song()
    {
        if (c->pr != h)
        {
            c = c->pr;
        }
        else
        {
            c = t->pr;
        }
    }
    void play_current()
    {
        if (c->mp3_path == "a")
        {
            c = c->ne;
        }
        current = c->mp3_path;
        current_name = c->mp3_name;
        cout << current_name << "\n";
    }
    void save_all()
    {
        std::ofstream ofs;
        ofs.open("save.txt", std::ofstream::out | std::ofstream::trunc);
        ofs.close();
        std::filesystem::path cwd = std::filesystem::current_path();
        string path = cwd.string();
        path += "\\save.txt";
        ofstream fw(path.c_str(), std::ofstream::out);
        Node *cu = h->ne;
        while (cu->ne != NULL)
        {
            fw << cu->mp3_path << "\n";
            cu = cu->ne;
        }
        fw.close();
    }
    void display_list()
    {
        int cnt = 1;
        Node *cu = h->ne;
        while (cu->ne != NULL)
        {
            cout << cnt << ". " << cu->mp3_name << "\n";
            cnt++;
            cu = cu->ne;
        }
    }
    void insert_anywhere(string name)
    {
        Node *r = collections[name];
        cout << "press 1 to add after this song\npress 0 to add before this song.";
        string key;
    label1:
        cin >> key;
        if (key == "1")
        {
            cin.ignore();
        PATH:
            cout << "Enter path of song:";
            string path1;  
            getline(cin, path1);
            if (!check_validity(path1))
            {
                cout << "Please enter a valid path! " << endl;
                goto PATH;
            }
            if (path1[path1.size() - 1] != '3')
            {
                cout << "\nPlease enter a valid song: ";
                goto PATH;
            }
            Node *u = new Node();
            u->mp3_path = path1;
            u->mp3_name = clean_path(path1);
            (r->ne)->pr = u;
            u->ne = r->ne;
            r->ne = u;
            u->pr = r;
            collections[u->mp3_name] = u;
        }
        else if (key == "0")
        {
             cin.ignore();
        PATH2:
            cout << "Enter path of song:";
            string path1;
           
            getline(cin, path1);
            if (!check_validity(path1))
            {
                cout << "Please enter a valid path! " << endl;
                goto PATH2;
            }
            if (path1[path1.size() - 1] != '3')
            {
                cout << "\nPlease enter a valid song: ";
                goto PATH2;
            }
            Node *u = new Node();
            u->mp3_path = path1;
            u->mp3_name = clean_path(path1);
            Node *t = r->pr;
            t->ne = u;
            u->ne = r;
            u->pr = t;
            r->pr = u;
            collections[u->mp3_name] = u;
        }
        else
        {
            cout << "Wrong Enter again\n";
            goto label1;
        }
    }
    void goto_song()
    {
        cout << endl;
        cout << "Enter Name of the song:";
        string si;
        cin.ignore();
        sip:
        getline(cin, si);
        if(collections.find(si)==collections.end()){
            cout<<"Please Enter a valid Name!\n";
            goto sip;
        }
        c = collections[si];
        playing = 1;
        mciSendString("close mp3", NULL, 0, NULL);
        t1 = time(0);
        t2 = time(0);
        play_current();
        play_music();
    }
    void insert_next()
    {
        cin.ignore();
    PATH3:
        string path1;
        getline(cin, path1);
        if (!check_validity(path1))
        {
            cout << "Please enter a valid path! " << endl;
            goto PATH3;
        }
        if (path1[path1.size() - 1] != '3')
        {
            cout << "\nPlease Enter a MP3 file!";
            goto PATH3;
        }

        Node *u = new Node();
        u->mp3_path = path1;
        u->mp3_name = clean_path(path1);
        (c->ne)->pr = u;
        u->ne = c->ne;
        c->ne = u;
        u->pr = c;
        collections[u->mp3_name] = u;
    }

private:
    Node *h;
    Node *t;
    Node *c;
};
DoublyLinkedList *dl = new DoublyLinkedList();
void save_playlist()
{
    dl->save_all();
}
void read_folder()
{
    std::filesystem::path cwd = std::filesystem::current_path();
    string path = cwd.string();
    path += "\\music";
    for (const auto &entry : fil::directory_iterator(path))
    {
        string e = entry.path().string();
        dl->add_back(e);
    }
}
void make_list()
{
    string paths;
    ifstream Fileread("save.txt");
    while (getline(Fileread, paths))
    {
        dl->add_back(paths);
    }
}
void display_all()
{
    dl->display_list();
}
int main()
{
    int n = 0;
    char ch;
    FILE *f = fopen("save.txt", "r");
    if (fscanf(f, "%c", &ch) == EOF)
    {
        read_folder();
    }
    else
    {
        make_list();
    }
    fclose(f);
    bool sta = false; // this is to check if this is the first file that player will play;
    // escape-quit
    // space pause play
    // arrow for changing songs
    string key;
    while (true)
    {
        if (!sta)
        {
            cout << "Press 1 to start skytunes and press 4 to quit\n";
            goto label;
        }
    label:
        key = -9;
        cin >> key;
        system("CLS");
cout<<"                     _______  ___   _  __   __  _______  __   __  __    _  _______  _______\n";
cout<<"                    |       ||   | | ||  | |  ||       ||  | |  ||  |  | ||       ||       |\n";
cout<<"                    |  _____||   |_| ||  |_|  ||_     _||  | |  ||   |_| ||    ___||  _____|\n";
cout<<"                    | |_____ |      _||       |  |   |  |  |_|  ||       ||   |___ | |_____ \n";
cout<<"                    |_____  ||     |_ |_     _|  |   |  |       ||  _    ||    ___||_____  |\n";
cout<<"                     _____| ||    _  |  |   |    |   |  |       || | |   ||   |___  _____| |\n";
cout<<"                    |_______||___| |_|  |___|    |___|  |_______||_|  |__||_______||_______|\n";

        cout << "Press 1 to pause/play the file \npress 2 to play the next song\npress 3 to play the previous song\npress 4 to exit the file.\npress 5 to display the playlist\npress 6 to add song at next position\npress 7 to add song anywhere" << endl;
        cout << "Press 8 to play any song" << endl;
        cout << "Press 9 to delete a song" << endl;
        cout << "Press 10 to move song anywhere" << endl;
        sta = true;
        if (key == "1")
        {
            if (!playing)
            {
                t1 = time(0);
                dl->play_current();
                playing = 1;
                cout << "                                                Audio file playing...\n\n";
                play_music();
            }
            else
            {
                mciSendString("pause mp3", NULL, 0, NULL);
                t2 = time(0);
                playing = 0;
                cout << "Audio file paused after " << t2 - t1 + previous_pause_time << " seconds.\n\n";
                previous_pause_time += t2 - t1;
            }
        }
        else if (key == "2")
        {
            mciSendString("close mp3", NULL, 0, NULL);
            t1 = time(0);
            previous_pause_time = 0;
            dl->next_song();
            dl->play_current();
            playing = 1;
            cout << "                                                Audio file playing...\n\n";
            play_music();
        }
        else if (key == "3")
        {
            mciSendString("close mp3", NULL, 0, NULL);
            t1 = time(0);
            dl->previous_song();
            previous_pause_time = 0;

            dl->play_current();
            playing = 1;
            cout << "                                                Audio file playing...\n\n";
            play_music();
        }
        else if (key == "4")
        {
            save_playlist();
            exit(0);
        }
        else if (key == "5")
        {
            display_all();
        }
        else if (key == "6")
        {
            cout << "add path of the song:";
            dl->insert_next();
        }
        else if (key == "7")
        {
            cout << "Enter name of song with respect you want to insert:";
            string si;
            cin.ignore();
        PATH4:
            
            getline(cin, si);
            if (collections.find(si) == collections.end())
            {
                cout << "Enter a valid song name!" << endl;
                goto PATH4;
            }
            dl->insert_anywhere(si);
        }
        else if (key == "8")
        {
            dl->goto_song();
        }
        else if (key == "9")
        {
            cout << "Enter the song name you want to delete: ";
            string s;
            cin.ignore();
        PATH5:
            
            getline(cin, s);

            if(collections.find(s)==collections.end()){
                cout<<"Please enter a valid name!\n";
                goto PATH5;
            }
            dl->delete_song(s);
        }
        else if (key == "10")
        {
            cout << "Enter the name of the song you want move:\n";
            string s1;
            cin.ignore();
        label11:
            
            getline(cin, s1);
            if (collections.count(s1) == 0)
            {
                cout << "Please enter a valid song!" << endl;
                goto label11;
            }
        label12:
            cout << "Enter the song before or after you want to insert:\n";
            string s2;
            getline(cin, s2);
            if (collections.count(s2) == 0)
            {
                cout << "Please enter a valid song!"
                     << " " << s2 << endl;
                goto label12;
            }
            dl->moveanywhere(s1, s2);
        }
        else
        {
            cout << "Please enter a valid key: " << endl;
            goto label;
        }
    label1:
        cin >> key;
        system("CLS");
cout<<"                     _______  ___   _  __   __  _______  __   __  __    _  _______  _______\n";
cout<<"                    |       ||   | | ||  | |  ||       ||  | |  ||  |  | ||       ||       |\n";
cout<<"                    |  _____||   |_| ||  |_|  ||_     _||  | |  ||   |_| ||    ___||  _____|\n";
cout<<"                    | |_____ |      _||       |  |   |  |  |_|  ||       ||   |___ | |_____ \n";
cout<<"                    |_____  ||     |_ |_     _|  |   |  |       ||  _    ||    ___||_____  |\n";
cout<<"                     _____| ||    _  |  |   |    |   |  |       || | |   ||   |___  _____| |\n";
cout<<"                    |_______||___| |_|  |___|    |___|  |_______||_|  |__||_______||_______|\n";        cout << "Press 1 to pause/play the file \npress 2 to play the next song\npress 3 to play the previous song\npress 4 to exit the file.\npress 5 to display the playlist\npress 6 to add song at next position\npress 7 to add song anywhere" << endl;
        cout << "Press 8 to play any song" << endl;
        cout << "Press 9 to delete a song" << endl;
        cout << "Press 10 to move song anywhere in the list" << endl;
        if (key == "1")
        {
            if (!playing)
            {
                t1 = time(0);
                dl->play_current();
                playing = 1;
                cout << "                                                Audio file playing...\n\n";
                play_music();
            }
            else
            {
                mciSendString("pause mp3", NULL, 0, NULL);
                t2 = time(0);
                playing = 0;
                cout << "Audio file paused after " << t2 - t1 + previous_pause_time << " seconds.\n\n";
                previous_pause_time += t2 - t1;
            }
        }
        else if (key == "2")
        {
            mciSendString("close mp3", NULL, 0, NULL);
            dl->next_song();
            previous_pause_time = 0;
            t1 = time(0);
            playing = 1;
            dl->play_current();
            cout << "                                                Audio file playing...\n\n";
            play_music();
        }
        else if (key == "3")
        {
            mciSendString("close mp3", NULL, 0, NULL);
            dl->previous_song();
            t1 = time(0);
            previous_pause_time = 0;
            dl->play_current();
            playing = 1;
            cout << "                                                Audio file playing...\n\n";
            play_music();
        }
        else if (key == "4")
        {
            save_playlist();
            exit(0);
        }
        else if (key == "5")
        {
            display_all();
        }
        else if (key == "6")
        {
            cout << "add path of the song:";
            dl->insert_next();
        }
        else if (key == "7")
        {

            string si;
            cin.ignore();
        PATH8:
            cout << "Enter name of song with respect you want to insert:";
            
            getline(cin, si);
            if (collections.find(si) == collections.end())
            {
                cout << "Enter a valid song name!" << endl;
                goto PATH8;
            }
            dl->insert_anywhere(si);
        }
        else if (key == "8")
        {
            dl->goto_song();
        }

        else if (key == "9")
        {
            cout << "Enter the song name you want to delete: ";
            string s;
            cin.ignore();
            PATH55:
            
            getline(cin, s);

            if(collections.find(s)==collections.end()){
                cout<<"Please enter a valid name!\n";
                goto PATH55;
            }
            dl->delete_song(s);
        }
        else if (key == "10")
        {
            cout << "Enter the name of the song you want move:\n";
            string st1;
        label111:
            
            getline(cin, st1);
            if (collections.count(st1) == 0)
            {
                cout << "Please enter a valid song!" << endl;
                goto label111;
            }
        label121:
            cout << "Enter the song before or after you want to insert:\n";
            string st2;
            getline(cin, st2);
            if (collections.count(st2) == 0)
            {
                cout << "Please enter a valid song!"
                     << " " << st2 << endl;
                goto label121;
            }
            dl->moveanywhere(st1, st2);
        }
        else
        {
            cout << "Please enter a valid key: " << endl;
            goto label1;
        }
    }
}