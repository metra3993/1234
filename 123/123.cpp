#include <iostream> // Ввод-вывод
#include <vector> // Векторы
#include <ctime> // Время
#include <chrono> // Хронометраж
#include <thread> // Потоки
#include <random> // Генерация случайных чисел

using namespace std; // Использование стандартного пространства имен
using namespace chrono; // Использование пространства имен chrono для работы с временем

enum class Role { Mafia, Citizen, Doctor, Detective }; // Перечисление ролей игроков

class Players { // Класс для представления игроков
private:
    string name; // Имя игрока
    Role role; // Роль игрока
    bool alive; // Флаг, указывающий, жив ли игрок
    bool protectedByDoctor; // Флаг, указывающий, защищен ли игрок доктором
    bool hasPerformedAction; // Флаг, указывающий, выполнил ли игрок действие в текущем цикле
    bool revived; // Флаг, указывающий, был ли игрок оживлен в текущем цикле

public:
    Players(const string& n) : name(n), alive(true), protectedByDoctor(false), hasPerformedAction(false), revived(false) {} // Конструктор класса Players

    void assignRole(Role r) { // Метод для назначения роли игроку
        role = r;
    }

    Role getRole() const { // Метод для получения роли игрока
        return role;
    }

    string getName() const { // Метод для получения имени игрока
        return name;
    }

    bool isAlive() const { // Метод для проверки, жив ли игрок
        return alive;
    }

    void kill() { // Метод для "убийства" игрока
        alive = false;
    }

    void protectByDoctor() { // Метод для защиты игрока доктором
        protectedByDoctor = true;
    }

    bool hasPerformed() const { // Метод для проверки, выполнил ли игрок действие
        return hasPerformedAction;
    }

    void setPerformed() { // Метод для установки флага выполнения действия
        hasPerformedAction = true;
    }

    bool isRevived() const { // Метод для проверки, был ли игрок оживлен
        return revived;
    }

    void revive() { // Метод для "оживления" игрока
        alive = true;
        revived = true;
    }

    void resetRoundFlags() { // Метод для сброса флагов текущего раунда
        hasPerformedAction = false;
        revived = false;
    }

    void performAction(vector<Players>& players) { // Метод для выполнения действия игрока
        switch (role) { // Выбор действия в зависимости от роли
        case Role::Mafia:
            if (!hasPerformedAction && isAlive())
                mafiaAction(players); // Действие мафии
            break;
        case Role::Doctor:
            if (!hasPerformedAction && isAlive())
                doctorAction(players); // Действие врача
            break;
        case Role::Detective:
            if (!hasPerformedAction && isAlive())
                detectiveAction(players); // Действие детектива
            break;
        default:
            break;
        }
    }

private:
    void mafiaAction(vector<Players>& players) { // Действие мафии
        cout << "Мафия, выберите игрока для убийства:" << endl; // Подсказка для выбора цели
        int targetIndex; // Индекс цели
        cin >> targetIndex; // Ввод индекса цели
        targetIndex--; // Коррекция на индексацию с нуля
        if (targetIndex >= 0 && targetIndex < players.size() && players[targetIndex].isAlive() && players[targetIndex].getRole() != Role::Mafia) { // Проверка корректности цели
            if (players[targetIndex].protectedByDoctor) { // Проверка, защищен ли выбранный игрок доктором
                cout << "Игрок " << players[targetIndex].getName() << " был защищен доктором и остается живым." << endl; // Уведомление о защите
            }
            else {
                cout << "Мафия убивает " << players[targetIndex].getName() << endl; // Уведомление об убийстве
                players[targetIndex].kill(); // "Убийство" игрока
            }
        }
        else {
            cout << "Некорректный выбор. Убийство не произошло." << endl; // Уведомление о некорректном выборе
        }
        hasPerformedAction = true; // Установка флага выполнения действия
    }

    void doctorAction(vector<Players>& players) { // Действие врача
        cout << "Врач, выберите игрока для лечения:" << endl; // Подсказка для выбора цели лечения
        int targetIndex; // Индекс цели
        cin >> targetIndex; // Ввод индекса цели
        targetIndex--; // Коррекция на индексацию с нуля

        if (targetIndex >= 0 && targetIndex < players.size() && players[targetIndex].isAlive()) { // Проверка корректности цели
            cout << "Врач лечит " << players[targetIndex].getName() << endl; // Уведомление о лечении выбранного игрока
            players[targetIndex].protectByDoctor(); // "Лечение" игрока
        }
        else {
            cout << "Некорректный выбор. Лечение не произошло." << endl; // Уведомление о некорректном выборе
        }

        // Врач также лечит себя
        protectByDoctor(); // "Лечение" врача

        hasPerformedAction = true; // Установка флага выполнения действия
    }

