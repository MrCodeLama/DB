#define OPEN_MODE ios::out | ios::binary | ios::in
#define LONG_TEXT 100
#define SHORT_TEXT 50
#define MASTER_FILEPATH "Dealers.dat"
#define SLAVE_FILEPATH "Cars.dat"
#define OUTPUT_CARS_FILEPATH "outputCars.txt"
#define OUTPUT_DEALERS_FILEPATH "outputDealers.txt"
#define TEMP_FILEPATH "temp.dat"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

//slave
struct Car
{
    char _isDeleted;
    char Model[SHORT_TEXT];
    int CarID;
    int HP;
    int MaxSpeed;
    int Weigth;
    int NumberOfNextCarInFile;
};

//master
struct Dealer
{
    char _isDeleted;
    char Name[SHORT_TEXT];
    char Adress[LONG_TEXT];
    int DealerID;
    int FirstCarNumberInFile;
};

Car CarReturn;
Dealer DealerReturn;

int NumberOfElementsInFile(bool _isSlave)
{
    ifstream readfile((_isSlave ? SLAVE_FILEPATH : MASTER_FILEPATH), OPEN_MODE);

    readfile.seekg(0, readfile.end);
    size_t rcount = readfile.tellg() / (_isSlave ? sizeof(Car) : sizeof(Dealer));

    readfile.close();
    if (!readfile.good())
        cout << "Error NumberOfElementsInFile";

    return rcount;
}

int NumberOfCarsInDealer(int DealerID)
{
    int NumberOfDealers = NumberOfElementsInFile(false);
    int i = 0;
    Dealer tempDealer;
    Car tempCar;
    bool checker = false;
    int counter = 0;
    int currentCarNumber = 0;

    ifstream ReadMaster1(MASTER_FILEPATH, OPEN_MODE);

    for (; i < NumberOfDealers; i++)
    {
        ReadMaster1.seekg(i * sizeof(Dealer), ReadMaster1.beg);
        ReadMaster1.read((char*)&tempDealer, sizeof(Dealer));
        if (tempDealer.DealerID == DealerID)
        {
            currentCarNumber = tempDealer.FirstCarNumberInFile;
            checker = true;
            break;
        }
    }

    ReadMaster1.close();
    if (!ReadMaster1.good())
        cout << "Error NumberOfCarsInDealer M1";

    if (checker)
    {
        ifstream ReadSlave1(SLAVE_FILEPATH, OPEN_MODE);

        while (currentCarNumber != -1)
        {
            ReadSlave1.seekg(currentCarNumber * sizeof(Car), ReadSlave1.beg);
            ReadSlave1.read((char*)&tempCar, sizeof(Car));

            currentCarNumber = tempCar.NumberOfNextCarInFile;
            counter++;
        }

        ReadSlave1.close();
        if (!ReadSlave1.good())
            cout << "Error NumberOfCarsInDealer S1";
    }
    else
        cout << "Error NumberOfCarsInDealer. No dealer found";

    return counter;
}

void GetAllDealersInfo()
{
    ofstream OutputFile1;
    OutputFile1.open(OUTPUT_DEALERS_FILEPATH, std::ofstream::out | std::ofstream::trunc);
    OutputFile1.close();
    if (!OutputFile1.good())
        cout << "Error GetAllDealersInfo O1";

    string Res;
    int i = 0;
    int NumberOfDealers = NumberOfElementsInFile(false);
    Dealer tempDealer;

    ofstream OutputFile2(OUTPUT_DEALERS_FILEPATH);

    for (; i < NumberOfDealers; i++)
    {
        ifstream ReadMaster1(MASTER_FILEPATH, OPEN_MODE);
        ReadMaster1.seekg(i * sizeof(Dealer), ReadMaster1.beg);
        ReadMaster1.read((char*)&tempDealer, sizeof(Dealer));

        Res += "Name: " + string(tempDealer.Name);
        Res += "\nAdress: " + string(tempDealer.Adress);
        Res += "\nDealerID: " + to_string(tempDealer.DealerID);
        Res += "\nFirstCarNumberInFile: " + to_string(tempDealer.FirstCarNumberInFile);
        Res += "\n\n";

        ReadMaster1.close();
        if (!ReadMaster1.good())
            cout << "Error GetAllDealersInfo M1";

        OutputFile2 << Res;

        Res.clear();
    }

    OutputFile2.close();
    if (!OutputFile2.good())
        cout << "Error GetAllDealersInfo O2";
}

