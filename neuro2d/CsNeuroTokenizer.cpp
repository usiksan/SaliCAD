#include "CsNeuroTokenizer.h"

#include <QDebug>

//Полезные слова
const char *words[] = {
  "вывод вывода выводами выводов выводу",
  "шаг шагу шагом",
  "ряд рядов рядами ряда рядом",
  "колонка колонки колонок колонками колонкой",
  "вниз внизу ниже",
  "вверх вверху выше",
  "слева влево левее",
  "справа вправо правее",
  "смести смещен смещение смещением",
  "и",
  "на",
  "с",
  "также",
  nullptr
};


//Бесполезные слова
const char *water[] = {
  "нарисуй",
  "изобрази",
  "из",
  nullptr
};

//Ряд из 5 выводов с шагом 1.27.
//Два ряда 5 и 4 вывода с шагом 2.54. Второй ряд смещен на 1.27 2.54 вниз и вправо

CsNeuroTokenizer::CsNeuroTokenizer()
  {
  //0 - пустой токен
  //1 - число
  //2 .!
  //3 ,
  //Заполнить таблицу токенами
  for( int i = 0; words[i] != nullptr; i++ ) {
    QString line( words[i] );
    QStringList list = line.split( QChar(' ') );
    for( auto word : list )
      mTokens.insert( word, i+4 );
    }

  //Числительные
  mNumbers.insert( QString("один"), 1 );
  mNumbers.insert( QString("одна"), 1 );
  mNumbers.insert( QString("первый"), 1 );
  mNumbers.insert( QString("первым"), 1 );
  mNumbers.insert( QString("первая"), 1 );
  mNumbers.insert( QString("первой"), 1 );
  mNumbers.insert( QString("первого"), 1 );
  mNumbers.insert( QString("первому"), 1 );

  mNumbers.insert( QString("два"), 2 );
  mNumbers.insert( QString("две"), 2 );
  mNumbers.insert( QString("второй"), 2 );
  mNumbers.insert( QString("вторым"), 2 );
  mNumbers.insert( QString("вторая"), 2 );
  mNumbers.insert( QString("второго"), 2 );
  mNumbers.insert( QString("второму"), 2 );

  mNumbers.insert( QString("три"), 3 );
  mNumbers.insert( QString("третий"), 3 );
  mNumbers.insert( QString("третьим"), 3 );
  mNumbers.insert( QString("третья"), 3 );
  mNumbers.insert( QString("третьей"), 3 );
  mNumbers.insert( QString("третьего"), 3 );
  mNumbers.insert( QString("третьему"), 3 );

  mNumbers.insert( QString("четыре"), 4 );
  mNumbers.insert( QString("четвертый"), 4 );
  mNumbers.insert( QString("четвертым"), 4 );
  mNumbers.insert( QString("четвертая"), 4 );
  mNumbers.insert( QString("четвертой"), 4 );
  mNumbers.insert( QString("четвертого"), 4 );
  mNumbers.insert( QString("четвертому"), 4 );

  mNumbers.insert( QString("пять"), 5 );
  mNumbers.insert( QString("пятый"), 5 );
  mNumbers.insert( QString("пятым"), 5 );
  mNumbers.insert( QString("пятая"), 5 );
  mNumbers.insert( QString("пятой"), 5 );
  mNumbers.insert( QString("пятого"), 5 );
  mNumbers.insert( QString("пятому"), 5 );

  mNumbers.insert( QString("шесть"), 6 );
  mNumbers.insert( QString("шестой"), 6 );
  mNumbers.insert( QString("шестым"), 6 );
  mNumbers.insert( QString("шестая"), 6 );
  mNumbers.insert( QString("шестого"), 6 );
  mNumbers.insert( QString("шестому"), 6 );

  mNumbers.insert( QString("семь"), 7 );
  mNumbers.insert( QString("седьмой"), 7 );
  mNumbers.insert( QString("седьмым"), 7 );
  mNumbers.insert( QString("седьмая"), 7 );
  mNumbers.insert( QString("седьмого"), 7 );
  mNumbers.insert( QString("седьмому"), 7 );

  mNumbers.insert( QString("восемь"), 8 );
  mNumbers.insert( QString("восьмой"), 8 );
  mNumbers.insert( QString("восьмым"), 8 );
  mNumbers.insert( QString("восьмая"), 8 );
  mNumbers.insert( QString("восьмого"), 8 );
  mNumbers.insert( QString("восьмому"), 8 );

  mNumbers.insert( QString("девять"), 9 );
  mNumbers.insert( QString("девятый"), 9 );
  mNumbers.insert( QString("девятым"), 9 );
  mNumbers.insert( QString("девятая"), 9 );
  mNumbers.insert( QString("девятой"), 9 );
  mNumbers.insert( QString("девятого"), 9 );
  mNumbers.insert( QString("девятому"), 9 );

  mNumbers.insert( QString("десять"), 10 );
  mNumbers.insert( QString("десятый"), 10 );
  mNumbers.insert( QString("десятым"), 10 );
  mNumbers.insert( QString("десятая"), 10 );
  mNumbers.insert( QString("десятой"), 10 );
  mNumbers.insert( QString("десятого"), 10 );
  mNumbers.insert( QString("десятому"), 10 );

  mNumbers.insert( QString("одинадцать"), 11 );
  mNumbers.insert( QString("одинадцатый"), 11 );
  mNumbers.insert( QString("одинадцатым"), 11 );
  mNumbers.insert( QString("одинадцатая"), 11 );
  mNumbers.insert( QString("одинадцатой"), 11 );
  mNumbers.insert( QString("одинадцатого"), 11 );
  mNumbers.insert( QString("одинадцатому"), 11 );

  mNumbers.insert( QString("двенадцать"), 12 );
  mNumbers.insert( QString("двенадцатый"), 12 );
  mNumbers.insert( QString("двенадцатым"), 12 );
  mNumbers.insert( QString("двенадцатая"), 12 );
  mNumbers.insert( QString("двенадцатой"), 12 );
  mNumbers.insert( QString("двенадцатого"), 12 );
  mNumbers.insert( QString("двенадцатому"), 12 );

  mNumbers.insert( QString("тринадцать"), 13 );
  mNumbers.insert( QString("тринадцатый"), 13 );
  mNumbers.insert( QString("тринадцатым"), 13 );
  mNumbers.insert( QString("тринадцатая"), 13 );
  mNumbers.insert( QString("тринадцатой"), 13 );
  mNumbers.insert( QString("тринадцатого"), 13 );
  mNumbers.insert( QString("тринадцатому"), 13 );

  mNumbers.insert( QString("четырнадцать"), 14 );
  mNumbers.insert( QString("четырнадцатый"), 14 );
  mNumbers.insert( QString("четырнадцатым"), 14 );
  mNumbers.insert( QString("четырнадцатая"), 14 );
  mNumbers.insert( QString("четырнадцатой"), 14 );
  mNumbers.insert( QString("четырнадцатого"), 14 );
  mNumbers.insert( QString("четырнадцатому"), 14 );

  mNumbers.insert( QString("пятнадцать"), 15 );
  mNumbers.insert( QString("пятнадцатый"), 15 );
  mNumbers.insert( QString("пятнадцатым"), 15 );
  mNumbers.insert( QString("пятнадцатая"), 15 );
  mNumbers.insert( QString("пятнадцатой"), 15 );
  mNumbers.insert( QString("пятнадцатого"), 15 );
  mNumbers.insert( QString("пятнадцатому"), 15 );

  mNumbers.insert( QString("шестнадцать"), 16 );
  mNumbers.insert( QString("шестнадцатый"), 16 );
  mNumbers.insert( QString("шестнадцатым"), 16 );
  mNumbers.insert( QString("шестнадцатая"), 16 );
  mNumbers.insert( QString("шестнадцатой"), 16 );
  mNumbers.insert( QString("шестнадцатого"), 16 );
  mNumbers.insert( QString("шестнадцатому"), 16 );

  mNumbers.insert( QString("семнадцать"), 17 );
  mNumbers.insert( QString("семнадцатый"), 17 );
  mNumbers.insert( QString("семнадцатым"), 17 );
  mNumbers.insert( QString("семнадцатая"), 17 );
  mNumbers.insert( QString("семнадцатой"), 17 );
  mNumbers.insert( QString("семнадцатого"), 17 );
  mNumbers.insert( QString("семнадцатому"), 17 );

  mNumbers.insert( QString("восемнадцать"), 18 );
  mNumbers.insert( QString("восемнадцатый"), 18 );
  mNumbers.insert( QString("восемнадцатым"), 18 );
  mNumbers.insert( QString("восемнадцатая"), 18 );
  mNumbers.insert( QString("восемнадцатой"), 18 );
  mNumbers.insert( QString("восемнадцатого"), 18 );
  mNumbers.insert( QString("восемнадцатому"), 18 );

  mNumbers.insert( QString("девятнадцать"), 19 );
  mNumbers.insert( QString("девятнадцатый"), 19 );
  mNumbers.insert( QString("девятнадцатым"), 19 );
  mNumbers.insert( QString("девятнадцатая"), 19 );
  mNumbers.insert( QString("девятнадцатой"), 19 );
  mNumbers.insert( QString("девятнадцатого"), 19 );
  mNumbers.insert( QString("девятнадцатому"), 19 );

  mNumbers.insert( QString("двадцать"), 20 );
  mNumbers.insert( QString("двадцатый"), 20 );
  mNumbers.insert( QString("двадцатым"), 20 );
  mNumbers.insert( QString("двадцатая"), 20 );
  mNumbers.insert( QString("двадцатой"), 20 );
  mNumbers.insert( QString("двадцатого"), 20 );
  mNumbers.insert( QString("двадцатому"), 20 );

  mNumbers.insert( QString("тридцать"), 30 );
  mNumbers.insert( QString("тридцатый"), 30 );
  mNumbers.insert( QString("тридцатым"), 30 );
  mNumbers.insert( QString("тридцатая"), 30 );
  mNumbers.insert( QString("тридцатой"), 30 );
  mNumbers.insert( QString("тридцатого"), 30 );
  mNumbers.insert( QString("тридцатому"), 30 );

  mNumbers.insert( QString("сорок"), 40 );
  mNumbers.insert( QString("сороковой"), 40 );
  mNumbers.insert( QString("сороковым"), 40 );
  mNumbers.insert( QString("сороковая"), 40 );
  mNumbers.insert( QString("сорокового"), 40 );
  mNumbers.insert( QString("сороковому"), 40 );

  mNumbers.insert( QString("пятьдесят"), 50 );
  mNumbers.insert( QString("пятидесятый"), 50 );
  mNumbers.insert( QString("пятидесятым"), 50 );
  mNumbers.insert( QString("пятидесятая"), 50 );
  mNumbers.insert( QString("пятидесятой"), 50 );
  mNumbers.insert( QString("пятидесятого"), 50 );
  mNumbers.insert( QString("пятидесятому"), 50 );

  mNumbers.insert( QString("шестьдесят"), 60 );
  mNumbers.insert( QString("шестидесятый"), 60 );
  mNumbers.insert( QString("шестидесятым"), 60 );
  mNumbers.insert( QString("шестидесятая"), 60 );
  mNumbers.insert( QString("шестидесятой"), 60 );
  mNumbers.insert( QString("шестидесятого"), 60 );
  mNumbers.insert( QString("шестидесятому"), 60 );

  mNumbers.insert( QString("семьдесят"), 70 );
  mNumbers.insert( QString("семидесятый"), 70 );
  mNumbers.insert( QString("семидесятым"), 70 );
  mNumbers.insert( QString("семидесятая"), 70 );
  mNumbers.insert( QString("семидесятой"), 70 );
  mNumbers.insert( QString("семидесятого"), 70 );
  mNumbers.insert( QString("семидесятому"), 70 );

  mNumbers.insert( QString("восемьдесят"), 80 );
  mNumbers.insert( QString("восьмидесятый"), 80 );
  mNumbers.insert( QString("восьмидесятым"), 80 );
  mNumbers.insert( QString("восьмидесятая"), 80 );
  mNumbers.insert( QString("восьмидесятой"), 80 );
  mNumbers.insert( QString("восьмидесятого"), 80 );
  mNumbers.insert( QString("восьмидесятому"), 80 );

  mNumbers.insert( QString("девяносто"), 90 );
  mNumbers.insert( QString("девяностый"), 90 );
  mNumbers.insert( QString("девяностым"), 90 );
  mNumbers.insert( QString("девяностая"), 90 );
  mNumbers.insert( QString("девяностой"), 90 );
  mNumbers.insert( QString("девяностого"), 90 );
  mNumbers.insert( QString("девяностому"), 90 );

  mNumbers.insert( QString("сто"), 100 );
  mNumbers.insert( QString("сотый"), 100 );
  mNumbers.insert( QString("сотым"), 100 );
  mNumbers.insert( QString("сотая"), 100 );
  mNumbers.insert( QString("сотой"), 100 );
  mNumbers.insert( QString("сотого"), 100 );
  mNumbers.insert( QString("сотому"), 100 );

  mNumbers.insert( QString("двести"), 200 );
  mNumbers.insert( QString("двухсотый"), 200 );
  mNumbers.insert( QString("двухсотым"), 200 );
  mNumbers.insert( QString("двухсотая"), 200 );
  mNumbers.insert( QString("двухсотой"), 200 );
  mNumbers.insert( QString("двухсотого"), 200 );
  mNumbers.insert( QString("двухсотому"), 200 );

  mNumbers.insert( QString("триста"), 300 );
  mNumbers.insert( QString("трехсотый"), 300 );
  mNumbers.insert( QString("трехсотым"), 300 );
  mNumbers.insert( QString("трехсотая"), 300 );
  mNumbers.insert( QString("трехсотой"), 300 );
  mNumbers.insert( QString("трехсотого"), 300 );
  mNumbers.insert( QString("трехсотому"), 300 );

  mNumbers.insert( QString("четыреста"), 400 );
  mNumbers.insert( QString("четырехсотый"), 400 );
  mNumbers.insert( QString("четырехсотым"), 400 );
  mNumbers.insert( QString("четырехсотая"), 400 );
  mNumbers.insert( QString("четырехсотой"), 400 );
  mNumbers.insert( QString("четырехсотого"), 400 );
  mNumbers.insert( QString("четырехсотому"), 400 );

  mNumbers.insert( QString("пятьсот"), 500 );
  mNumbers.insert( QString("пятисотый"), 500 );
  mNumbers.insert( QString("пятисотым"), 500 );
  mNumbers.insert( QString("пятисотая"), 500 );
  mNumbers.insert( QString("пятисотой"), 500 );
  mNumbers.insert( QString("пятисотого"), 500 );
  mNumbers.insert( QString("пятисотому"), 500 );

  mNumbers.insert( QString("шестьсот"), 600 );
  mNumbers.insert( QString("шестисотый"), 600 );
  mNumbers.insert( QString("шестисотым"), 600 );
  mNumbers.insert( QString("шестисотая"), 600 );
  mNumbers.insert( QString("шестисотой"), 600 );
  mNumbers.insert( QString("шестисотого"), 600 );
  mNumbers.insert( QString("шестисотому"), 600 );

  mNumbers.insert( QString("семьсот"), 700 );
  mNumbers.insert( QString("семисотый"), 700 );
  mNumbers.insert( QString("семисотым"), 700 );
  mNumbers.insert( QString("семисотая"), 700 );
  mNumbers.insert( QString("семисотой"), 700 );
  mNumbers.insert( QString("семисотого"), 700 );
  mNumbers.insert( QString("семисотому"), 700 );

  mNumbers.insert( QString("восемьсот"), 800 );
  mNumbers.insert( QString("восьмисотый"), 800 );
  mNumbers.insert( QString("восьмисотым"), 800 );
  mNumbers.insert( QString("восьмисотая"), 800 );
  mNumbers.insert( QString("восьмисотой"), 800 );
  mNumbers.insert( QString("восьмисотого"), 800 );
  mNumbers.insert( QString("восьмисотому"), 800 );

  mNumbers.insert( QString("девятьсот"), 900 );
  mNumbers.insert( QString("девятисотый"), 900 );
  mNumbers.insert( QString("девятисотым"), 900 );
  mNumbers.insert( QString("девятисотая"), 900 );
  mNumbers.insert( QString("девятисотой"), 900 );
  mNumbers.insert( QString("девятисотого"), 900 );
  mNumbers.insert( QString("девятисотому"), 900 );

  mNumbers.insert( QString("тысяча"), 1000 );
  mNumbers.insert( QString("тысяч"), 1000 );

  }




