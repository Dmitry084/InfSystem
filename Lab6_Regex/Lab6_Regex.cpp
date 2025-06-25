#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <Windows.h>
#include <regex>
#include <vector>
#include <string>
#include <cctype>
#include <map>
#include <stack>
#include <unordered_map>
#include <windows.h>
#include <algorithm>
#include <ctime>
#include <sstream>
#include <io.h>
#include <fcntl.h>
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
            cout << "Неправильный формат цвета. Попробуйте ещё раз" << endl;
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

string replace(const string& str, const string& from, const string& to) {
    string result = str;
    size_t pos = result.find(from);
    if (pos != string::npos) {
        result.replace(pos, from.length(), to);
    }
    return result;
}
bool is_number(const string& s) {
    return !s.empty() && all_of(s.begin(), s.end(), ::isdigit);
}
bool is_valid_date(int day, int month, int year) {
    if (year < 0 || month < 1 || month > 12 || day < 1 || day > 31) {
        return false;
    }
    int days_in_month[] = { 31, 28, 31, 30, 31, 30,
                            31, 31, 30, 31, 30, 31 };
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        days_in_month[1] = 29;
    }
    return day <= days_in_month[month - 1];
}
vector<string> split(const string& s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    return tokens;
}
bool validate_date(const string& date_str) {
    unordered_map<string, string> month_rus = {
        {"января", "January"}, {"февраля", "February"}, {"марта", "March"},
        {"апреля", "April"}, {"мая", "May"}, {"июня", "June"},
        {"июля", "July"}, {"августа", "August"}, {"сентября", "September"},
        {"октября", "October"}, {"ноября", "November"}, {"декабря", "December"}
    };
    bool rus_found = false, eng_found = false;
    for (const auto& pair : month_rus) {
        if (date_str.find(pair.first) != string::npos) {
            rus_found = true;
        }
        if (date_str.find(pair.second) != string::npos) {
            eng_found = true;
        }
    }
    if (rus_found && eng_found) {
        return false;
    }
    for (const auto& pair : month_rus) {
        if (date_str.find(pair.first) == 0) {
            return false;
        }
    }
    string modified_date = date_str;
    for (const auto& pair : month_rus) {
        if (modified_date.find(pair.first) != string::npos) {
            modified_date = replace(modified_date, pair.first, pair.second);
            break;
        }
    }
    vector<int> orderings = {
        0,
        1,
        2
    };

    for (int ordering : orderings) {
        istringstream iss(modified_date);
        string token;
        vector<string> parts;
        if (ordering == 0 || ordering == 1) {
            char separators[] = { '.', '/', '-', ' ' };
            for (char sep : separators) {
                parts = split(modified_date, sep);
                if (parts.size() == 3) {
                    break;
                }
            }
        }
        else {
            while (iss >> token) {
                parts.push_back(token);
            }
        }
        if (parts.size() != 3) {
            continue;
        }
        int d = -1, m = -1, y = -1;
        switch (ordering) {
        case 0:
            if (!is_number(parts[0]) || !is_number(parts[1]) || !is_number(parts[2])) {
                continue;
            }
            d = stoi(parts[0]);
            m = stoi(parts[1]);
            y = stoi(parts[2]);
            break;
        case 1:
            if (!is_number(parts[0]) || !is_number(parts[1]) || !is_number(parts[2])) {
                continue;
            }
            y = stoi(parts[0]);
            m = stoi(parts[1]);
            d = stoi(parts[2]);
            break;
        case 2:
            static const unordered_map<string, int> months = {
                {"January", 1}, {"February", 2}, {"March", 3},
                {"April", 4}, {"May", 5}, {"June", 6},
                {"July", 7}, {"August", 8}, {"September", 9},
                {"October", 10}, {"November", 11}, {"December", 12},
                {"Jan", 1}, {"Feb", 2}, {"Mar", 3},
                {"Apr", 4}, {"May", 5}, {"Jun", 6},
                {"Jul", 7}, {"Aug", 8}, {"Sep", 9},
                {"Oct", 10}, {"Nov", 11}, {"Dec", 12}
            };
            string month_str = parts[0];
            if (parts[1].back() == ',') {
                parts[1].pop_back();
            }
            if (!months.count(month_str) || !is_number(parts[1]) || !is_number(parts[2])) {
                continue;
            }
            m = months.at(month_str);
            d = stoi(parts[1]);
            y = stoi(parts[2]);
            break;
        }
        if (is_valid_date(d, m, y)) {
            return true;
        }
    }
    return false;
}
void DataIsCorrect() {
    string date = "";
    int correct = 0;
    cout << "Дата должна быть в формате:\n";
    cout << "1) день.месяц.год\n";
    cout << "2) день/месяц/год\n";
    cout << "3) день-месяц-год\n";
    cout << "4) год.месяц.день\n";
    cout << "5) год/месяц/день\n";
    cout << "6) год-месяц-день\n";
    cout << "7) день месяц_rus год\n";
    cout << "8) Месяц_eng день, год\n";
    cout << "9) Мес_eng день, год\n";
    cout << "10) год, Месяц_eng день\n";
    cout << "11) год, Мес_eng день\n";
    while (correct != 1) {
        cout << "Введите дату: ";
        getline(cin, date);
        if (validate_date(date)) {
            cout << "Дата корректна!\n";
            correct = 1;
        }
        else {
            cout << "Некорректная дата. Попробуйте снова.\n";
        }
    }
    cout << endl << endl;
}