void GetAllCarsInfo()
{
    ofstream OutputFile1;
    OutputFile1.open(OUTPUT_CARS_FILEPATH, std::ofstream::out | std::ofstream::trunc);
    OutputFile1.close();
    if (!OutputFile1.good())
        cout << "Error GetAllCarsInfo O1";

    string Res;
    int i = 0;
    int NumberOfCars = NumberOfElementsInFile(true);
    Car tempCar;

    ofstream OutputFile2(OUTPUT_CARS_FILEPATH);

    for (; i < NumberOfCars; i++)
    {
        ifstream ReadSlave1(SLAVE_FILEPATH, OPEN_MODE);

        ReadSlave1.seekg(i * sizeof(Car), ReadSlave1.beg);
        ReadSlave1.read((char*)&tempCar, sizeof(Car));

        Res += "Model: " + string(tempCar.Model);
        Res += "\nCar ID: " + to_string(tempCar.CarID);
        Res += "\nHP: " + to_string(tempCar.HP);
        Res += "\nMaxSpeed: " + to_string(tempCar.MaxSpeed);
        Res += "\nWeigth: " + to_string(tempCar.Weigth);
        Res += "\nNumberOfNextCarInFile: " + to_string(tempCar.NumberOfNextCarInFile);
        Res += "\n\n";
        ReadSlave1.close();
        if (!ReadSlave1.good())
            cout << "Error GetAllCarsInfo S1";

        OutputFile2 << Res;

        Res.clear();
    }

    OutputFile2.close();
    if (!OutputFile2.good())
        cout << "Error GetAllCarsInfo O2";
}

Car GetCar(int CarID)
{
    int NumerOfCars = NumberOfElementsInFile(true);
    int i = 0;
    Car tempCar = {};

    ifstream ReadSlave1(SLAVE_FILEPATH, OPEN_MODE);

    for (; i < NumerOfCars; i++)
    {
        ReadSlave1.seekg(i * sizeof(Car), ReadSlave1.beg);
        ReadSlave1.read((char*)&tempCar, sizeof(Car));
        if (tempCar.CarID == CarID)
            break;
    }

    ReadSlave1.close();
    if (!ReadSlave1.good())
        cout << "Error GetCar";

    return tempCar;
}

Dealer GetDealer(int DealerID)
{
    int NumberOfDealers = NumberOfElementsInFile(false);
    int i = 0;
    Dealer tempDealer = {};

    ifstream ReadMaster1(MASTER_FILEPATH, OPEN_MODE);

    for (; i < NumberOfDealers; i++)
    {
        ReadMaster1.seekg(i * sizeof(Dealer), ReadMaster1.beg);
        ReadMaster1.read((char*)&tempDealer, sizeof(Dealer));
        if (tempDealer.DealerID == DealerID)
            break;
    }

    ReadMaster1.close();
    if (!ReadMaster1.good())
        cout << "Error GetDealer";

    return tempDealer;
}

void UpdateCar(int CarID)
{
    int NumberOfCars = NumberOfElementsInFile(true);
    int i = 0;
    Car tempCar;
    bool _isCarExists = false;

    ifstream ReadSlave1(SLAVE_FILEPATH, OPEN_MODE);

    for (; i < NumberOfCars; i++)
    {
        ReadSlave1.seekg(i * sizeof(Car), ReadSlave1.beg);
        ReadSlave1.read((char*)&tempCar, sizeof(Car));
        if (tempCar.CarID == CarID)
        {
            _isCarExists = true;
            break;
        }
    }

    ReadSlave1.close();
    if (!ReadSlave1.good())
        cout << "Error UpdateCar S1";

    if (_isCarExists)
    {
        cout << "Enter Value to change: 0 - CarID 1 - Model 2 - HP 3 - MaxSpeed 4 - Weigth\n";
        int n;
        cin >> n;
        cout << "Enter new value:\n";
        switch (n)
        {
        case 0:
            cin >> tempCar.CarID;
            break;
        case 1:
            cin >> tempCar.Model;
            break;
        case 2:
            cin >> tempCar.HP;
            break;
        case 3:
            cin >> tempCar.MaxSpeed;
            break;
        case 4:
            cin >> tempCar.Weigth;
            break;
        default:
            cout << "Error UpdateCar not in range 0-4";
            break;
        }

        ofstream WriteSlave2(SLAVE_FILEPATH, OPEN_MODE);
        WriteSlave2.seekp(i * sizeof(Car), WriteSlave2.beg);
        WriteSlave2.write((char*)&tempCar, sizeof(Car));

        WriteSlave2.close();
        if (!WriteSlave2.good())
            cout << "Error UpdateCar S2";
    }
    else
        cout << "Error UpdateCar No car\n";
}

void UpdateDealer(int DealerID)
{
    int NumberOfDealers = NumberOfElementsInFile(false);
    int i = 0;
    Dealer tempDealer;
    bool _isDealerExists = false;
    int TaskNumber;
    int temp;

    ifstream ReadMaster1(MASTER_FILEPATH, OPEN_MODE);

    for (; i < NumberOfDealers; i++)
    {
        ReadMaster1.seekg(i * sizeof(Dealer), ReadMaster1.beg);
        ReadMaster1.read((char*)&tempDealer, sizeof(Dealer));
        if (tempDealer.DealerID == DealerID)
        {
            _isDealerExists = true;
            break;
        }
    }

    ReadMaster1.close();
    if (!ReadMaster1.good())
        cout << "Error UpdateDealer M1";

    if (_isDealerExists)
    {
        cout << "Enter Value to change: 0 - ID 1 - Name 2 - Adress\n";

        cin >> TaskNumber;
        cout << "Enter new value:\n";
        switch (TaskNumber)
        {
        case 0:
            cin >> temp;
            tempDealer.DealerID = temp;
            break;
        case 1:
            cin >> tempDealer.Name;
            break;
        case 2:
            cin >> tempDealer.Adress;
            break;
        default:
            cout << "Error UpdateDealer Not in range 0-2";
            break;
        }

        ofstream WriteMaster2(MASTER_FILEPATH, OPEN_MODE);
        WriteMaster2.seekp(i * sizeof(Dealer), WriteMaster2.beg);
        WriteMaster2.write((char*)&tempDealer, sizeof(Dealer));

        WriteMaster2.close();
        if (!WriteMaster2.good())
            cout << "Error UpdateDealer M2";
    }
    else {
        cout << "Error UpdateDealer Dealer doesnt exists\n";
    }
}

