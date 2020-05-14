#ifndef INVERTED_INDEX_H
#define INVERTED_INDEX_H



#endif // INVERTED_INDEX_H
#include<iostream>
#include <QString>
#include <list>
#include <QDebug>
#include <QtCore/qglobal.h>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QMainWindow>
#else
#include <QtGui/QMainWindow>
#endif
#define big_buckets 1000
#define small_buckets 10

using namespace std;

struct small_data_type
{
        QString file_name;
        unsigned short word_count=1;
};
struct big_data_type
{
        QString word_name;
        list<small_data_type> small_arr[small_buckets];
        unsigned short max_small_elements=3;
};

class inverted_index
{
private:
        list<big_data_type> big_array[big_buckets];
        unsigned short big_hash_fun(QString word)
        {
                unsigned short total=0;
                unsigned short len = word.size();
                for (unsigned short k=0;k<len;k++)       //open hashing
                {
                        total += word[k].digitValue();
                }
                return total%big_buckets;

        }
        unsigned short small_hash_fun(QString file)
        {
                unsigned short total=0;
                unsigned short len = file.size();
                for (unsigned short k=0;k<len;k++)   //open hashing
                {
                        total += file[k].digitValue();
                }

                return total%small_buckets;
        }
        bool is_word_found(QString word)
        {
                unsigned short big_index = big_hash_fun(word);
                for (auto it=(big_array[big_index]).begin();it!=(big_array[big_index]).end();it++)
                {
                        if (it->word_name == word)
                                return true;
                }
                return false;
        }
        void insert_word(QString word)
        {
                unsigned short big_index = big_hash_fun(word);
                big_data_type temp_instance;
                temp_instance.word_name = word;
                (big_array[big_index]).push_back(temp_instance);
        }
        bool is_file_found(QString word,QString file)
        {
                unsigned short big_index = big_hash_fun(word);
                unsigned short small_index = small_hash_fun(file);

                for (auto big_it=(big_array[big_index]).begin();big_it!=(big_array[big_index]).end();big_it++)
                        {
                                if (big_it->word_name == word)
                                {
                                        for (unsigned short i=0;i<small_buckets;i++)
                                        {
                                                small_index = (small_index+i)%small_buckets;
                                                auto small_it=((big_it->small_arr)[small_index]).begin();
                                                while (small_it!=((big_it->small_arr)[small_index]).end())
                                                {
                                                        if (small_it->file_name==file)
                                                                return true;
                                                        small_it++;
                                                }
                                                if ((((big_it->small_arr)[small_index]).size())<big_it->max_small_elements)
                                                        return false;
                                        }
                                        return false;

                                }
                        }
                return false;
        }
        void insert_file(QString word,QString file)
        {
                unsigned short big_index = big_hash_fun(word);
                unsigned short small_index = small_hash_fun(file);
                unsigned short i;
                for (auto it=(big_array[big_index]).begin();it!=(big_array[big_index]).end();it++)
                {
                        if (it->word_name == word)
                        {

                                for (i=small_index;i<=small_buckets+small_index;i++)       //closing hashing
                                {
                                        small_index = i%small_buckets;
                                        if ((((it->small_arr)[small_index]).size())<(it->max_small_elements))
                                        {
                                                small_data_type temp_instance;
                                                temp_instance.file_name = file;
                                                ((it->small_arr)[small_index]).push_back(temp_instance);
                                                break;
                                        }

                                }
                                if (i==small_buckets+small_index)
                                {
                                        (it->max_small_elements)++;
                                        small_data_type temp_instance;
                                        temp_instance.file_name = file;
                                        ((it->small_arr)[small_index]).push_back(temp_instance);
                                }
                        }
                }

        }
        void increase_word_count(QString word,QString file)
        {
                unsigned short big_index = big_hash_fun(word);
                unsigned short small_index = small_hash_fun(file);
                unsigned short i;
                for (auto big_it=(big_array[big_index]).begin();big_it!=(big_array[big_index]).end();big_it++)
                {
                        if (big_it->word_name == word)
                        {
                                for (i=small_index;i<=small_buckets+small_index;i++)
                                {
                                        small_index = i%small_buckets;
                                        for (auto small_it=((big_it->small_arr)[small_index]).begin();
                                                        small_it!=((big_it->small_arr)[small_index]).end();
                                                        small_it++)
                                        {
                                                if (small_it->file_name == file)
                                                        (small_it->word_count)++;
                                        }


                                }
                        }
                }
        }

public:
        void build_inverted_index(QStringList &wordsList,QString file,bool done)
        {

                if (!done)
                {
                    foreach(QString word,wordsList)
                    {
                       qDebug() <<word <<file;
                    if (is_word_found(word))
                    {
                            if (is_file_found(word,file))
                                    increase_word_count(word,file);
                            else
                                    insert_file(word,file);
                    }
                    else
                    {
                            insert_word(word);
                            insert_file(word,file);
                    }
                   }
                }
            else if (done)
            {
                traverse_inverted_index();
            }
}

        void traverse_inverted_index() // just for testing , will  be removed when building real database
        {
        for (unsigned short i=0;i<big_buckets;i++)
        {
                if (!((big_array[i]).empty()))
                {
                        for (auto big_it=(big_array[i]).begin();big_it!=(big_array[i]).end();big_it++)
                        {
                                qDebug()<< "\tthe word: " << big_it->word_name << " exists in \n\n";
                                for (unsigned short j=0;j<small_buckets;j++)
                                {
                                        if (!(((big_it->small_arr)[j]).empty()))
                                        {
                                                for (auto small_it=((big_it->small_arr)[j]).begin();
                                                                small_it!=((big_it->small_arr)[j]).end();small_it++)
                                                {
                                                         qDebug()<< small_it->file_name << " and repeated " << small_it->word_count <<" time(s)\n";
                                                }
                                        }
                                }
                        }
                }
        }
        }
};

