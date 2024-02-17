#define OPEN_MODE ios::out | ios::binary | ios::in
#define LONG_TEXT 100
#define SHORT_TEXT 50
#define MASTER_FILEPATH "Dealers.dat"
#define SLAVE_FILEPATH "Cars.dat"
#define OUTPUT_FILEPATH "output.txt"

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

//slave
struct Car
{
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
    char Name[SHORT_TEXT];
    char Adress[LONG_TEXT];
    int DealerID;
    int FirstCarNumberInFile;
};

//Number of elements in file(true if Slave)
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

//Number of Cars in dealer
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

//Get all info about dealer
void GetAllDealersInfo()
{
    ofstream OutputFile1;
    OutputFile1.open(OUTPUT_FILEPATH, std::ofstream::out | std::ofstream::trunc);
    OutputFile1.close();
    if (!OutputFile1.good())
        cout << "Error GetAllDealersInfo O1";

    string Res;
    int i = 0;
    int NumberOfDealers = NumberOfElementsInFile(false);
    Dealer tempDealer;

    ofstream OutputFile2(OUTPUT_FILEPATH);

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

//Get all info about cars
void GetAllCarsInfo()
{
    ofstream OutputFile1;
    OutputFile1.open(OUTPUT_FILEPATH, std::ofstream::out | std::ofstream::trunc);
    OutputFile1.close();
    if (!OutputFile1.good())
        cout << "Error GetAllCarsInfo O1";

    string Res;
    int i = 0;
    int NumberOfCars = NumberOfElementsInFile(true);
    Car tempCar;

    ofstream OutputFile2(OUTPUT_FILEPATH);

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

//get the Car object
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

//get the dealer object
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

//Change parameter of Car
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
        int temp;
        cin >> n;
        cout << "Enter new value:\n";
        switch (n)
        {
        case 0:
            cin >> temp;
            tempCar.CarID = temp;
            break;
        case 1:
            cin.getline((char*)&tempCar.Model, SHORT_TEXT);
            break;
        case 2:
            cin >> temp;
            tempCar.HP = temp;
            break;
        case 3:
            cin >> temp;
            tempCar.MaxSpeed = temp;
            break;
        case 4:
            cin >> temp;
            tempCar.Weigth = temp;
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
    else {
        cout << "Error UpdateCar No car\n";
    }
}

//Change parameter of dealer
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
            cin.getline((char*)&tempDealer.Name, SHORT_TEXT);
            break;
        case 2:
            cin.getline((char*)&tempDealer.Name, LONG_TEXT);
            break;
        default:
            cout << "Error UpdateDealer Not in range 0-2";
            break;
        }

        ofstream WriteMaster2(SLAVE_FILEPATH, OPEN_MODE);
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

//Add car to dealer
void AddCar(int DealerID, Car NewCar)//problem line marked
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

            while (currentCarNumber != -1) //
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

//Add dealer
void AddDealer(Dealer NewDealer)
{
    ofstream WriteMaster1(MASTER_FILEPATH, OPEN_MODE);

    WriteMaster1.seekp(0, WriteMaster1.end);
    WriteMaster1.write((char*)&NewDealer, sizeof(Dealer));

    WriteMaster1.close();
    if (!WriteMaster1.good())
        cout << "Error AddDealer M1";
}

void Menu()
{
    int n = 0;
    cout <<
        "get-m 1 \n" <<
        "get-s 2 \n" <<
        "del-m 3 \n" <<
        "del-s 4 \n" <<
        "update-m 5 \n" <<
        "update-s 6 \n" <<
        "insert-m 7 \n" <<
        "insert-s 8 \n" <<
        "calc-m 9 \n" <<
        "calc-s 10 \n" <<
        "ut-m 11 \n" <<
        "ut-s 12\n";
    //executeTasks();
}

void makeFiles() {
    remove(MASTER_FILEPATH);
    remove(SLAVE_FILEPATH);
    remove(OUTPUT_FILEPATH);
    
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
    
    ifstream output(OUTPUT_FILEPATH, OPEN_MODE);
    isopen = output.is_open();
    output.close();

    if (!isopen) {
        ofstream output(OUTPUT_FILEPATH);
        output.close();
    }

    
}

int main()
{
    makeFiles();

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

    //GetAllDealersInfo();

   // cout << NumberOfElementsInFile(false);
    //cout << "\n" << NumberOfElementsInFile(true) << "\n";
    GetAllCarsInfo();
}