void AddCar(int DealerID, Car NewCar)
{
    int NumberOfDealers = NumberOfElementsInFile(false);
    int NumberOfCars = NumberOfElementsInFile(true);
    bool checker = false;
    int i = 0;
    Dealer tempDealer;
    Car tempCar;
    int currentCarNumber = -1;
    int previousCarNumber = -1;

    


    ifstream ReadMaster1(MASTER_FILEPATH, OPEN_MODE);

    for (; i < NumberOfDealers; i++)
    {
        ReadMaster1.seekg(i * sizeof(Dealer), ReadMaster1.beg);
        ReadMaster1.read((char*)&tempDealer, sizeof(Dealer));
        if (tempDealer.DealerID == DealerID)
        {
            checker = true;
            break;
        }
    }

    ReadMaster1.close();
    if (!ReadMaster1.good())
        cout << "Error AddCar M1";

    if (checker)
    {
        currentCarNumber = tempDealer.FirstCarNumberInFile;
        checker = (currentCarNumber == -1);

        if (checker)
        {
            tempDealer.FirstCarNumberInFile = NumberOfCars;

            ofstream WriteSlave2(SLAVE_FILEPATH, OPEN_MODE);

            WriteSlave2.seekp(NumberOfCars * sizeof(Car), WriteSlave2.beg);
            WriteSlave2.write((char*)&NewCar, sizeof(Car));

            WriteSlave2.close();
            if (!WriteSlave2.good())
                cout << "Error AddCar S2";

            ofstream WriteMaster2(MASTER_FILEPATH, OPEN_MODE);

            WriteMaster2.seekp(i * sizeof(Dealer), WriteMaster2.beg);
            WriteMaster2.write((char*)&tempDealer, sizeof(Dealer));

            WriteMaster2.close();
            if (!WriteMaster2.good())
                cout << "Error AddCar M2";
        }
        else
        {
            ifstream ReadSlave1(SLAVE_FILEPATH, OPEN_MODE);

            while (currentCarNumber != -1)
            {
                ReadSlave1.seekg(currentCarNumber * sizeof(Car), ReadSlave1.beg);
                ReadSlave1.read((char*)&tempCar, sizeof(Car));

                previousCarNumber = currentCarNumber;
                currentCarNumber = tempCar.NumberOfNextCarInFile;

            }

            ReadSlave1.close();
            if (!ReadSlave1.good())
                cout << "Error AddCar S1";

            tempCar.NumberOfNextCarInFile = NumberOfCars;

            ofstream WriteSlave3(SLAVE_FILEPATH, OPEN_MODE);
            WriteSlave3.seekp(previousCarNumber * sizeof(Car), WriteSlave3.beg);
            WriteSlave3.write((char*)&tempCar, sizeof(Car));

            WriteSlave3.seekp(NumberOfCars * sizeof(Car), WriteSlave3.beg);
            WriteSlave3.write((char*)&NewCar, sizeof(Car));

            WriteSlave3.close();
            if (!WriteSlave3.good())
                cout << "Error AddCar S3";
        }
    }
}

void AddDealer(Dealer NewDealer)
{
    ofstream WriteMaster1(MASTER_FILEPATH, OPEN_MODE);

    WriteMaster1.seekp(0, WriteMaster1.end);
    WriteMaster1.write((char*)&NewDealer, sizeof(Dealer));

    WriteMaster1.close();
    if (!WriteMaster1.good())
        cout << "Error AddDealer M1";
}

void makeFiles(bool _removeOldFiles) {
    if (_removeOldFiles)
    {
        remove(MASTER_FILEPATH);
        remove(SLAVE_FILEPATH);
        remove(OUTPUT_CARS_FILEPATH);
        remove(OUTPUT_DEALERS_FILEPATH);
    }

    ifstream master(MASTER_FILEPATH, OPEN_MODE);
    bool isopen = master.is_open();
    master.close();

    if (!isopen) {
        ofstream master(MASTER_FILEPATH);
        master.close();
    }

    ifstream slave(SLAVE_FILEPATH, OPEN_MODE);
    isopen = slave.is_open();
    slave.close();

    if (!isopen) {
        ofstream slave(SLAVE_FILEPATH);
        slave.close();
    }

    ifstream outputCars(OUTPUT_CARS_FILEPATH, OPEN_MODE);
    isopen = outputCars.is_open();
    outputCars.close();

    if (!isopen) {
        ofstream outputCars(OUTPUT_CARS_FILEPATH);
        outputCars.close();
    }

    ifstream outputDealers(OUTPUT_DEALERS_FILEPATH, OPEN_MODE);
    isopen = outputDealers.is_open();
    outputDealers.close();

    if (!isopen) {
        ofstream outputDealers(OUTPUT_DEALERS_FILEPATH);
        outputDealers.close();
    }

}