    void detectiveAction(const vector<Players>& players) { // Действие детектива
        cout << "Детектив, выберите игрока для расследования:" << endl; // Подсказка для выбора цели расследования
        int targetIndex; // Индекс цели
        cin >> targetIndex; // Ввод индекса цели
        targetIndex--; // Коррекция на индексацию с нуля

        if (targetIndex >= 0 && targetIndex < players.size() && players[targetIndex].isAlive() && players[targetIndex].getRole() != Role::Detective) { // Проверка корректности цели
            if (players[targetIndex].getRole() == Role::Mafia) { // Проверка, является ли выбранный игрок мафией
                cout << "Детектив расследует " << players[targetIndex].getName() << endl; // Уведомление о расследовании
                cout << players[targetIndex].getName() << " является мафией" << endl; // Результат расследования
            }
            else {
                cout << "Детектив расследует " << players[targetIndex].getName() << endl; // Уведомление о расследовании
                cout << players[targetIndex].getName() << " не является мафией" << endl; // Результат расследования
            }
        }
        else {
            cout << "Некорректный выбор. Расследование не проведено." << endl; // Уведомление о некорректном выборе
        }
        hasPerformedAction = true; // Установка флага выполнения действия
    }
};

class GameMaster { // Класс для управления игрой
public:
    void viewRoles(const vector<Players>& players) { // Метод для отображения ролей игроков
        cout << "Роли у игроков:" << endl; // Уведомление о выводе ролей
        for (const auto& player : players) { // Перебор всех игроков
            if (player.isAlive()) { // Проверка, жив ли игрок
                cout << player.getName() << ": "; // Вывод имени игрока
                switch (player.getRole()) { // Определение роли игрока
                case Role::Mafia:
                    cout << "Мафия" << endl; // Вывод роли "Мафия"
                    break;
                case Role::Citizen:
                    cout << "Мирный житель" << endl; // Вывод роли "Мирный житель"
                    break;
                case Role::Doctor:
                    cout << "Врач" << endl; // Вывод роли "Врач"
                    break;
                case Role::Detective:
                    cout << "Детектив" << endl; // Вывод роли "Детектив"
                    break;
                default:
                    break;
                }
            }
        }
    }
};

class Game { // Класс для представления игры
private:
    vector<Players> players; // Вектор игроков
    int mafiaCount; // Количество мафии
    int citizenCount; // Количество мирных жителей
    int doctorCount; // Количество врачей
    int detectiveCount; // Количество детективов
    int mafiaWins; // Количество побед мафии
    int citizensWins; // Количество побед мирных жителей
    GameMaster gameMaster; // Объект класса GameMaster для управления игрой

    void assignRoles() { // Метод для назначения ролей игрокам
        mafiaCount = 0; // Обнуление количества мафии
        citizenCount = 0; // Обнуление количества мирных жителей
        doctorCount = 0; // Обнуление количества врачей
        detectiveCount = 0; // Обнуление количества детективов

        vector<Role> allRoles; // Вектор всех ролей

        // Назначение ролей мафии, врачей и детективов
        allRoles.insert(allRoles.end(), 2, Role::Mafia); // Две роли мафии
        allRoles.push_back(Role::Doctor); // Одна роль врача
        allRoles.push_back(Role::Detective); // Одна роль детектива
        int remainingCitizens = players.size() - allRoles.size(); // Вычисление оставшегося количества мирных жителей
        allRoles.insert(allRoles.end(), remainingCitizens, Role::Citizen); // Добавление оставшихся ролей мирных жителей

        random_device rd; // Генератор случайных чисел
        mt19937 g(rd()); // Использование генератора случайных чисел
        shuffle(allRoles.begin(), allRoles.end(), g); // Перемешивание ролей

        // Назначение ролей игрокам
        for (int i = 0; i < players.size(); ++i) {
            players[i].assignRole(allRoles[i]);
            switch (allRoles[i]) {
            case Role::Mafia:
                mafiaCount++;
                break;
            case Role::Citizen:
                citizenCount++;
                break;
            case Role::Doctor:
                doctorCount++;
                break;
            case Role::Detective:
                detectiveCount++;
                break;
            default:
                break;
            }
        }
    }

    void startDay() { // Начало дня
        cout << "Доброе утро, дебилы!" << endl; // Приветствие игроков
        gameMaster.viewRoles(players); // Просмотр ролей игроков
    }

    void startNight() { // Начало ночи
        cout << "Наступает ночь" << endl; // Уведомление о начале ночи
        if (!players.empty()) { // Проверка, есть ли игроки
            for (auto& player : players) {
                player.performAction(players); // Выполнение действий всех игроков
            }
            for (auto& player : players) {
                player.resetRoundFlags(); // Сброс флагов текущего раунда
            }
        }
    }

