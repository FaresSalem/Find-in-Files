#ifndef INVERTED_INDEX_H
#define INVERTED_INDEX_H




#include<iostream>
#include <QString>
#include <list>
#include <QDebug>
#include <QDir>
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

        small_data_type& operator =(const small_data_type& a)
        {
            file_name = a.file_name;
            word_count = a.word_count;
            return *this;
        }
};
struct big_data_type
{
        QString word_name;
        QList<small_data_type> small_arr[small_buckets];
        unsigned short max_small_elements=3;
};
struct special_data_type
{
    QString word_name;
    QList<small_data_type> special_list;
};

class pseudo_inverted_index
{
private:
    list<special_data_type> special_array[big_buckets];
public:
    list<small_data_type>* search(QString word);
    bool returnDB(QDir dir);
};

class inverted_index
{
private:
        QList<big_data_type> big_array[big_buckets];

        //unsigned short big_hash_fun(QString word);

        unsigned short small_hash_fun(QString file);
        bool is_word_found(QString word);
        void insert_word(QString word);
        bool is_file_found(QString word,QString file);
        void increase_word_count(QString word,QString file);
        void insert_file(QString word,QString file);
//        friend QDataStream & operator<< (QDataStream& out, const QList<big_data_type> &big_array );
//        friend QDataStream & operator>> (QDataStream& stream, QList<big_data_type> &big_array);
public:
        void build_inverted_index(QStringList &wordsList,QString file,bool done);
        bool createDB(QDir dir);
        bool returnDB(QDir dir);
        void traverse_inverted_index(); // just for testing , will  be removed when building real database
        list<small_data_type>* search(QString word);


};





#endif // INVERTED_INDEX_H
