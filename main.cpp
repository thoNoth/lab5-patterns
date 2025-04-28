#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace std;

//паттерн "Стратегия" для совместимости
class CompatibilityStrategy {
public:
    virtual ~CompatibilityStrategy() = default;
    virtual void checkCompatibility(const string& device, const string& brand) = 0;
};

class DefaultCompatibility : public CompatibilityStrategy {
public:
    void checkCompatibility(const string& device, const string& brand) override {
        cout << "Checking compatibility with " << device << "..." << endl;
        cout << brand << " tablets are compatible with " << device << "." << endl;
    }
};

class AdvancedCompatibility : public CompatibilityStrategy {
public:
    void checkCompatibility(const string& device, const string& brand) override {
        cout << "Performing advanced compatibility check for " << device << "..." << endl;
        cout << brand << " tablets have enhanced compatibility with " << device << "." << endl;
    }
};

//паттерна "Шаблонный метод" для скидки
class GraphicTablet {
protected:
    string model;
    double price;
    string activeArea;
    int pressureLevels;
    unique_ptr<CompatibilityStrategy> compatibilityStrategy;

    virtual void applyBrandSpecificDiscount(double& discount) {
    }

    virtual void validateDiscount(double& discount) {
        // Max скидка 30%
        if (discount > 30) discount = 30;
    }

public:
    GraphicTablet(string m, double p, string a, int pl, unique_ptr<CompatibilityStrategy> cs)
        : model(m), price(p), activeArea(a), pressureLevels(pl), compatibilityStrategy(move(cs)) {}

    virtual ~GraphicTablet() = default;

    // Шаблонный метод для скидки
    void calculateDiscount(double discountPercent) {
        cout << "Calculating discount for " << model << ":" << endl;

        double finalDiscount = discountPercent;

        // Применяем специфичные скидки
        applyBrandSpecificDiscount(finalDiscount);

        // Проверка скидки
        validateDiscount(finalDiscount);

        // Применяем скидку
        double discountedPrice = price * (1 - finalDiscount / 100);

        cout << "Base price: $" << price << endl;
        cout << "Applied discount: " << finalDiscount << "%" << endl;
        cout << "Final price: $" << discountedPrice << endl;
    }

    void checkCompatibility(string device) {
        compatibilityStrategy->checkCompatibility(device, getBrand());
    }

    virtual string getBrand() const = 0;

    virtual void displayInfo() {
        cout << "Model: " << model << endl;
        cout << "Brand: " << getBrand() << endl;
        cout << "Price: $" << price << endl;
        cout << "Active Area: " << activeArea << endl;
        cout << "Pressure Levels: " << pressureLevels << endl;
    }

    void setCompatibilityStrategy(unique_ptr<CompatibilityStrategy> strategy) {
        compatibilityStrategy = move(strategy);
    }
};

// Класс планшета Wacom
class Wacom : public GraphicTablet {
private:
    bool touchScreen;
public:
    Wacom(string m, double p, string a, int pl, bool ts, unique_ptr<CompatibilityStrategy> cs)
        : GraphicTablet(m, p, a, pl, move(cs)), touchScreen(ts) {}

    string getBrand() const override { return "Wacom"; }

    void displayInfo() override {
        GraphicTablet::displayInfo();
        cout << "Touch screen: " << (touchScreen ? "Yes" : "No") << endl;
    }

protected:
    void applyBrandSpecificDiscount(double& discount) override {
        // Wacom дает дополнительную 5% скидку
        discount += 5;
        cout << "Applying Wacom brand bonus: +5% discount" << endl;
    }
};

// Класс планшета XP-Pen
class XP_Pen : public GraphicTablet {
private:
    int shortcutKeys;
public:
    XP_Pen(string m, double p, string a, int pl, int sk, unique_ptr<CompatibilityStrategy> cs)
        : GraphicTablet(m, p, a, pl, move(cs)), shortcutKeys(sk) {}

    string getBrand() const override { return "XP-Pen"; }

    void displayInfo() override {
        GraphicTablet::displayInfo();
        cout << "Shortcut Keys: " << shortcutKeys << endl;
    }

protected:
    void validateDiscount(double& discount) override {
        // XP-Pen ограничивает максимальную скидку 25%
        if (discount > 25) discount = 25;
        GraphicTablet::validateDiscount(discount);
    }
};

// Класс планшета Gaomon
class Gaomon : public GraphicTablet {
private:
    bool noBatteryPen;
public:
    Gaomon(string m, double p, string a, int pl, bool nbp, unique_ptr<CompatibilityStrategy> cs)
        : GraphicTablet(m, p, a, pl, move(cs)), noBatteryPen(nbp) {}

    string getBrand() const override { return "Gaomon"; }

    void displayInfo() override {
        GraphicTablet::displayInfo();
        cout << "No Battery in pen: " << (noBatteryPen ? "Yes" : "No") << endl;
    }
};

// Демонстрация работы паттернов
int main() {
    vector<unique_ptr<GraphicTablet>> tablets;

    tablets.push_back(make_unique<Wacom>("Wacom Intuos M", 150.99, "257mm", 4096, true,
                                        make_unique<DefaultCompatibility>()));
    tablets.push_back(make_unique<XP_Pen>("XP-Pen Deco 01 V2", 60.12, "302mm", 8192, 8,
                                        make_unique<AdvancedCompatibility>()));
    tablets.push_back(make_unique<Gaomon>("GAOMON M10K Pro", 47.40, "247mm", 8192, true,
                                        make_unique<DefaultCompatibility>()));

    // Демонстрация работы шаблонного метода
    cout << "<=== Discount Calculation  ===>" << endl;
    for (auto& tablet : tablets) {
        tablet->displayInfo();
        tablet->calculateDiscount(15);
        cout << "--------------------------------------" << endl;
    }

    // Демонстрация работы стратегии
    cout << "\n<=== Compatibility Check Demo ===>" << endl;
    for (auto& tablet : tablets) {
        tablet->checkCompatibility("PC");
        cout << "--------------------------------------" << endl;
    }

    // Демонстрация смены стратегии во время выполнения
    cout << "\n<=== Changing Strategy at Runtime ===>" << endl;
    tablets[0]->setCompatibilityStrategy(make_unique<AdvancedCompatibility>());
    tablets[0]->checkCompatibility("Mac");

    return 0;
}