    void vote() { // Голосование
        cout << "Голосование" << endl; // Уведомление о начале голосования
        vector<int> votes(players.size(), 0); // Вектор для подсчета голосов
        for (int i = 0; i < players.size(); ++i) { // Перебор всех игроков
            if (players[i].isAlive()) { // Проверка, жив ли игрок
                cout << players[i].getName() << ", за кого голосуете (укажите номер игрока): "; // Уведомление о голосовании для игрока
                int voteIndex; // Индекс цели голосования
                cin >> voteIndex; // Ввод индекса цели голосования
                voteIndex--; // Коррекция на индексацию с нуля
                if (voteIndex >= 0 && voteIndex < players.size() && players[voteIndex].isAlive()) { // Проверка корректности голоса
                    votes[voteIndex]++; // Увеличение счетчика голосов
                }
                else {
                    cout << "Некорректный выбор. Ваш голос не засчитан." << endl; // Уведомление о некорректном выборе
                }
            }
            else {
                cout << players[i].getName() << " выбыл из игры и не может голосовать." << endl; // Уведомление о невозможности голосования для выбывшего игрока
            }
        }

        int maxVotes = 0; // Максимальное количество голосов
        int maxIndex = -1; // Индекс игрока с максимальным количеством голосов
        for (int i = 0; i < votes.size(); ++i) { // Перебор всех голосов
            if (votes[i] > maxVotes && players[i].isAlive()) { // Проверка, больше ли количество голосов текущего игрока максимального
                maxVotes = votes[i]; // Обновление максимального количества голосов
                maxIndex = i; // Обновление индекса игрока с максимальным количеством голосов
            }
        }

        if (maxIndex != -1) { // Проверка, был ли выбран кандидат на выбывание
            cout << "Игрок " << players[maxIndex].getName() << " выбывает из игры." << endl; // Уведомление о выбывании игрока
            players[maxIndex].kill(); // "Убийство" игрока
        }
        else {
            cout << "Никто не выбыл из игры." << endl; // Уведомление о том, что никто не выбыл
        }
    }

    bool checkWin() { // Проверка победы
        int mafiaAlive = 0; // Количество живых мафии
        int citizenAlive = 0; // Количество живых мирных жителей
        for (const auto& player : players) { // Перебор всех игроков
            if (player.isAlive()) { // Проверка, жив ли игрок
                if (player.getRole() == Role::Mafia) { // Проверка, является ли игрок мафией
                    mafiaAlive++; // Увеличение количества живых мафии
                }
                else {
                    citizenAlive++; // Увеличение количества живых мирных жителей
                }
            }
        }

        if (mafiaAlive == 0) { // Проверка, победила ли мафия
            cout << "Мафия проиграла." << endl; // Уведомление о поражении мафии
            return true; // Возвращение значения true, обозначающего окончание игры
        }
        else if (citizenAlive <= mafiaAlive) { // Проверка, победили ли мирные жители
            cout << "Город проиграл." << endl; // Уведомление о поражении мирных жителей
            return true; // Возвращение значения true, обозначающего окончание игры
        }
        return false; // Возвращение значения false, обозначающего продолжение игры
    }

public:
    Game() : mafiaCount(0), citizenCount(0), doctorCount(0), detectiveCount(0), mafiaWins(0), citizensWins(0) {} // Конструктор класса Game

    void addPlayer(const string& name) { // Метод для добавления игрока
        players.push_back(Players(name)); // Добавление нового игрока в вектор
    }

    void start() { // Метод для начала игры
        assignRoles(); // Назначение ролей игрокам
        while (!checkWin()) { // Проверка победы
            startDay(); // Начало дня
            cout << "\nОбсуждение начинается " << endl; // Уведомление о начале обсуждения
            this_thread::sleep_for(seconds(5)); // Задержка на 5 секунд
            vote(); // Голосование
            this_thread::sleep_for(seconds(5)); // Задержка на 5 секунд
            startNight(); // Начало ночи
        }
    }
};

int main() { // Главная функция программы
    setlocale(LC_ALL, "Rus"); // Установка локали для корректного отображения русского текста

    srand(time(0)); // Инициализация генератора случайных чисел
    Game game; // Создание объекта класса Game
    GameMaster gameMaster; // Создание объекта класса GameMaster
    game.addPlayer("Игрок 1"); // Добавление игрока 1
    game.addPlayer("Игрок 2"); // Добавление игрока 2
    game.addPlayer("Игрок 3"); // Добавление игрока 3
    game.addPlayer("Игрок 4"); // Добавление игрока 4
    game.addPlayer("Игрок 5"); // Добавление игрока 5
    game.addPlayer("Игрок 6"); // Добавление игрока 6
    game.start(); // Начало игры

    return 0; // Возвращение значения 0, обозначающего успешное завершение программы
}