//Часть 2
void BracketIsCorrect() {
    
    string bracket = "";
    int correct = 0;
    while (correct != 1) {
        
        cout << "Введите скобочное выражение: ";
        cin >> bracket;
        stack<char> stack;
        unordered_map<char, char> BracketMap = {
            {')', '('},
            {'}', '{'},
            {']', '['}
        };
        int count = 1;
        for (char ch : bracket) {
            if (ch == '(' || ch == '{' || ch == '[') {
                stack.push(ch);
            }
            else {
                if (ch == ')' || ch == '}' || ch == ']') {
                    if (stack.empty() || stack.top() != BracketMap[ch]) {
                        count = 0;
                        break;
                    }
                    stack.pop();
                }
                else {
                    count = 0;
                    break;
                }
            }
        }
        if (count == 1) {
            correct = 1;
            cout << "Вы правильно ввели скобочное выражение!" << endl;
        }
        else {
            cout << "Некорректное скобочное выражение. Попробуйте ещё раз." << endl;
        }
    }
}
void SetColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}
void Sentences() {
    string text = R"(
Что сразу бросается в глаза, так это нестандартная рисовка и отсутствие эмоций на лицах,
в первом сезоне "смешные" моменты были со вставками глупых лиц, как в аниме начала
2000х. Потом, видимо, поняли, что это уже не круто и от таких ходов отказались. Если не
обращать внимание на картинку, а полностью окунуться в сюжет, в принципе очень даже
смотрибельно. Интересно следить за развитием персонажа, как он сначала вершит
правосудие над обидчиками своего отца, а потом глубоко погружается в овладение магией
разного толка. Присутствует жестокость и почти нет фансервиса, что радует. Монстры от
сезона к сезону от топорных моделек переходят в состояние "неплохо", авторы исправляют
свои ошибки, как и все, за и др. что берётся копировать китайская нация. В общем вас ждет
вырвиглазная рисовка с неплохим сюжетом и поиском приемлемой озвучки.
7 из 10

Резюмируя можно сказать:
1. Герои объединяются под сомнительным предлогом;
2. Их отношения выглядят неестественно;
3. Карьерный рост Кэсси не прокатил бы даже в диснеевской сказке.
Несмотря на то, что оценка в общепринятом понимании относится к серой зоне, 
субъективно фильм оставляет приятное послевкусие.

Кратко и по пунктам:
1. Начал смотреть, потому что новый сериал по подписке.
2. Сразу "проглотил" полторы серии, запнулся на отсылке к "лихим 90-м" и бандитам, не
нравится мне такое. Решил не досматривать.
3. Через день всё-таки любопытство взяло верх. Сказал себе: если будет неожиданный
поворот в банальном сюжете, досмотрю. Поворот случился, пришлось смотреть весь
сериал.
)";
    //1-\n\d+\sиз\s\d+\n
    //2.1-\d\.[^.!?]+\.
    //2.2-\n\d\.\s[^.!?]+\.
    //3-[^.!?]+\:(\n\d\.\s[^!?.]+\;)+\n\d\.[^.!?]+\.
    //4.1-[^.!?:]+\:\n
    //4.2-[^.!?]+\.)
    const regex r(R"(\n\d+\sиз\s\d+\n|\d\.[^.!?]+\.|(\n\d\.\s[^.!?]+\.|[^.!?]+\:(\n\d\.\s[^!?.]+\;)+\n\d\.[^.!?]+\.|[А-Я][^.!?:]+\:\n|[А-Я][^.!?]+\.))");
    sregex_iterator iter(text.begin(), text.end(), r);
    sregex_iterator end;
    int colorIndex = 1;

    cout << "Найденные предложения:\n";
    while (iter != end) {
        string sentence = iter->str(0); 
        if (!sentence.empty()) {
            SetColor(colorIndex + 7);
            cout << sentence;
        }
        iter++;
        colorIndex = (colorIndex % 7) + 1;
    }
    SetColor(7);
    cout << endl << endl;
}

int main() {
    setlocale(LC_ALL, "rus");
    char input;
    int choice = 0;
    while (true) {
        cout << "Введите номер\nЧасть1:\n1) проверка корректности пароля (регулярные выражения)";
        cout << "\n2) Проверка корректности web цвета (регулярные выражения)";
        cout << "\n3) Токенизация математического выражения";
        cout << "\n4) Проверка корректности даты";
        cout << "\nЧасть 2:\n5) Проверка корректности скобочного выражения";
        cout << "\n6) Разбиение текста на предложения";
        cout << "\n7) Выход";
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
            BracketIsCorrect();
            break;
        case 6:
            Sentences();
            break;
        case 7:
            cout << "Выход из программы.\n";
            break;
        default:
            cout << "Неверный диапазон. Введите число от 1 до 7.\n";
            break;
        }
        if (choice == 7) {
            break;
        }
    }
    return 0;
}