Car CreateCar()
{
    

    Car tempCar;

    cout << "Enter car model:\n";
    cin >> tempCar.Model;
    cout << "Enter car id:\n";
    cin >> tempCar.CarID;
    cout << "Enter car HP:\n";
    cin >> tempCar.HP;
    cout << "Enter car MaxSpeed:\n";
    cin >> tempCar.MaxSpeed;
    cout << "Enter car Weigth:\n";
    cin >> tempCar.Weigth;
    tempCar.NumberOfNextCarInFile = -1;

    return tempCar;
}

Dealer CreateDealer()
{
    Dealer tempDealer;

    cout << "Enter dealer Name:\n";
    cin >> tempDealer.Name;
    cout << "Enter dealer Adress:\n";
    cin >> tempDealer.Adress;
    cout << "Enter dealer ID:\n";
    cin >> tempDealer.DealerID;
    cout << "\n\n";
    tempDealer.FirstCarNumberInFile = -1;

    return tempDealer;
}

void DeleteCar(int CarID)
{
    int numberOfCars = NumberOfElementsInFile(true);
    int numberOfDealers = NumberOfElementsInFile(false);
    int i = 0;
    Car car, prevCar;
    Dealer dealerOfCarToDel;
    bool carExists = false;
    bool isCarFoundInDealers = false;
    bool prevCarExists = false;

    ifstream rs1(SLAVE_FILEPATH, OPEN_MODE);

    for (; i < numberOfCars; i++)
    {
        rs1.seekg(i * sizeof(Car), rs1.beg);
        rs1.read((char*)&car, sizeof(Car));

        if (car.CarID == CarID) {
            carExists = true;
            break;
        }
    }

    if (!carExists) {
        cout << "Error Car doesn't exist\n";
        return;
    }

    ifstream rm1(MASTER_FILEPATH, OPEN_MODE);

    for (int j = 0; j < numberOfDealers; j++)
    {
        rm1.seekg(j * sizeof(Dealer), rm1.beg);
        rm1.read((char*)&dealerOfCarToDel, sizeof(Dealer));

        if (dealerOfCarToDel.FirstCarNumberInFile == -1)
            continue;

        if (dealerOfCarToDel.FirstCarNumberInFile == i) {
            dealerOfCarToDel.FirstCarNumberInFile = (car.NumberOfNextCarInFile == -1)
                ? -1 : car.NumberOfNextCarInFile;

            ofstream wm(MASTER_FILEPATH, OPEN_MODE);

            wm.seekp(j * sizeof(Dealer), wm.beg);
            wm.write((char*)&dealerOfCarToDel, sizeof(Dealer));

            wm.close();
            if (!wm.good()) {
                cout << "Error Edit Master";
                return;
            }

            isCarFoundInDealers = true;
            break;
        }

        Car tempCar;
        rs1.seekg(dealerOfCarToDel.FirstCarNumberInFile * sizeof(Car), rs1.beg);
        rs1.read((char*)&prevCar, sizeof(Car));

        if (prevCar.NumberOfNextCarInFile == -1)
            continue;

        do {
            rs1.seekg(prevCar.NumberOfNextCarInFile * sizeof(Car), rs1.beg);
            rs1.read((char*)&tempCar, sizeof(Car));

            if (prevCar.NumberOfNextCarInFile != i) {
                prevCar = tempCar;
                continue;
            }

            prevCar.NumberOfNextCarInFile = tempCar.NumberOfNextCarInFile;

            prevCarExists = true;
            isCarFoundInDealers = true;
            break;
        } while (tempCar.NumberOfNextCarInFile != -1);

        if (isCarFoundInDealers)
            break;
    }

    if (!isCarFoundInDealers) {
        cout << "Error isCarFoundInDealers=false";
        return;
    }

    rs1.close();
    if (!rs1.good()) {
        cout << "Error DeleteDealer M1";
        return;
    }

    rm1.close();
    if (!rm1.good()) {
        cout << "Error rm.close()";
        return;
    }


    // rewriting indexes in SLAVE

    ifstream tempSlave(TEMP_FILEPATH, OPEN_MODE);
    bool isopenSlave = tempSlave.is_open();
    tempSlave.close();

    if (!isopenSlave) {
        ofstream tempFile(TEMP_FILEPATH);
        tempFile.close();
    }

    Car tempCar;
    ifstream rs2(SLAVE_FILEPATH, OPEN_MODE);

    for (int j = 0; j < numberOfCars; j++)
    {
        if (j == i) {
            continue;
        }

        rs2.seekg(j * sizeof(Car), rs2.beg);
        rs2.read((char*)&tempCar, sizeof(Car));

        if (prevCarExists && (prevCar.CarID == tempCar.CarID))
            tempCar.NumberOfNextCarInFile = prevCar.NumberOfNextCarInFile;

        int num = tempCar.NumberOfNextCarInFile;
        tempCar.NumberOfNextCarInFile = (num > i) ? (num - 1) : num;
        int newNumberInFile = (j > i) ? (j - 1) : j;

        ofstream wtemp(TEMP_FILEPATH, OPEN_MODE);

        wtemp.seekp(newNumberInFile * sizeof(Car), wtemp.beg);
        wtemp.write((char*)&tempCar, sizeof(Car));

        wtemp.close();
        if (!wtemp.good()) {
            cout << "Error DeleteDealer T1";
            return;
        }
    }

    rs2.close();
    if (!rs2.good()) {
        cout << "Error DeleteDealer M2";
        return;
    }

    remove(SLAVE_FILEPATH);
    rename(TEMP_FILEPATH, SLAVE_FILEPATH);


    // rewriting indexes in MASTER

    ifstream tempMaster(TEMP_FILEPATH, OPEN_MODE);
    bool isopenMaster = tempMaster.is_open();
    tempMaster.close();

    if (!isopenMaster) {
        ofstream tempMaster(TEMP_FILEPATH);
        tempMaster.close();
    }

    Dealer tempDealer;
    ifstream rm2(MASTER_FILEPATH, OPEN_MODE);

    for (int j = 0; j < numberOfDealers; j++)
    {
        rm2.seekg(j * sizeof(Dealer), rm2.beg);
        rm2.read((char*)&tempDealer, sizeof(Dealer));

        int num = tempDealer.FirstCarNumberInFile;
        tempDealer.FirstCarNumberInFile = (num > i) ? (num - 1) : num;

        ofstream wtemp(TEMP_FILEPATH, OPEN_MODE);

        wtemp.seekp(j * sizeof(Dealer), wtemp.beg);
        wtemp.write((char*)&tempDealer, sizeof(Dealer));

        wtemp.close();
        if (!wtemp.good()) {
            cout << "Error";
            return;
        }
    }

    rm2.close();
    if (!rm2.good()) {
        cout << "Error DeleteDealer M2";
        return;
    }

    remove(MASTER_FILEPATH);
    rename(TEMP_FILEPATH, MASTER_FILEPATH);
}