QList<int> CsNeuroTokenizer::tokenize(const QString &src)
  {
  bool isNumber = false;
  double number;
  mValues.clear();
  QList<int> out;
  for( int i = 0; i < src.length(); ) {
    if( src.at(i).isSpace() ) {
      i++;
      continue;
      }

    if( src.at(i) == QChar('.') || src.at(i) == QChar('!') ) {
      if( isNumber ) {
        //Ранее было числительное
        mValues.append( number );
        isNumber = false;
        out.append(1);
        }
      out.append(2);
      i++;
      continue;
      }

    if( src.at(i) == QChar(',') ) {
      if( isNumber ) {
        //Ранее было числительное
        mValues.append( number );
        isNumber = false;
        out.append(1);
        }
      out.append(3);
      i++;
      continue;
      }

    if( src.at(i).isLetter() ) {
      //Сканируем слово
      QString name;
      while( i < src.length() && src.at(i).isLetter() ) {
        name.append( src.at(i).toLower() );
        i++;
        }

      //Проверим числительные
      if( mNumbers.contains(name) ) {
        //Это числительное
        if( isNumber ) number += mNumbers.value(name);
        else number = mNumbers.value(name);
        isNumber = true;
        continue;
        }

      if( isNumber ) {
        //Ранее было числительное
        mValues.append( number );
        isNumber = false;
        out.append(1);
        }

      //Проверим бесполезные слова
      if( mWater.contains(name) )
        continue;

      //Проверим токен
      if( mTokens.contains(name) ) {
        out.append( mTokens.value(name) );
        continue;
        }

      qDebug() << "Add token " << name;
      continue;
      }

    if( isNumber ) {
      //Ранее было числительное
      mValues.append( number );
      isNumber = false;
      out.append(1);
      }

    //Проверим числа
    if( src.at(i).isDigit() ) {
      //Сканируем начало числа
      QString num;
      bool wasPoint = false;
      while( i < src.length() && ( src.at(i).isDigit() || (!wasPoint && src.at(i) == QChar('.') ) ) ) {
        num.append( src.at(i) );
        //Проверим точку и установим флаг, если она была
        if( src.at(i) == QChar('.') )
          wasPoint = true;
        i++;
        }
      //Число отсканировано
      mValues.append( num.toDouble() );
      out.append(1);
      continue;
      }

    //Это какая-то непонятная хрень
    i++;
    }
  return out;
  }
