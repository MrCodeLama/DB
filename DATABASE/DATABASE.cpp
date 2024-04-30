#define OPEN_MODE ios::out | ios::binary | ios::in
#define LONG_TEXT 100
#define SHORT_TEXT 50
#define MASTER_FILEPATH "Events.dat"
#define SLAVE_FILEPATH "Members.dat"
#define OUTPUT_MEMBERS_FILEPATH "outputMembers.txt"
#define OUTPUT_EVENTS_FILEPATH "outputEvents.txt"
#define TEMP_FILEPATH "temp.dat"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

//slave
struct Member
{
    char Name[SHORT_TEXT];
    int ID;
    int NumberOfNextMemberInFile;
};

//master
struct Event
{
    char Name[SHORT_TEXT];
    char Adress[LONG_TEXT];
    int EventID;
    int FirstMemberNumberInFile;
};

Member MemberReturn;
Event EventReturn;

int NumberOfElementsInFile(bool _isSlave)
{
    ifstream readfile((_isSlave ? SLAVE_FILEPATH : MASTER_FILEPATH), OPEN_MODE);

    readfile.seekg(0, readfile.end);
    size_t rcount = readfile.tellg() / (_isSlave ? sizeof(Member) : sizeof(Event));

    readfile.close();
    if (!readfile.good())
        cout << "Error NumberOfElementsInFile";

    return rcount;
}