void DeleteCarsFromDealer(int DealerID)
{
    int numberOfCars = NumberOfElementsInFile(true);
    int numberOfDealers = NumberOfElementsInFile(false);
    int i = 0;
    Dealer dealer;
    bool dealerExists = false;


    // check if the dealer exists

    ifstream rm1(MASTER_FILEPATH, OPEN_MODE);

    for (; i < numberOfCars; i++)
    {
        rm1.seekg(i * sizeof(Dealer), rm1.beg);
        rm1.read((char*)&dealer, sizeof(Dealer));

        if (dealer.DealerID == DealerID) {
            dealerExists = true;
            break;
        }
    }

    if (!dealerExists) {
        cout << "Error Dealer doesn't exist\n";
        return;
    }

    rm1.close();
    if (!rm1.good()) {
        cout << "Error DeleteCarsFromDealer rm.close()";
        return;
    }


    // find the IDs of the dealer's cars

    if (dealer.FirstCarNumberInFile == -1) {
        cout << "Dealer has no cars!";
        return;
    }

    Car tempCar;
    vector<int> carIDs;
    int indx = dealer.FirstCarNumberInFile;

    ifstream rs1(SLAVE_FILEPATH, OPEN_MODE);

    while (true) {
        rs1.seekg(indx * sizeof(Car), rs1.beg);
        rs1.read((char*)&tempCar, sizeof(Car));

        carIDs.push_back(tempCar.CarID);

        if (tempCar.NumberOfNextCarInFile == -1)
            break;

        indx = tempCar.NumberOfNextCarInFile;
    }

    rs1.close();
    if (!rs1.good()) {
        cout << "Error DeleteCarsFromDealer rs1.good()";
        return;
    }


    // delete all cars of the dealer

    for (int& id : carIDs)
        DeleteCar(id);
}

