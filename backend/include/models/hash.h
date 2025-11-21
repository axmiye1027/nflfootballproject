#include "../includes.h"

#ifndef HASH_H
#define HASH_H

const int TABLE_SIZE  = 31;
const int TABLE_SIZE2 = 29;

template<typename T>
struct keyInput
{
    T    value;
    int  key;
    bool isDelete;

    keyInput(T value, int key) : value(value), key(key), isDelete(false) {}
};

template<typename T>
class DoubleHashTable 
{  
public:
    DoubleHashTable() : size{TABLE_SIZE}, count{0}
    {
        table = new keyInput<T>[size];

        for (int i = 0; i < size; ++i) 
        {
            table[i].key = -1; // Mark all slots as empty
        }
    }

    ~DoubleHashTable()
    {
        delete [] table;
    }
    
    // Insert a value into the hash table
    bool insert(keyInput<T> input) 
    {
        if (count >= size) 
        {
            cout << "Hash table is full" << endl;
            return false;
        }

        int i = 0;
        int index = hash1(input.key);
        int firstDeletedIndex = -1;

        while (i < size)
        {
            if (table[index].key == input.key) 
            {
                table[index].value = input.value; // Update existing key
                cout << "Updated " << input.key << " to " << input.value << endl;
                return true;
            }

            if (table[index].key == -2 && firstDeletedIndex == -1)
            {
                firstDeletedIndex = index;
            }
            
            if (table[index].key == -1)
            {
                break;
            }

            i++;
            index = doubleHash(input.key, i);
        }

        int insertIndex    = (firstDeletedIndex != -1) ? firstDeletedIndex : index;
        table[insertIndex] = input;

        count++;

        cout << "Inserted " << input.key << " " << input.value << endl;
        return true;
    }

    // Remove a key from the hash table
    bool remove(int key) 
    {
        int i = 0;
        int index = hash1(key);
        
        while (i < size && table[index].key != -1)
        {
            if (table[index].key == key) 
            {
                table[index].key = -2; // Mark as deleted
                count--;
                cout << "Key " << key << " was removed" << endl;
                return true;
            }

            i++;
            index = doubleHash(key, i);
        }

        cout << "Key " << key << " not found" << endl;
        return false;
    }
    
    // Search for a key in the hash table
    bool search(int key) 
    {
        int i = 0;
        int index = hash1(key);

        while (i < size && (table[index].key != -1 || table[index].key == -2))
        {
            if (table[index].key == key)
            {
                return true;
            }

            i++;
            index = doubleHash(key, i);
        }
        return false;
    }
    
    // Print the hash table
    void printTable() 
    {
        for (int i = 0; i < size; i++) 
        {
            cout << "Index " << i << ": ";

            if (table[i].key == -1) 
            {
                cout << "Empty";
            }
            else if (table[i].key == -2)
            {
                cout << "Available";
            }
            else 
            {
                cout << table[i].key << " " << table[i].value;
            }

            cout << endl;
        }
    }

private:
    keyInput<T>* table; // The table
    int          size;  // Size of the table
    int          count; // Amount of stored keys

    int hash1(int key) 
    {
        return key % size;
    }

    int hash2(int key) 
    {
        return 13 - (key % 13); // Assignment-specific secondary hash
    }
    
    int doubleHash(int key, int i) 
    {
        return (hash1(key) + i * hash2(key)) % size;
    }
};

template<typename T>
class QuadraticHashTable
{
public:
    QuadraticHashTable() : size{TABLE_SIZE2}, count{0} // Fixed size per assignment
    {
        table = new keyInput<T>[size];
        for (int i = 0; i < size; ++i) 
        {
            table[i].key = -1; // Mark all slots as empty
        }
    }

    ~QuadraticHashTable() 
    {
        delete[] table;
    }
    
    // Insert a value into the hash table
    bool insert(keyInput<T> input)
    {
        int baseIndex = hash1(input.key);
        int firstDeletedIndex = -1;

        for (int j = 0; j < size; ++j)
        {
            int index = (baseIndex + j * j) % size;

            if (table[index].key == input.key)
            {
                table[index].value = input.value;
                cout << "Updated " << input.key << " to " << input.value << endl;
                return true;
            }

            if (table[index].key == -2 && firstDeletedIndex == -1)
            {
                firstDeletedIndex = index;
            }

            if (table[index].key == -1)
            {
                int insertIndex    = (firstDeletedIndex != -1) ? firstDeletedIndex : index;
                table[insertIndex] = input;

                count++;

                cout << "Inserted " << input.key << " " << input.value << endl;
                return true;
            }
        }

        cout << "Unable to insert key " << input.key << ": No available position" << endl;
        return false;
    }

    // Remove a key from the hash table
    bool remove(int key)
    {
        int baseIndex = hash1(key);

        for (int j = 0; j < size; ++j)
        {
            int index = (baseIndex + j * j) % size;

            if (table[index].key == key)
            {
                table[index].key = -2;
                count--;
                cout << "Key " << key << " was removed" << endl;
                return true;
            }
            else if (table[index].key == -1)
            {
                break;
            }   
        }

        cout << "Key " << key << " not found" << endl;
        return false;
    }

    // Search for a key in the hash table
    bool search(int key)
    {
        int baseIndex = hash1(key);

        for (int j = 0; j < size; ++j)
        {
            int index = (baseIndex + j * j) % size;

            if (table[index].key == key)
            {
                return true;
            }
            else if (table[index].key == -1)
            {
                return false;
            }
        }

        return false;
    }

    // Print the hash table
    void printTable() 
    {
        for (int i = 0; i < size; i++) 
        {
            cout << "Index " << i << ": ";

            if (table[i].key == -1) 
            {
                cout << "Empty";
            }
            else if (table[i].key == -2)
            {
                cout << "Available";
            }
            else 
            {
                cout << table[i].key << " " << table[i].value;
            }

            cout << endl;
        }
    }

private:
    keyInput<T>* table; // The table
    int          size;  // Size of the table
    int          count; // Amount of stored keys

    int hash1(int key) 
    {
        return key % size;
    }
};

#endif