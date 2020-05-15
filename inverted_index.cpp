#include <QDir>
#include <inverted_index.h>
list<small_data_type> info_list;
QList<big_data_type_ass> bigArray[big_buckets];
//QDataStream &operator<<(QDataStream& out, const QList<big_data_type> &big_array )
//{
//    big_array.begin()->word_name;
//    return out;
//}
unsigned short inverted_index::big_hash_fun(QString word)
{
        unsigned short total=0;
        unsigned short len = word.size();
        for (unsigned short k=0;k<len;k++)       //open hashing
        {
                total += word[k].digitValue();
        }
        return total%big_buckets;

}
unsigned short inverted_index::small_hash_fun(QString file)
{
        unsigned short total=0;
        unsigned short len = file.size();
        for (unsigned short k=0;k<len;k++)   //open hashing
        {
                total += file[k].digitValue();
        }

        return total%small_buckets;
}
bool inverted_index::is_word_found(QString word)
{
        unsigned short big_index = big_hash_fun(word);
        for (auto it=(big_array[big_index]).begin();it!=(big_array[big_index]).end();it++)
        {
                if (it->word_name == word)
                        return true;
        }
        return false;
}
void inverted_index::insert_word(QString word)
{
        unsigned short big_index = big_hash_fun(word);
        big_data_type temp_instance;
        temp_instance.word_name = word;
        (big_array[big_index]).push_back(temp_instance);
}
bool inverted_index::is_file_found(QString word,QString file)
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
void inverted_index::insert_file(QString word,QString file)
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
void inverted_index::increase_word_count(QString word,QString file)
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


void inverted_index::build_inverted_index(QStringList &wordsList,QString file,bool done)
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

    }
}

void inverted_index::traverse_inverted_index() // just for testing , will  be removed when building real database
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

list<small_data_type>* inverted_index::search(QString word)
{
    info_list.clear();
    unsigned short big_index = big_hash_fun(word);
    for (auto big_it=(big_array[big_index]).begin();big_it!=(big_array[big_index]).end();big_it++)
    {
        if (big_it->word_name == word)
        {
            for (unsigned short i=0;i<small_buckets;i++)
            {
                for (auto small_it=(big_it->small_arr)[i].begin();
                    small_it!=(big_it->small_arr)[i].end();small_it++)
                {
                    info_list.push_back(*small_it);
                }
            }
            return &info_list;
        }
    }

    return &info_list;
}
bool inverted_index::createDB(QDir dir)
{
    qDebug()<<"here";
    QString absolute_file_path = dir.absoluteFilePath(".ii");
    QFile file(absolute_file_path);
    if (!file.open(QIODevice::WriteOnly)) {
                return false;
            }

    else {
        QTextStream out(&file);
        for (unsigned short i=0;i<big_buckets;i++)
        {
                if (!((big_array[i]).empty()))
                {
                    out << i <<" " <<big_array[i].size()<<"\n";
                        for (auto big_it=(big_array[i]).begin();big_it!=(big_array[i]).end();big_it++)
                        {
                                out<< big_it->word_name <<"\n";
                                for (unsigned short j=0;j<small_buckets;j++)
                                {
                                        if (!(((big_it->small_arr)[j]).empty()))
                                        {
                                                for (auto small_it=((big_it->small_arr)[j]).begin();
                                                                small_it!=((big_it->small_arr)[j]).end();small_it++)
                                                {
                                                    out << small_it->file_name<<" "<< QString::number(small_it->word_count)<<" "   ;

                                                }

                                        }
                                }
                                out <<"\n";
                        }
                }

        }
        file.close();

                return true;
    }

}
bool inverted_index::returnDB(QDir dir)
{
    QString absolute_file = dir.absoluteFilePath(".ii");
    QFile file(absolute_file);
     //big_array->erase(big_array->begin(),big_array->end());
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() <<"false";
        return false;
            }
        QTextStream in(&file);

        while (!in.atEnd())
           {
              big_data_type_ass big;
              small_data_type files;
              QString line = in.readLine();
              QStringList list = line.split(" ");
              int bigit=list[0].toInt();
              int bigsize=list[1].toInt();

              qDebug() <<bigit<<" "<<bigsize;
              for (int i=0;i<bigsize;i++)
              {
                  QString newline = in.readLine();
                  //qDebug()<<newline;
                   big.word_name=newline;
                   newline = in.readLine();
                   list = newline.split(" ");
                   for (int i=0;i<list.size()-1;i+=2)
                   {
                       files.file_name=list[i];
                       files.word_count=list[i+1].toUShort();
                       //qDebug() <<files.file_name<<" "<<files.word_count;
                       big.small_arr.push_back(files);
                   }
                   bigArray[bigit].push_back(big);
                   //qDebug() <<bigArray[bigit].last().word_name<<bigArray[bigit].last().small_arr.last().file_name;
              }

         }
        return true;
}