void DeleteDealer(int DealerID)
{
    int NumberOfDealers = NumberOfElementsInFile(false);
    int i = 0;
    Dealer tempDealer;
    bool _isDealerExists = false;
    int counter = 0;

    ifstream ReadMaster1(MASTER_FILEPATH, OPEN_MODE);

    for (; i < NumberOfDealers; i++)
    {
        ReadMaster1.seekg(i * sizeof(Dealer), ReadMaster1.beg);
        ReadMaster1.read((char*)&tempDealer, sizeof(Dealer));
        if (tempDealer.DealerID == DealerID)
        {
            _isDealerExists = true;
            break;
        }
    }

    ReadMaster1.close();
    if (!ReadMaster1.good())
        cout << "Error DeleteDealer M1";

    if (_isDealerExists)
    {
        if (tempDealer.FirstCarNumberInFile != -1)
        {
            DeleteCarsFromDealer(tempDealer.DealerID);
        }

        ifstream tempFile(TEMP_FILEPATH, OPEN_MODE);
        bool isopen = tempFile.is_open();
        tempFile.close();

        if (!isopen) {
            ofstream tempFile(TEMP_FILEPATH);
            tempFile.close();
        }

        for (int j = 0; j < NumberOfDealers; j++)
        {
            if (j == i)
                continue;

            ifstream ReadMaster2(MASTER_FILEPATH, OPEN_MODE);

            ReadMaster2.seekg(j * sizeof(Dealer), ReadMaster2.beg);
            ReadMaster2.read((char*)&tempDealer, sizeof(Dealer));

            ReadMaster2.close();
            if (!ReadMaster2.good())
                cout << "Error DeleteDealer M2";

            ofstream WriteTemp1(TEMP_FILEPATH, OPEN_MODE);

            WriteTemp1.seekp(counter++ * sizeof(Dealer), WriteTemp1.beg);
            WriteTemp1.write((char*)&tempDealer, sizeof(Dealer));

            WriteTemp1.close();
            if (!WriteTemp1.good())
                cout << "Error DeleteDealer T1";
        }

        remove(MASTER_FILEPATH);
        rename(TEMP_FILEPATH, MASTER_FILEPATH);

    }
    else {
        cout << "Error UpdateDealer Dealer doesnt exists\n";
    }
}

void printAllDealers()
{
    cout << "\nList of delaers\nName - id\n";
    int NumberOfDealers = NumberOfElementsInFile(false);
    Dealer tempDealer;

    ifstream ReadMaster1(MASTER_FILEPATH, OPEN_MODE);
    cout << "\n";

    for (int i = 0; i < NumberOfDealers; i++)
    {
        ReadMaster1.seekg(i * sizeof(Dealer), ReadMaster1.beg);
        ReadMaster1.read((char*)&tempDealer, sizeof(Dealer));
        cout << tempDealer.Name << " - " << tempDealer.DealerID << "\n";
    }
    cout << "\n";

    ReadMaster1.close();
    if (!ReadMaster1.good())
        cout << "Error AddCar M1";
}

void printAllCars()
{
    cout << "\nList of cars\nModel - id\n";
    int NumberOfDealers = NumberOfElementsInFile(true);
    Car tempCar;

    ifstream ReadMaster1(SLAVE_FILEPATH, OPEN_MODE);
    cout << "\n";

    for (int i = 0; i < NumberOfDealers; i++)
    {
        ReadMaster1.seekg(i * sizeof(Car), ReadMaster1.beg);
        ReadMaster1.read((char*)&tempCar, sizeof(Car));
        cout << tempCar.Model << " - " << tempCar.CarID << "\n";
    }
    cout << "\n";

    ReadMaster1.close();
    if (!ReadMaster1.good())
        cout << "Error AddCar M1";
}

void Menu()
{
    int taskNumber = 0;
    int carId;
    int dealerId;
    while (true)
    {
        cout <<
            "Exit - 0\n" <<
            "Get car - 1\n" <<
            "Get dealer - 2\n" <<
            "Delete car - 3\n" <<
            "Delete dealer - 4\n" <<
            "Update car - 5\n" <<
            "Update dealer - 6\n" <<
            "Add car - 7\n" <<
            "Add dealer - 8\n" <<
            "Calculate all dealers - 9\n" <<
            "Calculate cars in dealer - 10\n" <<
            "Calculate all cars - 11\n" <<
            "Print dealers info - 12\n" <<
            "Print cars info - 13\n";
        cin >> taskNumber;

        switch (taskNumber)
        {
        case 0:
            exit(0);
            break;
        case 1:
            printAllCars();
            cout << "Enter car id\n";
            cin >> carId;
            CarReturn = GetCar(carId);
            break;
        case 2:
            printAllDealers();
            cout << "Enter dealer id\n";
            cin >> dealerId;
            DealerReturn = GetDealer(dealerId);
            break;
        case 3:
            printAllCars();
            cout << "Enter car id\n";
            cin >> carId;
            DeleteCar(carId);
            break;
        case 4:
            printAllDealers();
            cout << "Enter dealer id\n";
            cin >> dealerId;
            DeleteDealer(dealerId);
            break;
        case 5:
            printAllCars();
            cout << "Enter car id\n";
            cin >> carId;
            UpdateCar(carId);
            break;
        case 6:
            printAllDealers();
            cout << "Enter dealer id\n";
            cin >> dealerId;
            UpdateDealer(dealerId);
            break;
        case 7:
            printAllCars();
            printAllDealers();
            cout << "Enter dealer id\n";
            cin >> dealerId;
            AddCar(dealerId, CreateCar());
            break;
        case 8:
            printAllDealers();
            AddDealer(CreateDealer());
            break;
        case 9:
            cout << NumberOfElementsInFile(false) << "\n";
            break;
        case 10:
            cout << "Enter dealer id\n";
            cin >> dealerId;
            cout << NumberOfCarsInDealer(dealerId) << "\n";
            break;
        case 11:
            cout << NumberOfElementsInFile(true) << "\n";
            break;
        case 12:
            GetAllDealersInfo();
            cout << "Check " << OUTPUT_DEALERS_FILEPATH << " file\n";
            break;
        case 13:
            GetAllCarsInfo();
            cout << "Check " << OUTPUT_CARS_FILEPATH << " file\n";
            break;
        default:
            cout << "Error executeTasks not in range 1-13\n";
            break;
        }
    }
}

