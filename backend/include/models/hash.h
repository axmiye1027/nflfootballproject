#include "../includes.h"

#include <iterator>

#ifndef HASH_H
#define HASH_H

const int TABLE_SIZE  = 13;

template<typename T>
struct keyInput
{
    T    value;
    int  key;
    bool isDelete;

    keyInput()                 : key(-1), isDelete(false) {}
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

    DoubleHashTable(int size) : size{size}, count{0}
    {
        table = new keyInput<T>[size];

        for (int i = 0; i < size; ++i) 
        {
            table[i].key = -1; // Mark all slots as empty
        }
    }

    DoubleHashTable(const DoubleHashTable& other) : size{other.size}, count{other.count}
    {
        table = new keyInput<T>[size];
        for (int i = 0; i < size; ++i) 
        {
            table[i] = other.table[i];
        }
    }

    DoubleHashTable& operator=(const DoubleHashTable& other)
    {
        if (this != &other) {
            delete[] table;
            
            size = other.size;
            count = other.count;
            table = new keyInput<T>[size];
            
            for (int i = 0; i < size; ++i) 
            {
                table[i] = other.table[i];
            }
        }
        return *this;
    }

    ~DoubleHashTable()
    {
        delete [] table;
    }
    
    // Insert a value into the hash table
    bool insert(keyInput<T> input) 
    {
        if (count >= size * 0.7) 
        {
            rehash();
        }

        int i                 = 0;
        int index             = hash1(input.key);
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

    bool isPrime(int n) 
    {
        if (n <= 1)                   return false;
        if (n <= 3)                   return true;
        if (n % 2 == 0 || n % 3 == 0) return false;
        
        for (int i = 5; i * i <= n; i += 6) 
        {
            if (n % i == 0 || n % (i + 2) == 0)
            {    
                return false;
            }
        }
        return true;
    }

    int nextPrime(int n) 
    {
        while (!isPrime(n)) 
        {
            n++;
        }
        return n;
    }

    void rehash() 
    {
        int oldSize           = size;
        keyInput<T>* oldTable = table;
        
        size = nextPrime(size * 2);
        
        table = new keyInput<T>[size];
        for (int i = 0; i < size; i++) 
        {
            table[i].key = -1;
        }
        
        count = 0;
        
        cout << "Rehashing from size " << oldSize << " to " << size << endl;
        
        for (int i = 0; i < oldSize; i++) 
        {
            if (oldTable[i].key != -1 && oldTable[i].key != -2) 
            {
                insertWithoutResize(oldTable[i]);
            }
        }
        
        delete[] oldTable;
    }

    bool insertWithoutResize(keyInput<T> input) 
    {
        int i = 0;
        int index = hash1(input.key);
        int firstDeletedIndex = -1;

        while (i < size) 
        {
            if (table[index].key == input.key) 
            {
                table[index].value = input.value;
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

        int insertIndex;

        if(firstDeletedIndex != -1)
        {
            insertIndex = firstDeletedIndex; 
        }
        else
        {
            insertIndex = index;
        }

        table[insertIndex] = input;
        count++;
        return true;
    }
};

#endif