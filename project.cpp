#include <iostream>
#include <fstream>

using namespace std;

class Storage
{
public:
    virtual void save(string filename, string text) {}
    virtual string load(string filename) {}
};

class FileStorage : public Storage
{
public:
    void save(string filename, string text)
    {
        ofstream file(filename);
        file << text << endl;
        file.close();
    }

    string load(string filename)
    {
        string result;
        ifstream file(filename);
        getline(file, result);
        file.close();
        return result;
    }
};

class TextProcessor
{
public:
    virtual string process(string input, string secret) {}
};

class EncryptionProcessor : public TextProcessor
{
public:
    string process(string input, string secret)
    {
        int key = 0;
        string result = "";

        for (int i = 0; i < secret.length(); i++)
        {
            key += secret[i];
        }

        for (int i = 0; i < input.length(); i++)
        {
            result += input[i] ^ key;
        }

        return result;
    }
};

class NoteManager
{
private:
    Storage *storage;
    TextProcessor *processor;

public:
    NoteManager(Storage *storage, TextProcessor *processor)
    {
        this->storage = storage;
        this->processor = processor;
    }

    void createNote()
    {
        string encryptionKey, noteTitle, text, result;

        cout << "Enter contents of note: ";
        getline(cin, text);

        cout << "Enter title of note: ";
        getline(cin, noteTitle);

        cout << "Enter a key for encryption: ";
        getline(cin, encryptionKey);

        result = processor->process(text, encryptionKey);
        storage->save(noteTitle + ".txt", result);
    }

    void readNote()
    {
        string encryptionKey, noteTitle, text, result;

        cout << "Enter only title of note: ";
        getline(cin, noteTitle);

        cout << "Enter the key for this note: ";
        getline(cin, encryptionKey);

        text = storage->load(noteTitle + ".txt");
        result = processor->process(text, encryptionKey);

        cout << "Title: " << noteTitle << endl;
        cout << "Content: " << result << endl;
    }
};

class Menu
{
private:
    NoteManager *noteManager;

public:
    Menu(NoteManager *noteManager)
    {
        this->noteManager = noteManager;
    }

    string getChoice()
    {
        string choice;

        cout << "1) Create note." << endl;
        cout << "2) Read note." << endl;
        cout << "3) Exit." << endl;

        cout << "^) Enter your choice: ";
        getline(cin, choice);

        return choice;
    }

    void run()
    {
        string choice;

        while (true)
        {
            choice = getChoice();

            if (choice == "1")
            {
                noteManager->createNote();
            }
            else if (choice == "2")
            {
                noteManager->readNote();
            }
            else if (choice == "3")
            {
                cout << "Exiting..." << endl;
                break;
            }
            else
            {
                cout << "Not implemented yet!" << endl;
            }
        }
    }
};

int main()
{
    Storage *storage = new FileStorage();
    TextProcessor *processor = new EncryptionProcessor();
    NoteManager *noteManager = new NoteManager(storage, processor);
    Menu menu(noteManager);

    menu.run();

    return 0;
}