void test1()
{
    makeFiles(true);
    const int SIZE = 5;
    Dealer old_dealers[SIZE] = {
        {
            {'B', 'M', 'W'},
            {'K', 'y', 'i', 'v'},
            1,
            -1
        },
        {
            {'B', 'e', 'n', 'z'},
            {'L', 'v', 'i', 'v'},
            2,
            -1
        },
        {
            {'A', 'u', 'd', 'i'},
            {'Z', 'h', 'i', 't', 'o', 'm', 'i', 'r'},
            3,
            -1
        },
        {
            {'A', 'u', 'd', 'i'},
            {'K', 'a', 'n', 'i', 'v'},
            4,
            -1
        },
        {
            {'B', 'M', 'W'},
            {'L', 'v', 'i', 'v'},
            5,
            -1
        },
    };

    for (int i = 0; i < SIZE; i++)
    {
        AddDealer(old_dealers[i]);
    }

    Car M3 =
    {
        {'M', '3'},
        1,
        345,
        300,
        1500,
        -1
    };

    Car M2 =
    {
        {'M', '2'},
        2,
        300,
        230,
        1300,
        -1
    };

    Car G63 =
    {
        {'A', 'M', 'G'},
        3,
        200,
        210,
        3000,
        -1
    };

    Car A4 =
    {
        {'A', '4'},
        4,
        310,
        210,
        2400,
        -1
    };

    Car etron =
    {
        {'E', '-', 't', 'r', 'o', 'n'},
        5,
        260,
        210,
        2400,
        -1
    };

    Car Rs7 =
    {
        {'R', 's', '-', '7'},
        6,
        380,
        210,
        2600,
        -1
    };

    AddCar(1, M3);
    AddCar(1, M2);
    AddCar(2, G63);
    AddCar(4, A4);
    AddCar(4, etron);
    AddCar(4, Rs7);

    GetAllCarsInfo();
    GetAllDealersInfo();
}

void test2()
{
    makeFiles(true);
    const int SIZE = 5;
    Dealer old_dealers[SIZE] = {
        {
            {'B', 'M', 'W'},
            {'K', 'y', 'i', 'v'},
            1,
            -1
        },
        {
            {'B', 'e', 'n', 'z'},
            {'L', 'v', 'i', 'v'},
            2,
            -1
        },
        {
            {'A', 'u', 'd', 'i'},
            {'Z', 'h', 'i', 't', 'o', 'm', 'i', 'r'},
            3,
            -1
        },
        {
            {'A', 'u', 'd', 'i'},
            {'K', 'a', 'n', 'i', 'v'},
            4,
            -1
        },
        {
            {'B', 'M', 'W'},
            {'L', 'v', 'i', 'v'},
            5,
            -1
        },
    };

    for (int i = 0; i < SIZE; i++)
    {
        AddDealer(old_dealers[i]);
    }

    Car M3 =
    {
        {'M', '3'},
        1,
        345,
        300,
        1500,
        -1
    };

    Car M2 =
    {
        {'M', '2'},
        2,
        300,
        230,
        1300,
        -1
    };

    Car G63 =
    {
        {'A', 'M', 'G'},
        3,
        200,
        210,
        3000,
        -1
    };

    Car A4 =
    {
        {'A', '4'},
        4,
        310,
        210,
        2400,
        -1
    };

    Car etron =
    {
        {'E', '-', 't', 'r', 'o', 'n'},
        5,
        260,
        210,
        2400,
        -1
    };

    Car Rs7 =
    {
        {'R', 's', '-', '7'},
        6,
        380,
        210,
        2600,
        -1
    };

    AddCar(1, M3);
    AddCar(1, M2);
    AddCar(2, G63);
    AddCar(4, A4);
    AddCar(4, etron);
    AddCar(4, Rs7);

    DeleteDealer(1);
    DeleteCar(5);

    GetAllCarsInfo();
    GetAllDealersInfo();
}