int NumberOfMembersInDealer(int EventID)
{
    int NumberOfEvents = NumberOfElementsInFile(false);
    int i = 0;
    Event tempEvent;
    Member tempMember;
    bool checker = false;
    int counter = 0;
    int currentMemberNumber = 0;

    ifstream ReadMaster1(MASTER_FILEPATH, OPEN_MODE);

    for (; i < NumberOfEvents; i++)
    {
        ReadMaster1.seekg(i * sizeof(Event), ReadMaster1.beg);
        ReadMaster1.read((char*)&tempEvent, sizeof(Event));
        if (tempEvent.EventID == EventID)
        {
            currentMemberNumber = tempEvent.FirstMemberNumberInFile;
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

        while (currentMemberNumber != -1)
        {
            ReadSlave1.seekg(currentMemberNumber * sizeof(Member), ReadSlave1.beg);
            ReadSlave1.read((char*)&tempMember, sizeof(Member));

            currentMemberNumber = tempMember.NumberOfNextMemberInFile;
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

void GetAllEventsInfo()
{
    ofstream OutputFile1;
    OutputFile1.open(OUTPUT_EVENTS_FILEPATH, std::ofstream::out | std::ofstream::trunc);
    OutputFile1.close();
    if (!OutputFile1.good())
        cout << "Error GetAllDealersInfo O1";

    string Res;
    int i = 0;
    int NumberOfEvents = NumberOfElementsInFile(false);
    Event tempEvent;

    ofstream OutputFile2(OUTPUT_EVENTS_FILEPATH);

    for (; i < NumberOfEvents; i++)
    {
        ifstream ReadMaster1(MASTER_FILEPATH, OPEN_MODE);
        ReadMaster1.seekg(i * sizeof(Event), ReadMaster1.beg);
        ReadMaster1.read((char*)&tempEvent, sizeof(Event));

        Res += "Name: " + string(tempEvent.Name);
        Res += "\nAdress: " + string(tempEvent.Adress);
        Res += "\nDealerID: " + to_string(tempEvent.EventID);
        Res += "\nFirstMemberNumberInFile: " + to_string(tempEvent.FirstMemberNumberInFile);
        Res += "\n\n";

        ReadMaster1.close();
        if (!ReadMaster1.good())
            cout << "Error GetAllEventsInfo M1";

        OutputFile2 << Res;

        Res.clear();
    }

    OutputFile2.close();
    if (!OutputFile2.good())
        cout << "Error GetAllDealersInfo O2";
}

void GetAllMembersInfo()
{
    ofstream OutputFile1;
    OutputFile1.open(OUTPUT_MEMBERS_FILEPATH, std::ofstream::out | std::ofstream::trunc);
    OutputFile1.close();
    if (!OutputFile1.good())
        cout << "Error GetAllCarsInfo O1";

    string Res;
    int i = 0;
    int NumberOfMember = NumberOfElementsInFile(true);
    Member tempMember;

    ofstream OutputFile2(OUTPUT_MEMBERS_FILEPATH);

    for (; i < NumberOfMember; i++)
    {
        ifstream ReadSlave1(SLAVE_FILEPATH, OPEN_MODE);

        ReadSlave1.seekg(i * sizeof(Member), ReadSlave1.beg);
        ReadSlave1.read((char*)&tempMember, sizeof(Member));

        Res += "Model: " + string(tempMember.Name);
        Res += "\nCar ID: " + to_string(tempMember.ID);
        Res += "\nNumberOfNextMemberInFile: " + to_string(tempMember.NumberOfNextMemberInFile);
        Res += "\n\n";
        ReadSlave1.close();
        if (!ReadSlave1.good())
            cout << "Error GetAllMembersInfo S1";

        OutputFile2 << Res;

        Res.clear();
    }

    OutputFile2.close();
    if (!OutputFile2.good())
        cout << "Error GetAllMembersInfo O2";
}

Member GetMember(int MemberID)
{
    int NumerOfMembers = NumberOfElementsInFile(true);
    int i = 0;
    Member tempMember = {};

    ifstream ReadSlave1(SLAVE_FILEPATH, OPEN_MODE);

    for (; i < NumerOfMembers; i++)
    {
        ReadSlave1.seekg(i * sizeof(Member), ReadSlave1.beg);
        ReadSlave1.read((char*)&tempMember, sizeof(Member));
        if (tempMember.ID == MemberID)
            break;
    }

    ReadSlave1.close();
    if (!ReadSlave1.good())
        cout << "Error GetCar";

    return tempMember;
}

Event GetEvent(int EventID)
{
    int NumberOfEvents = NumberOfElementsInFile(false);
    int i = 0;
    Event tempEvent = {};

    ifstream ReadMaster1(MASTER_FILEPATH, OPEN_MODE);

    for (; i < NumberOfEvents; i++)
    {
        ReadMaster1.seekg(i * sizeof(Event), ReadMaster1.beg);
        ReadMaster1.read((char*)&tempEvent, sizeof(Event));
        if (tempEvent.EventID == EventID)
            break;
    }

    ReadMaster1.close();
    if (!ReadMaster1.good())
        cout << "Error GetEvent";

    return tempEvent;
}

void UpdateMember(int MemberID)
{
    int NumberOfMembers = NumberOfElementsInFile(true);
    int i = 0;
    Member tempMember;
    bool _isMemberExists = false;

    ifstream ReadSlave1(SLAVE_FILEPATH, OPEN_MODE);

    for (; i < NumberOfMembers; i++)
    {
        ReadSlave1.seekg(i * sizeof(Member), ReadSlave1.beg);
        ReadSlave1.read((char*)&tempMember, sizeof(Member));
        if (tempMember.ID == MemberID)
        {
            _isMemberExists = true;
            break;
        }
    }

    ReadSlave1.close();
    if (!ReadSlave1.good())
        cout << "Error UpdateCar S1";

    if (_isMemberExists)
    {
        cout << "Enter Value to change: 0 - Name 1 - ID \n";
        int n;
        cin >> n;
        cout << "Enter new value:\n";
        switch (n)
        {
        case 0:
            cin >> tempMember.Name;
            break;
        case 1:
            cin >> tempMember.ID;
            break;
        default:
            cout << "Error UpdateCar not in range 0-1";
            break;
        }

        ofstream WriteSlave2(SLAVE_FILEPATH, OPEN_MODE);
        WriteSlave2.seekp(i * sizeof(Member), WriteSlave2.beg);
        WriteSlave2.write((char*)&tempMember, sizeof(Member));

        WriteSlave2.close();
        if (!WriteSlave2.good())
            cout << "Error UpdateMember S2";
    }
    else
        cout << "Error UpdateMember No Member\n";
}

void UpdateEvent(int EventID)
{
    int NumberOfEvents = NumberOfElementsInFile(false);
    int i = 0;
    Event tempEvent;
    bool _isEventExists = false;
    int TaskNumber;
    int temp;

    ifstream ReadMaster1(MASTER_FILEPATH, OPEN_MODE);

    for (; i < NumberOfEvents; i++)
    {
        ReadMaster1.seekg(i * sizeof(Event), ReadMaster1.beg);
        ReadMaster1.read((char*)&tempEvent, sizeof(Event));
        if (tempEvent.EventID == EventID)
        {
            _isEventExists = true;
            break;
        }
    }

    ReadMaster1.close();
    if (!ReadMaster1.good())
        cout << "Error UpdateEvent M1";

    if (_isEventExists)
    {
        cout << "Enter Value to change: 0 - EventID 1 - Name 2 - Adress\n";

        cin >> TaskNumber;
        cout << "Enter new value:\n";
        switch (TaskNumber)
        {
        case 0:
            cin >> temp;
            tempEvent.EventID = temp;
            break;
        case 1:
            cin >> tempEvent.Name;
            break;
        case 2:
            cin >> tempEvent.Adress;
            break;
        default:
            cout << "Error UpdateDealer Not in range 0-2";
            break;
        }

        ofstream WriteMaster2(MASTER_FILEPATH, OPEN_MODE);
        WriteMaster2.seekp(i * sizeof(Event), WriteMaster2.beg);
        WriteMaster2.write((char*)&tempEvent, sizeof(Event));

        WriteMaster2.close();
        if (!WriteMaster2.good())
            cout << "Error UpdateDealer M2";
    }
    else {
        cout << "Error UpdateDealer Dealer doesnt exists\n";
    }
}

void AddMember(int EventID, Member NewMember)
{
    int NumberOfEvents = NumberOfElementsInFile(false);
    int NumberOfMembers = NumberOfElementsInFile(true);
    bool checker = false;
    int i = 0;
    Event tempEvent;
    Member tempMember;
    int currentMemberNumber = -1;
    int previousMemberNumber = -1;

    ifstream ReadMaster1(MASTER_FILEPATH, OPEN_MODE);

    for (; i < NumberOfEvents; i++)
    {
        ReadMaster1.seekg(i * sizeof(Event), ReadMaster1.beg);
        ReadMaster1.read((char*)&tempEvent, sizeof(Event));
        if (tempEvent.EventID == EventID)
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
        currentMemberNumber = tempEvent.FirstMemberNumberInFile;
        checker = (currentMemberNumber == -1);

        if (checker)
        {
            tempEvent.FirstMemberNumberInFile = NumberOfMembers;

            ofstream WriteSlave2(SLAVE_FILEPATH, OPEN_MODE);

            WriteSlave2.seekp(NumberOfMembers * sizeof(Member), WriteSlave2.beg);
            WriteSlave2.write((char*)&NewMember, sizeof(Member));

            WriteSlave2.close();
            if (!WriteSlave2.good())
                cout << "Error AddCar S2";

            ofstream WriteMaster2(MASTER_FILEPATH, OPEN_MODE);

            WriteMaster2.seekp(i * sizeof(Event), WriteMaster2.beg);
            WriteMaster2.write((char*)&tempEvent, sizeof(Event));

            WriteMaster2.close();
            if (!WriteMaster2.good())
                cout << "Error AddCar M2";
        }
        else
        {
            ifstream ReadSlave1(SLAVE_FILEPATH, OPEN_MODE);

            while (currentMemberNumber != -1)
            {
                ReadSlave1.seekg(currentMemberNumber * sizeof(Member), ReadSlave1.beg);
                ReadSlave1.read((char*)&tempMember, sizeof(Member));

                previousMemberNumber = currentMemberNumber;
                currentMemberNumber = tempMember.NumberOfNextMemberInFile;

            }

            ReadSlave1.close();
            if (!ReadSlave1.good())
                cout << "Error AddCar S1";

            tempMember.NumberOfNextMemberInFile = NumberOfMembers;

            ofstream WriteSlave3(SLAVE_FILEPATH, OPEN_MODE);
            WriteSlave3.seekp(previousMemberNumber * sizeof(Member), WriteSlave3.beg);
            WriteSlave3.write((char*)&tempMember, sizeof(Member));

            WriteSlave3.seekp(NumberOfMembers * sizeof(Member), WriteSlave3.beg);
            WriteSlave3.write((char*)&NewMember, sizeof(Member));

            WriteSlave3.close();
            if (!WriteSlave3.good())
                cout << "Error AddCar S3";
        }
    }
}

void AddEvent(Event NewEvent)
{
    ofstream WriteMaster1(MASTER_FILEPATH, OPEN_MODE);

    WriteMaster1.seekp(0, WriteMaster1.end);
    WriteMaster1.write((char*)&NewEvent, sizeof(Event));

    WriteMaster1.close();
    if (!WriteMaster1.good())
        cout << "Error AddDealer M1";
}

void makeFiles(bool _removeOldFiles) {
    if (_removeOldFiles)
    {
        remove(MASTER_FILEPATH);
        remove(SLAVE_FILEPATH);
        remove(OUTPUT_MEMBERS_FILEPATH);
        remove(OUTPUT_EVENTS_FILEPATH);
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

    ifstream outputCars(OUTPUT_MEMBERS_FILEPATH, OPEN_MODE);
    isopen = outputCars.is_open();
    outputCars.close();

    if (!isopen) {
        ofstream outputCars(OUTPUT_MEMBERS_FILEPATH);
        outputCars.close();
    }

    ifstream outputDealers(OUTPUT_EVENTS_FILEPATH, OPEN_MODE);
    isopen = outputDealers.is_open();
    outputDealers.close();

    if (!isopen) {
        ofstream outputDealers(OUTPUT_EVENTS_FILEPATH);
        outputDealers.close();
    }

}

Member CreateMember()
{
    

    Member tempMember;

    cout << "Enter Member Name:\n";
    cin >> tempMember.Name;
    cout << "Enter Member id:\n";
    cin >> tempMember.ID;
    tempMember.NumberOfNextMemberInFile = -1;

    return tempMember;
}

Event CreateEvent()
{
    Event tempEvent;

    cout << "Enter Event Name:\n";
    cin >> tempEvent.Name;
    cout << "Enter Event Adress:\n";
    cin >> tempEvent.Adress;
    cout << "Enter Event ID:\n";
    cin >> tempEvent.EventID;
    cout << "\n\n";
    tempEvent.FirstMemberNumberInFile = -1;

    return tempEvent;
}

void DeleteMember(int MemberID)
{
    int numberOfMembers = NumberOfElementsInFile(true);
    int numberOfEvents = NumberOfElementsInFile(false);
    int i = 0;
    Member member, prevMember;
    Event eventOfMemberToDel;
    bool memberExists = false;
    bool isCarFoundInDealers = false;
    bool prevCarExists = false;

    ifstream rs1(SLAVE_FILEPATH, OPEN_MODE);

    for (; i < numberOfMembers; i++)
    {
        rs1.seekg(i * sizeof(Member), rs1.beg);
        rs1.read((char*)&member, sizeof(Member));

        if (member.ID == MemberID) {
            memberExists = true;
            break;
        }
    }

    if (!memberExists) {
        cout << "Error member doesn't exist\n";
        return;
    }

    ifstream rm1(MASTER_FILEPATH, OPEN_MODE);

    for (int j = 0; j < numberOfEvents; j++)
    {
        rm1.seekg(j * sizeof(Event), rm1.beg);
        rm1.read((char*)&eventOfMemberToDel, sizeof(Event));

        if (eventOfMemberToDel.FirstMemberNumberInFile == -1)
            continue;

        if (eventOfMemberToDel.FirstMemberNumberInFile == i) {
            eventOfMemberToDel.FirstMemberNumberInFile = (member.NumberOfNextMemberInFile == -1)
                ? -1 : member.NumberOfNextMemberInFile;

            ofstream wm(MASTER_FILEPATH, OPEN_MODE);

            wm.seekp(j * sizeof(Event), wm.beg);
            wm.write((char*)&eventOfMemberToDel, sizeof(Event));

            wm.close();
            if (!wm.good()) {
                cout << "Error Edit Master";
                return;
            }

            isCarFoundInDealers = true;
            break;
        }

        Member tempMember;
        rs1.seekg(eventOfMemberToDel.FirstMemberNumberInFile * sizeof(Member), rs1.beg);
        rs1.read((char*)&prevMember, sizeof(Member));

        if (prevMember.NumberOfNextMemberInFile == -1)
            continue;

        do {
            rs1.seekg(prevMember.NumberOfNextMemberInFile * sizeof(Member), rs1.beg);
            rs1.read((char*)&tempMember, sizeof(Member));

            if (prevMember.NumberOfNextMemberInFile != i) {
                prevMember = tempMember;
                continue;
            }

            prevMember.NumberOfNextMemberInFile = tempMember.NumberOfNextMemberInFile;

            prevCarExists = true;
            isCarFoundInDealers = true;
            break;
        } while (tempMember.NumberOfNextMemberInFile != -1);

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

    Member tempMember;
    ifstream rs2(SLAVE_FILEPATH, OPEN_MODE);

    for (int j = 0; j < numberOfMembers; j++)
    {
        if (j == i) {
            continue;
        }

        rs2.seekg(j * sizeof(Member), rs2.beg);
        rs2.read((char*)&tempMember, sizeof(Member));

        if (prevCarExists && (prevMember.ID == tempMember.ID))
            tempMember.NumberOfNextMemberInFile = prevMember.NumberOfNextMemberInFile;

        int num = tempMember.NumberOfNextMemberInFile;
        tempMember.NumberOfNextMemberInFile = (num > i) ? (num - 1) : num;
        int newNumberInFile = (j > i) ? (j - 1) : j;

        ofstream wtemp(TEMP_FILEPATH, OPEN_MODE);

        wtemp.seekp(newNumberInFile * sizeof(Member), wtemp.beg);
        wtemp.write((char*)&tempMember, sizeof(Member));

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

    Event tempEvent;
    ifstream rm2(MASTER_FILEPATH, OPEN_MODE);

    for (int j = 0; j < numberOfEvents; j++)
    {
        rm2.seekg(j * sizeof(Event), rm2.beg);
        rm2.read((char*)&tempEvent, sizeof(Event));

        int num = tempEvent.FirstMemberNumberInFile;
        tempEvent.FirstMemberNumberInFile = (num > i) ? (num - 1) : num;

        ofstream wtemp(TEMP_FILEPATH, OPEN_MODE);

        wtemp.seekp(j * sizeof(Event), wtemp.beg);
        wtemp.write((char*)&tempEvent, sizeof(Event));

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

void DeleteMembersFromEvent(int EventID)
{
    int numberOfMembers = NumberOfElementsInFile(true);
    int numberOfEvents = NumberOfElementsInFile(false);
    int i = 0;
    Event event;
    bool dealerExists = false;


    // check if the dealer exists

    ifstream rm1(MASTER_FILEPATH, OPEN_MODE);

    for (; i < numberOfEvents; i++)
    {
        rm1.seekg(i * sizeof(Event), rm1.beg);
        rm1.read((char*)&event, sizeof(Event));

        if (event.EventID == EventID) {
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

    if (event.FirstMemberNumberInFile == -1) {
        cout << "Dealer has no cars!";
        return;
    }

    Member tempMember;
    vector<int> carIDs;
    int indx = event.FirstMemberNumberInFile;

    ifstream rs1(SLAVE_FILEPATH, OPEN_MODE);

    while (true) {
        rs1.seekg(indx * sizeof(Member), rs1.beg);
        rs1.read((char*)&tempMember, sizeof(Member));

        carIDs.push_back(tempMember.ID);

        if (tempMember.NumberOfNextMemberInFile == -1)
            break;

        indx = tempMember.NumberOfNextMemberInFile;
    }

    rs1.close();
    if (!rs1.good()) {
        cout << "Error DeleteMembersFromDealer rs1.good()";
        return;
    }


    // delete all cars of the dealer

    for (int& id : carIDs)
        DeleteMember(id);
}

void DeleteEvent(int EventID)
{
    int NumberOfEvents = NumberOfElementsInFile(false);
    int i = 0;
    Event tempEvent;
    bool _isDealerExists = false;
    int counter = 0;

    ifstream ReadMaster1(MASTER_FILEPATH, OPEN_MODE);

    for (; i < NumberOfEvents; i++)
    {
        ReadMaster1.seekg(i * sizeof(Event), ReadMaster1.beg);
        ReadMaster1.read((char*)&tempEvent, sizeof(Event));
        if (tempEvent.EventID == EventID)
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
        if (tempEvent.FirstMemberNumberInFile != -1)
        {
            DeleteMembersFromEvent(tempEvent.EventID);
        }

        ifstream tempFile(TEMP_FILEPATH, OPEN_MODE);
        bool isopen = tempFile.is_open();
        tempFile.close();

        if (!isopen) {
            ofstream tempFile(TEMP_FILEPATH);
            tempFile.close();
        }

        for (int j = 0; j < NumberOfEvents; j++)
        {
            if (j == i)
                continue;

            ifstream ReadMaster2(MASTER_FILEPATH, OPEN_MODE);

            ReadMaster2.seekg(j * sizeof(Event), ReadMaster2.beg);
            ReadMaster2.read((char*)&tempEvent, sizeof(Event));

            ReadMaster2.close();
            if (!ReadMaster2.good())
                cout << "Error DeleteEvent M2";

            ofstream WriteTemp1(TEMP_FILEPATH, OPEN_MODE);

            WriteTemp1.seekp(counter++ * sizeof(Event), WriteTemp1.beg);
            WriteTemp1.write((char*)&tempEvent, sizeof(Event));

            WriteTemp1.close();
            if (!WriteTemp1.good())
                cout << "Error DeleteEvent T1";
        }

        remove(MASTER_FILEPATH);
        rename(TEMP_FILEPATH, MASTER_FILEPATH);

    }
    else {
        cout << "Error UpdateEvent Event doesnt exists\n";
    }
}

void printAllEvents()
{
    cout << "\nList of Events\nName - id\n";
    int NumberOfEvents = NumberOfElementsInFile(false);
    Event tempEvent;

    ifstream ReadMaster1(MASTER_FILEPATH, OPEN_MODE);
    cout << "\n";

    for (int i = 0; i < NumberOfEvents; i++)
    {
        ReadMaster1.seekg(i * sizeof(Event), ReadMaster1.beg);
        ReadMaster1.read((char*)&tempEvent, sizeof(Event));
        cout << tempEvent.Name << " - " << tempEvent.EventID << "\n";
    }
    cout << "\n";

    ReadMaster1.close();
    if (!ReadMaster1.good())
        cout << "Error AddCar M1";
}

void printAllMembers()
{
    cout << "\nList of cars\Name - id\n";
    int NumberOfDealers = NumberOfElementsInFile(true);
    Member tempMember;

    ifstream ReadMaster1(SLAVE_FILEPATH, OPEN_MODE);
    cout << "\n";

    for (int i = 0; i < NumberOfDealers; i++)
    {
        ReadMaster1.seekg(i * sizeof(Member), ReadMaster1.beg);
        ReadMaster1.read((char*)&tempMember, sizeof(Member));
        cout << tempMember.Name << " - " << tempMember.ID << "\n";
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
            printAllMembers();
            cout << "Enter member id\n";
            cin >> carId;
            MemberReturn = GetMember(carId);
            break;
        case 2:
            printAllEvents();
            cout << "Enter event id\n";
            cin >> dealerId;
            EventReturn = GetEvent(dealerId);
            break;
        case 3:
            printAllMembers();
            cout << "Enter member id\n";
            cin >> carId;
            DeleteMember(carId);
            break;
        case 4:
            printAllEvents();
            cout << "Enter event id\n";
            cin >> dealerId;
            DeleteEvent(dealerId);
            break;
        case 5:
            printAllMembers();
            cout << "Enter member id\n";
            cin >> carId;
            UpdateMember(carId);
            break;
        case 6:
            printAllEvents();
            cout << "Enter event id\n";
            cin >> dealerId;
            UpdateEvent(dealerId);
            break;
        case 7:
            printAllMembers();
            printAllEvents();
            cout << "Enter event id\n";
            cin >> dealerId;
            AddMember(dealerId, CreateMember());
            break;
        case 8:
            printAllEvents();
            AddEvent(CreateEvent());
            break;
        case 9:
            cout << NumberOfElementsInFile(false) << "\n";
            break;
        case 10:
            cout << "Enter event id\n";
            cin >> dealerId;
            cout << NumberOfMembersInDealer(dealerId) << "\n";
            break;
        case 11:
            cout << NumberOfElementsInFile(true) << "\n";
            break;
        case 12:
            GetAllEventsInfo();
            cout << "Check " << OUTPUT_EVENTS_FILEPATH << " file\n";
            break;
        case 13:
            GetAllMembersInfo();
            cout << "Check " << OUTPUT_MEMBERS_FILEPATH << " file\n";
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
    Event events[SIZE] = {
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
        AddEvent(events[i]);
    }

    Member M3 =
    {
        {'M', '3'},
        1,
        -1
    };

    Member M2 =
    {
        {'M', '2'},
        2,
        -1
    };

    Member G63 =
    {
        {'G', '6', '3'},
        3,
        -1
    };

    Member A4 =
    {
        {'A', '4'},
        4,
        -1
    };

    Member etron =
    {
        {'e', 't'},
        5,
        -1
    };

    Member Rs7 =
    {
        {'r', 's', 7 },
        6,
        -1
    };

    AddMember(1, M3);
    AddMember(1, M2);
    AddMember(2, G63);
    AddMember(4, A4);
    AddMember(4, etron);
    AddMember(4, Rs7);

    GetAllEventsInfo();
    GetAllMembersInfo();
}

int main()
{
    makeFiles(true);
    //Menu();
    test1();
}