#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <Windows.h>
#include <regex>=
#include <vector>
#include <string>
#include <cctype>
#include <map>

using namespace std;

void PasswordIsCorrect() {
    string pass = "";
    int correct = 0;
    while (correct != 7) {
        cout << "Введите пароль: ";
        cin >> pass; // rtG3FGITr^e
        const regex r1(R"(^[A-Za-z0-9^$%@#&*!]+$)");
        if (regex_match(pass, r1)) {
            correct++;
        }
        else {
            cout << "Пароль должен содержать только латинские символы, цифры и специальные символы ^$%@#&*!" << endl;
            correct = 0;
        }

        const regex r2(R"(^.{8,}$)");
        if (regex_match(pass, r2)) {
            correct++;
        }
        else {
            cout << "Пароль должен должен состоять из не менее чем восьми символов" << endl;
            correct = 0;
        }

        const regex r3(R"(^(?=.*[A-Z]).*$)");
        if (regex_match(pass, r3)) {
            correct++;
        }
        else {
            cout << "Пароль должен содержать по крайней мере один латинский символ в верхнем регистре" << endl;
            correct = 0;
        }

        const regex r4(R"(^(?=.*[a-z]).*$)");
        if (regex_match(pass, r4)) {
            correct++;
        }
        else {
            cout << "Пароль должен содержать по крайней мере один латинский символ в нижнем регистре" << endl;
            correct = 0;
        }

        const regex r5(R"(^(?=.*\d).*$)");
        if (regex_match(pass, r5)) {
            correct++;
        }
        else {
            cout << "Пароль должен содержать по крайней мере одну цифру" << endl;
            correct = 0;
        }

        const regex r6(R"((?=.*[!@#$%^&*()_+=[\]{};':",./<>?])(.*[!@#$%^&*()_+=[\]{};':",./<>?].*){1,})");
        if (regex_match(pass, r6)) {
            correct++;
        }
        else {
            cout << "Пароль должен содержать по крайней мере два различных специальных символа" << endl;
            correct = 0;
        }

        const regex r7(R"(^(?!.*(.)\1).+$)");
        if (regex_match(pass, r7)) {
            correct++;
        }
        else {
            cout << "Пароль не должен содержать двух одинаковых символов подряд" << endl;
            correct = 0;
        }
    }
    cout << "Вы ввели правильный пароль!" << endl << endl;
}

void WebColorIsCorrect() {
    string color = "";
    int correct = 0;
    cout << "Цвет должен быть в формате: \n1) rgb(255,255,255)" << endl;
    cout << "2) rgb(100%,100%,100%)" << endl;
    cout << "3) #rrggbb, где каждый цвет должен быть в шестнадцатиричном формате" << endl;
    cout << "4) hsl(255,100%,100%)" << endl;
    while (correct != 1) {
        correct = 0;
        cout << "Введите цвет: ";
        cin >> color;

        const regex r1(R"(rgb\(\s*(\d{1,3})\s*,\s*(\d{1,3})\s*,\s*(\d{1,3})\s*\))");
        if (regex_match(color, r1)) {
            correct++;
        }

        const regex r2(R"(rgb\(\s*([0-9]{1,3}%)\s*,\s*([0-9]{1,3}%)\s*,\s*([0-9]{1,3}%)\s*\))");
        if (regex_match(color, r2)) {
            correct=1;
        }
        const regex r3(R"(^#([A-Fa-f0-9]{6}|[A-Fa-f0-9]{3})$)");
        if (regex_match(color, r3) ) {
            correct=1;
        }
        const regex r4(R"(hsl\(\s*([0-9]{1,3}(?:\.[0-9]+)?|100\.0?)\s*,\s*([0-9]{1,3}(?:\.[0-9]+)?%)\s*,\s*([0-9]{1,3}(?:\.[0-9]+)?%)\s*\))");
        if (regex_match(color, r4) && correct != 1) {
            correct=1;
        }
        if (correct == 0) {
            cout << "Неправильный формат цвета, введите ещё раз" << endl;
        }
    }
    cout << "Вы правильно ввели цвет!" << endl << endl;
}

void TokenIsCorrect() {
    string token = "";
    cout << "Введите математическое выражение: ";
    getline(cin >> ws, token);
    vector<pair<string, pair<int, int>>> tokens;
    map<string, bool> functions = { {"sin", true}, {"cos", true}, {"tan", true}, {"sqrt", true}, {"log", true} };
    map<string, bool> constants = { {"pi", true}, {"e", true} };
    int count = 0;
    int tokenlenght = token.length();
    while (count < tokenlenght) {
        char c = token[count];
        if (isspace(c)) {
            count++;
            continue;
        }
        if (c == '(') {
            tokens.push_back({ "left_parenthesis", {count, count + 1} });
            count++;
            continue;
        }
        if (c == ')') {
            tokens.push_back({ "right_parenthesis", {count, count + 1} });
            count++;
            continue;
        }
        if (strchr("+-*/^", c)) {
            tokens.push_back({ "operator", {count, count + 1} });
            count++;
            continue;
        }
        if (isdigit(c) || c == '.') {
            int start = count;
            while (count < tokenlenght && (isdigit(token[count]) || token[count] == '.')) {
                count++;
            }
            tokens.push_back({ "number", {start, count} });
            continue;
        }
        if (isalpha(c)) {
            int start = count;
            while (count < tokenlenght && isalnum(token[count])) {
                count++;
            }
            string word = token.substr(start, count - start);
            if (constants.count(word)) {
                tokens.push_back({ "constant", {start, count} });
            }
            else {
                if (functions.count(word)) {
                    tokens.push_back({ "function", {start, count} });
                }
                else {
                    tokens.push_back({ "variable", {start, count} });
                }
            }
            continue;
        }
        count++;
    }
    cout << "Токены: " << endl;
    for (size_t j = 0; j < tokens.size(); ++j) {
        const auto& token = tokens[j];
        cout << "  {\"type\": \"" << token.first << "\", \"span\": [" << token.second.first << ", " << token.second.second << "]}";
        if (j != tokens.size() - 1) {
            cout << ",";
        }
        cout << endl;
    }
    cout << endl;
}

void DataIsCorrect() {//не готово
    string data = "";
    int correct = 0;
    cout << "Дата должна быть в формате:" << endl;
    cout << "1) день.месяц.год" << endl;
    cout << "2) день/месяц/год" << endl;
    cout << "3) день-месяц-год" << endl;
    cout << "4) год.месяц.день" << endl;
    cout << "5) год/месяц/день" << endl;
    cout << "6) год-месяц-день" << endl;
    cout << "7) день месяц_rus год" << endl;
    cout << "8) Месяц_eng день, год" << endl;
    cout << "9) Мес_eng день, год" << endl;
    cout << "10) год, Месяц_eng день" << endl;
    cout << "11) год, Мес_eng день" << endl;
    while (correct!=1) {
        cout << "Введите дату: ";
        getline(cin, data);
        correct = 1;
    }
}
int main()
{
    setlocale(LC_ALL, "rus");
    char input;
    int choice = 0;
    while (true) {
        cout << "Введите номер\n1) проверка корректности пароля";
        cout << "\n2) Проверка корректности web цвета";
        cout << "\n3) Токенизация математического выражения";
        cout << "\n4) Проверка корректности даты";
        cout << "\n5) Выход";
        cout << "\nВаш выбор: ";
        cin >> input;
        int choice = input - '0';
        switch (choice) {
        case 1:
            PasswordIsCorrect();
            break;
        case 2:
            WebColorIsCorrect();
            break;
        case 3:
            TokenIsCorrect();
            break;
        case 4:
            DataIsCorrect();
            break;
        case 5:
            cout << "Выход из программы.\n";
            break;
        default:
            cout << "Неверный диапазон. Введите число от 1 до 5.\n";
            break;
        }
        if (choice == 5) {
            break;
        }
    }
    return 0;
}