void test3()
{
    makeFiles(true);
    const int SIZE = 5;
    Dealer old_dealers[SIZE] = {
        {
            {'B', 'M', 'W'},
            {'K', 'y', 'i', 'v'},
            1,
            -1
        },
        {
            {'B', 'e', 'n', 'z'},
            {'L', 'v', 'i', 'v'},
            2,
            -1
        },
        {
            {'A', 'u', 'd', 'i'},
            {'Z', 'h', 'i', 't', 'o', 'm', 'i', 'r'},
            3,
            -1
        },
        {
            {'A', 'u', 'd', 'i'},
            {'K', 'a', 'n', 'i', 'v'},
            4,
            -1
        },
        {
            {'B', 'M', 'W'},
            {'L', 'v', 'i', 'v'},
            5,
            -1
        },
    };

    for (int i = 0; i < SIZE; i++)
    {
        AddDealer(old_dealers[i]);
    }

    Car M3 =
    {
        {'M', '3'},
        1,
        345,
        300,
        1500,
        -1
    };

    Car M2 =
    {
        {'M', '2'},
        2,
        300,
        230,
        1300,
        -1
    };

    Car G63 =
    {
        {'A', 'M', 'G'},
        3,
        200,
        210,
        3000,
        -1
    };

    Car A4 =
    {
        {'A', '4'},
        4,
        310,
        210,
        2400,
        -1
    };

    Car etron =
    {
        {'E', '-', 't', 'r', 'o', 'n'},
        5,
        260,
        210,
        2400,
        -1
    };

    Car Rs7 =
    {
        {'R', 's', '-', '7'},
        6,
        380,
        210,
        2600,
        -1
    };

    AddCar(1, M3);
    AddCar(1, M2);
    AddCar(2, G63);
    AddCar(4, A4);
    AddCar(4, etron);
    AddCar(4, Rs7);

    DeleteDealer(1);
    DeleteCar(5);

    AddDealer
    (
        {
            {'A', 'u', 'd', 'i'},
            {'O','d','e','s','s','a'},
            10,
            -1
        }
    );
    AddCar
    (
        10,
        {
        {'E', '-', 't', 'r', 'o', 'n', '-', 'G', 'T'},
        599,
        345,
        111,
        3000,
        -1
        }
    );
    GetAllCarsInfo();
    GetAllDealersInfo();
}

void test4()
{
    makeFiles(true);
    const int SIZE = 5;
    Dealer old_dealers[SIZE] = {
        {
            {'B', 'M', 'W'},
            {'K', 'y', 'i', 'v'},
            1,
            -1
        },
        {
            {'B', 'e', 'n', 'z'},
            {'L', 'v', 'i', 'v'},
            2,
            -1
        },
        {
            {'A', 'u', 'd', 'i'},
            {'Z', 'h', 'i', 't', 'o', 'm', 'i', 'r'},
            3,
            -1
        },
        {
            {'A', 'u', 'd', 'i'},
            {'K', 'a', 'n', 'i', 'v'},
            4,
            -1
        },
        {
            {'B', 'M', 'W'},
            {'L', 'v', 'i', 'v'},
            5,
            -1
        },
    };

    for (int i = 0; i < SIZE; i++)
    {
        AddDealer(old_dealers[i]);
    }

    Car M3 =
    {
        {'M', '3'},
        1,
        345,
        300,
        1500,
        -1
    };

    Car M2 =
    {
        {'M', '2'},
        2,
        300,
        230,
        1300,
        -1
    };

    Car G63 =
    {
        {'A', 'M', 'G'},
        3,
        200,
        210,
        3000,
        -1
    };

    Car A4 =
    {
        {'A', '4'},
        4,
        310,
        210,
        2400,
        -1
    };

    Car etron =
    {
        {'E', '-', 't', 'r', 'o', 'n'},
        5,
        260,
        210,
        2400,
        -1
    };

    Car Rs7 =
    {
        {'R', 's', '-', '7'},
        6,
        380,
        210,
        2600,
        -1
    };

    AddCar(1, M3);
    AddCar(1, M2);
    AddCar(2, G63);
    AddCar(4, A4);
    AddCar(4, etron);
    AddCar(4, Rs7);

    DeleteDealer(1);
    DeleteCar(5);

    AddDealer
    (
        {
            {'A', 'u', 'd', 'i'},
            {'O','d','e','s','s','a'},
            10,
            -1
        }
    );
    AddCar
    (
        10,
        {
        {'E', '-', 't', 'r', 'o', 'n', '-', 'G', 'T'},
        599,
        345,
        111,
        3000,
        -1
        }
    );
    printAllDealers();
    int temp;
    cout << "Enter Delaer ID\n";
    cin >> temp;
    UpdateDealer(temp);
    printAllCars();
    cout << "Enter Car ID\n";
    cin >> temp;
    UpdateCar(temp);

    GetAllCarsInfo();
    GetAllDealersInfo();
}

void runTests()
{
    int n = 0;
    while (n != -1)
    {
        cout << "Enter the number of test(1-4); -1 to exit :\n";
        cin >> n;
        switch (n)
        {
        case 1:
            test1();
            cout << "Test " << n << " completed!\n\n";
            break;
        case 2:
            test2();
            cout << "Test " << n << " completed!\n\n";
            break;
        case 3:
            test3();
            cout << "Test " << n << " completed!\n\n";
            break;
        case 4:
            test4();
            cout << "Test " << n << " completed!\n\n";
            break;
        default:
            break;
        }
        
    }
}
int main()
{
    makeFiles(false);
    Menu();
    //runTests();
}