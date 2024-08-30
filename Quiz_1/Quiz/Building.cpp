#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

struct Amount {
    int value;
    Amount* next;

    Amount(int v) : value(v), next(nullptr) {}
};

struct Employee {
    std::string name;
    int totalSales;
    Amount* headAmount;
    Employee* next;

    Employee(std::string n) : name(n), totalSales(0), headAmount(nullptr), next(nullptr) {}
};

struct Floor {
    int number;
    int totalSalesFloor;
    Employee* headEmployee;
    Floor* prev;
    Floor* next;

    Floor(int num) : number(num), totalSalesFloor(0), headEmployee(nullptr), prev(nullptr), next(nullptr) {}
};

class Building {
public:
    Floor* head;

    Building() : head(nullptr) {}

    void insertFloor(int number) {
        Floor* newFloor = new Floor(number);

        if (!head) {
            head = newFloor;
        }
        else {
            Floor* temp = head;
            while (temp->next && temp->number < number) {
                temp = temp->next;
            }
            if (temp->number > number) {
                newFloor->next = temp;
                newFloor->prev = temp->prev;
                if (temp->prev) temp->prev->next = newFloor;
                temp->prev = newFloor;
                if (temp == head) head = newFloor;
            }
            else {
                temp->next = newFloor;
                newFloor->prev = temp;
            }
        }
    }

    Floor* findFloor(int number) {
        Floor* temp = head;
        while (temp) {
            if (temp->number == number) return temp;
            temp = temp->next;
        }
        return nullptr;
    }

    void insertEmployee(Building& building, int floorNumber, std::string employeeName, std::vector<int> sales) {
        Floor* floor = building.findFloor(floorNumber);
        if (!floor) return;

        Employee* newEmployee = new Employee(employeeName);

        int totalSales = 0;
        for (int sale : sales) {
            totalSales += sale;
        }
        newEmployee->totalSales = totalSales;

        if (!floor->headEmployee) {
            floor->headEmployee = newEmployee;
        }
        else {
            Employee* temp = floor->headEmployee;
            while (temp->next && temp->name < employeeName) {
                temp = temp->next;
            }
            if (temp->name > employeeName) {
                newEmployee->next = temp;
                if (temp == floor->headEmployee) floor->headEmployee = newEmployee;
            }
            else {
                temp->next = newEmployee;
            }
        }

        floor->totalSalesFloor += newEmployee->totalSales;
        for (int sale : sales) {
            insertSale(newEmployee, sale);
        }
    }

    void insertSale(Employee* employee, int value) {
        Amount* newAmount = new Amount(value);

        if (!employee->headAmount) {
            employee->headAmount = newAmount;
        }
        else {
            Amount* temp = employee->headAmount;
            while (temp->next && temp->value > value) {
                temp = temp->next;
            }
            newAmount->next = temp->next;
            temp->next = newAmount;
        }
    }

    void displayData(Building& building) {
        Floor* tempFloor = building.head;
        while (tempFloor) {
            std::cout << "Piso: " << tempFloor->number << " - Ventas Totales: " << tempFloor->totalSalesFloor << std::endl;
            Employee* tempEmployee = tempFloor->headEmployee;
            while (tempEmployee) {
                std::cout << "  Empleado: " << tempEmployee->name << " - Ventas Totales: " << tempEmployee->totalSales << std::endl;
                Amount* tempAmount = tempEmployee->headAmount;
                while (tempAmount) {
                    std::cout << "    Venta: " << tempAmount->value << std::endl;
                    tempAmount = tempAmount->next;
                }
                tempEmployee = tempEmployee->next;
            }
            tempFloor = tempFloor->next;
        }
    }

    void deleteFloor(Building& building, int floorNumber) {
        Floor* floorToDelete = building.findFloor(floorNumber);
        if (!floorToDelete) return;

        Floor* prevFloor = floorToDelete->prev;
        if (!prevFloor) return;

        Employee* employeeToMove = floorToDelete->headEmployee;
        while (employeeToMove) {
            Employee* nextEmployee = employeeToMove->next;
            insertEmployee(building, prevFloor->number, employeeToMove->name, {});
            prevFloor->totalSalesFloor += employeeToMove->totalSales;
            employeeToMove = nextEmployee;
        }

        if (floorToDelete->prev) floorToDelete->prev->next = floorToDelete->next;
        if (floorToDelete->next) floorToDelete->next->prev = floorToDelete->prev;

        if (floorToDelete == building.head) building.head = floorToDelete->next;

        delete floorToDelete;
    }

    void verifyTotals(Building& building) {
        Floor* tempFloor = building.head;
        while (tempFloor) {
            int sumFloor = 0;
            Employee* tempEmployee = tempFloor->headEmployee;
            while (tempEmployee) {
                int sumEmployee = 0;
                Amount* tempAmount = tempEmployee->headAmount;
                while (tempAmount) {
                    sumEmployee += tempAmount->value;
                    tempAmount = tempAmount->next;
                }
                if (sumEmployee != tempEmployee->totalSales) {
                    std::cout << "Desajuste en los totales para el empleado: " << tempEmployee->name << std::endl;
                }
                sumFloor += sumEmployee;
                tempEmployee = tempEmployee->next;
            }
            if (sumFloor != tempFloor->totalSalesFloor) {
                std::cout << "Desajuste en los totales para el piso: " << tempFloor->number << std::endl;
            }
            tempFloor = tempFloor->next;
        }
    }

    void menu(Building& building) {
        int choice;
        do {
            std::cout << "1. Insertar Piso\n";
            std::cout << "2. Insertar Empleado\n";
            std::cout << "3. Mostrar Datos\n";
            std::cout << "4. Verificar Totales\n";
            std::cout << "5. Eliminar Piso\n";
            std::cout << "6. Salir\n";
            std::cout << "Ingrese su opción: ";
            std::cin >> choice;

            switch (choice) {
            case 1: {
                int floorNumber;
                std::cout << "Ingrese el número del piso: ";
                std::cin >> floorNumber;
                building.insertFloor(floorNumber);
                break;
            }
            case 2: {
                int floorNumber;
                std::string employeeName;
                std::vector<int> sales;
                int numSales, sale;

                std::cout << "Ingrese el número del piso: ";
                std::cin >> floorNumber;
                std::cout << "Ingrese el nombre del empleado: ";
                std::cin >> employeeName;
                std::cout << "Ingrese el número de ventas: ";
                std::cin >> numSales;
                for (int i = 0; i < numSales; i++) {
                    std::cout << "Ingrese el monto de la venta: ";
                    std::cin >> sale;
                    sales.push_back(sale);
                }

                insertEmployee(building, floorNumber, employeeName, sales);
                break;
            }
            case 3:
                displayData(building);
                break;
            case 4:
                verifyTotals(building);
                break;
            case 5: {
                int floorNumber;
                std::cout << "Ingrese el número del piso a eliminar: ";
                std::cin >> floorNumber;
                deleteFloor(building, floorNumber);
                break;
            }
            case 6:
                std::cout << "Saliendo...\n";
                break;
            default:
                std::cout << "Opción inválida. Por favor, intente de nuevo.\n";
            }
        } while (choice != 6);
    }
};
