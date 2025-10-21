#include "../include/UI.hpp"

void UI::printMenu() {
	cout << "\n------------------- Меню -------------------\n";
    cout << "1. Cгенерировать универсум мультимножеств\n";
    cout << "2. Заполнить множества A и B вручную\n";
    cout << "3. Заполнить множества A и B автоматически\n";
    cout << "4. Выполнить операции над множествами\n";
    cout << "5. Вывести множество\n";
    cout << "6. Выход\n";
    cout << "--------------------------------------------\n";
}

void UI::printSetMenu() {
	cout << "\n----- Выбор множества -----\n";
    cout << "1. Вывести универсум\n";
    cout << "2. Вывести множество A\n";
    cout << "3. Вывести множество B\n";
    cout << "4. Вернуться в главное меню\n";
    cout << "---------------------------\n";
}

void UI::printOpMenu() {
	 cout << "\n---------- Выбор операции -----------\n";
    cout << "1. Объединение            (A ∪ B)\n";
    cout << "2. Пересечение            (A ∩ B)\n";
    cout << "3. Дополнение к A         (U \\ A)\n";
    cout << "4. Дополнение к B         (U \\ B)\n";
    cout << "5. Разность               (A \\ B)\n";
    cout << "6. Разность               (B \\ A)\n";
    cout << "7. Симм. разность         (A ∆ B)\n";
    cout << "8. Сумма                  (A + B)\n";
    cout << "9. Арифм. разность        (A - B)\n";
    cout << "10. Арифм. разность       (B - A)\n";
    cout << "11. Произведение          (A * B)\n";
    cout << "12. Деление               (A / B)\n";
    cout << "13. Деление               (B / A)\n";
    cout << "14. Главное меню\n";
    cout << "-------------------------------------\n";
}

// --- Основной цикл ---
void UI::run() {
    bool running = true;
    while (running) {
        printMenu();
        cout << "Введите номер пункта: ";

        int choice;
        cin >> choice;
        if (!cin) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Ошибка ввода! Повторите.\n";
            continue;
        }

        switch (choice) {
            case 1: handleGenerateUniverse(); break;
            case 2: handleManualFill(); break;
            case 3: handleAutoFill(); break;
            case 4: handleOpMenu(); break;
            case 5: handleSetMenu(); break;
            case 6: running = false; break;
            default: cout << "Неверный пункт меню!\n"; break;
        }
    }
}

// --- Обработчики подменю ---

void UI::handleGenerateUniverse() {
    int n;
    cout << "Введите разрядность кода Грея: ";
    cin >> n;
    if (n < 0) {
        cout << "Ошибка: разрядность должна быть положительной или равная нулю!\n";
        return;
    }

    Multiset::Universum.fillUniverse(n);
    Multiset::Universum.print("\nСгенерированный универсум:\n");
}

void UI::handleManualFill() {
    if (Multiset::Universum.isEmpty()) {
        cout << "Универсум пуст — множества A и B будут пустыми.\n";
        A.fillByHand();
        B.fillByHand();
        return;
    }

    cout << "\nЗаполнение множества A вручную:\n";
    A.fillByHand();

    cout << "\nЗаполнение множества B вручную:\n";
    B.fillByHand();
}

void UI::handleAutoFill() {
    if (Multiset::Universum.isEmpty()) {
        cout << "Универсум пуст — множества A и B будут пустыми.\n";
        A.fillAutomaticly(0);
        B.fillAutomaticly(0);
        return;
    }

    int aSize, bSize;
    cout << "Введите мощность множества A (0.." << Multiset::Universum.getCardinality() << "): ";
    cin >> aSize;
    A.fillAutomaticly(aSize);

    cout << "Введите мощность множества B (0.." << Multiset::Universum.getCardinality() << "): ";
    cin >> bSize;
    B.fillAutomaticly(bSize);

    cout << "Множества A и B сгенерированы!\n";
}

void UI::handleSetMenu() {
    bool back = false;
    while (!back) {
        printSetMenu();
        cout << "Выберите пункт: ";
        int choice;
        cin >> choice;

        switch (choice) {
            case 1: Multiset::Universum.print("Универсум:\n"); break;
            case 2: A.print("Множество A:\n"); break;
            case 3: B.print("Множество B:\n"); break;
            case 4: back = true; break;
            default: cout << "Неверный выбор!\n"; break;
        }
    }
}

void UI::handleOpMenu() {
    bool back = false;
    while (!back) {
        printOpMenu();
        cout << "Выберите операцию: ";
        int choice;
        cin >> choice;

        Multiset result;

        switch (choice) {
            case 1: result = A.Union(B); result.print("A ∪ B:\n"); break;
            case 2: result = A.Intersection(B); result.print("A ∩ B:\n"); break;
            case 3: result = A.Complement(); result.print("U \\ A:\n"); break;
            case 4: result = B.Complement(); result.print("U \\ B:\n"); break;
            case 5: result = A.Diff(B); result.print("A \\ B:\n"); break;
            case 6: result = B.Diff(A); result.print("B \\ A:\n"); break;
            case 7: result = A.SimmDiff(B); result.print("A ∆ B:\n"); break;
            case 8: result = A + B; result.print("A + B:\n"); break;
            case 9: result = A - B; result.print("A - B:\n"); break;
            case 10: result = B - A; result.print("B - A:\n"); break;
            case 11: result = A * B; result.print("A * B:\n"); break;
            case 12: result = A / B; result.print("A / B:\n"); break;
            case 13: result = B / A; result.print("B / A:\n"); break;
            case 14: back = true; break;
            default: cout << "Неверный выбор!\n"; break;
        }
    }
}