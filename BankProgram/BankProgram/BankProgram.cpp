#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdlib>

using namespace std;
static string FileName = "ClientsData.txt";
void MainMenuScreen();
void TransactionsMenuScreen();

enum enOptions
{
    ClientsList = 1, AddClient = 2, DeleteClient = 3, UpdateClient = 4, FindClient = 5, Transactions = 6, Exit = 7
};

enum enTransactions
{
    Deposit = 1, Withdraw = 2, TotalBalances = 3, MainMenu = 4
};

struct stClientData
{
    string AccountNumber, PinCode, Name, Phone;
    float AccountBalance = 0;
    bool MarkedForDeletion = false;
};

string ReadString(string Message)
{
    string Data;

    cout << Message;
    getline(cin >> ws, Data);

    return Data;
}

char ReadCharacter(string Message)
{
    char Letter;

    cout << Message;
    cin >> Letter;

    return Letter;
}

short ReadOptionNumber()
{
    short Operation;
    
    do
    {
        cout << "\nChoose what do you want to do [1 to 7]: ";
        cin >> Operation;
    } while (Operation < 1 || Operation > 7);

    return Operation;
}

void BackToMainMenu()
{
    cout << "\nPress any key to go back to main menu...";
    system("pause>0");
    MainMenuScreen();
}

vector<string> SplitString(string S, string Delimeter)
{
    vector <string> vWords;
    short DelimPosition;
    string Word;

    while ((DelimPosition = S.find(Delimeter)) != string::npos)
    {
        Word = S.substr(0, DelimPosition);
        if (Word != "")
            vWords.push_back(Word);

        S.erase(0, DelimPosition + Delimeter.length());
    }

    if (S != "")
        vWords.push_back(S);

    return vWords;
}

stClientData ConvertLineToRecord(string LineOfData, string Dilemeter = "#//#")
{
    stClientData Client;
    vector <string> vClientData = SplitString(LineOfData, Dilemeter);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);

    return Client;
}

vector <stClientData> LoadAllClientsinVector()
{
    fstream MyFile;
    vector <stClientData> vClientData;
    string ClientDataLine;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        while (getline(MyFile, ClientDataLine))
            vClientData.push_back(ConvertLineToRecord(ClientDataLine));

        MyFile.close();
    }
    return vClientData;
}

void ShowTableHeader(short ClientsNumber)
{
    cout << "\n\t\t\t\tClient list (" << ClientsNumber << ") client(s)\n";
    cout << "________________________________________________________________________________________\n\n";
}

void TableFirstRow()
{
    cout << "| " << left << setw(20) << "Account Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(25) << "Client Name";
    cout << "| " << left << setw(14) << "Phone Number ";
    cout << "| " << left << setw(9) << "Balance\n";
    cout << "_______________________________________________________________________________________\n\n";
}

void PrintOneClientData(stClientData Client)
{
    cout << "| " << left << setw(20) << Client.AccountNumber;
    cout << "| " << left << setw(10) << Client.PinCode;
    cout << "| " << left << setw(25) << Client.Name;
    cout << "| " << left << setw(14) << Client.Phone;
    cout << "| " << left << setw(9) << Client.AccountBalance;
}

void PrintAllClientsData(vector <stClientData>& vClients)
{
    for (stClientData& Data : vClients)
    {
        PrintOneClientData(Data);
        cout << endl;
    }
    cout << "________________________________________________________________________________________\n";
}

void ShowClientsList()
{
    system("cls");
    vector <stClientData> vClients = LoadAllClientsinVector();

    ShowTableHeader(vClients.size());
    TableFirstRow();
    PrintAllClientsData(vClients);
}

void AddNewClientScreen()
{
    cout << "\n------------------------------------------\n";
    cout << "         Add New Client Screen\n";
    cout << "------------------------------------------\n";
    cout << "Adding a new client:\n\n";
}

string CheckAccNumberExistence(vector <stClientData>& vClients)
{
    string AccNumber = ReadString("Enter account number: ");
    bool Exists = true;

    while(Exists)
    {
        Exists = false;
        for (stClientData& C : vClients)
        {
            if (C.AccountNumber == AccNumber)
            {
                cout << "\n\nClient with [" << AccNumber << "] already exists, ";
                AccNumber = ReadString("Enter another account number: ");
                Exists = true;
                break;
            }
        }
    }
    return AccNumber;
}

stClientData ReadNewClient(vector <stClientData>& vClients)
{
    stClientData Client;

    cout << "\nPlease enter new client data:\n\n";

    Client.AccountNumber = CheckAccNumberExistence(vClients);

    cout << "Enter Pin Code: ";
    getline(cin, Client.PinCode);

    cout << "Enter Name: ";
    getline(cin, Client.Name);

    cout << "Enter Phone Number: ";
    getline(cin, Client.Phone);

    cout << "Enter Account Balance: ";
    cin >> Client.AccountBalance;

    return Client;
}

void AddDataLineToFile(string DataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {
        MyFile << DataLine << endl;
        MyFile.close();
    }
}

string ConvertRecordToLine(stClientData Client, string Dilemeter = "#//#")
{
    string ClientData = "";

    ClientData += Client.AccountNumber + Dilemeter;
    ClientData += Client.PinCode + Dilemeter;
    ClientData += Client.Name + Dilemeter;
    ClientData += Client.Phone + Dilemeter;
    ClientData += to_string(Client.AccountBalance);

    return ClientData;
}

void AddNewClient(vector <stClientData>& vClients)
{
    stClientData Client = ReadNewClient(vClients);
    AddDataLineToFile(ConvertRecordToLine(Client));
}

void AddClients()
{
    char AddMore = 'Y';
    vector <stClientData> vClients = LoadAllClientsinVector();

    do
    {
        system("cls");
        AddNewClientScreen();
        AddNewClient(vClients);

        cout << "Client added successfully! Do you want to add more clients? [y/n]: ";
        cin >> AddMore;
    } while (toupper(AddMore) == 'Y');
}

void DeleteClientScreen()
{
    system("cls");
    cout << "\n------------------------------------------------\n";
    cout << "              Delete Client Screen\n";
    cout << "------------------------------------------------\n";
}

bool FindClientDataUsingAccountNumber(string AccountNumber, vector <stClientData>& vClients, stClientData& ClientData)
{
    for (stClientData& Client : vClients)
    {
        if (Client.AccountNumber == AccountNumber)
        {
            ClientData = Client;
            return true;
        }
    }
    return false;
}

void MarkClientForDeletion(vector <stClientData>& vClients, string AccNumber)
{
    for (stClientData& Client : vClients)
    {
        if (Client.AccountNumber == AccNumber)
        {
            Client.MarkedForDeletion = true;
            break;
        }
    }
}

void PrintClientCard(stClientData Client)
{
    cout << "\nThe following are the client's details:";
    cout << "\n---------------------------------------";
    cout << "\nAccount Number : " << Client.AccountNumber;
    cout << "\nPin Code       : " << Client.PinCode;
    cout << "\nName           : " << Client.Name;
    cout << "\nPhone Number   : " << Client.Phone;
    cout << "\nAccount Balance: " << Client.AccountBalance;
    cout << "\n---------------------------------------";
}

void SaveClientsVectorToFile(vector <stClientData>& vClients)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);

    if (MyFile.is_open())
    {
        for (stClientData& C : vClients)
        {
            if (!C.MarkedForDeletion)
                MyFile << ConvertRecordToLine(C) << endl;
        }
        MyFile.close();
    }
}

void DeleteClientByAccNumber()
{
    stClientData Client;
    char AskForDeletion = 'n';
    string AccNumber;
    vector <stClientData> vClients = LoadAllClientsinVector();

    DeleteClientScreen();
    AccNumber = ReadString("\nPlease enter Account Number: ");

    if (FindClientDataUsingAccountNumber(AccNumber, vClients, Client))
    {
        PrintClientCard(Client);

        AskForDeletion = ReadCharacter("\n\nAre you sure you want to delete this client? [y/n]: ");
        if (toupper(AskForDeletion) == 'Y')
        {
            MarkClientForDeletion(vClients, AccNumber);
            SaveClientsVectorToFile(vClients);

            vClients = LoadAllClientsinVector();

            cout << "\n\nClient deleted successfully!";
        }
    }
    else
    {
        cout << "\nClient with account number (" << AccNumber << ") is not found";
    }
}

void UpdateClientInfoScreen()
{
    system("cls");
    cout << "\n---------------------------------------------\n";
    cout << "           Update Client Info Screen\n";
    cout << "---------------------------------------------\n";
}

stClientData UpdateClientData(string AccNumber)
{
    stClientData Client;

    Client.AccountNumber = AccNumber;

    cout << "\nEnter Pin Code     : ";
    getline(cin >> ws, Client.PinCode);

    cout << "Enter Name         : ";
    getline(cin, Client.Name);

    cout << "Enter Phone Number : ";
    getline(cin, Client.Phone);

    cout << "Enter Balance      : ";
    cin >> Client.AccountBalance;

    return Client;
}

void UpdateClientDataUsingAccNumber()
{
    char AskForUpdate = 'n';
    stClientData Client;
    string AccNumber;
    vector <stClientData> vClients = LoadAllClientsinVector();

    UpdateClientInfoScreen();
    AccNumber = ReadString("\nPlease enter Account Number: ");

    if (FindClientDataUsingAccountNumber(AccNumber, vClients, Client))
    {
        PrintClientCard(Client);

        AskForUpdate = ReadCharacter("\n\nAre you sure you want to update this client's data? [y/n]: ");

        if (toupper(AskForUpdate) == 'Y')
        {
            for (stClientData& C : vClients)
            {
                if (C.AccountNumber == AccNumber)
                {
                    C = UpdateClientData(AccNumber);
                    break;
                }
            }
            SaveClientsVectorToFile(vClients);

            cout << "\n\nClient's data is updated successfully!";
        }
    }
    else
    {
        cout << "\nClient with account number (" << AccNumber << ") is not found";
    }
}

void FindClientScreen()
{
    system("cls");
    cout << "\n---------------------------------------------\n";
    cout << "           Find Client Screen\n";
    cout << "---------------------------------------------\n";
}

void FindClientByAccNumber()
{
    string AccNumber;
    stClientData Client;
    vector <stClientData> vClients = LoadAllClientsinVector();

    FindClientScreen();
    AccNumber = ReadString("\nPlease enter Account Number: ");

    if (FindClientDataUsingAccountNumber(AccNumber, vClients, Client))
        PrintClientCard(Client);
    else
        cout << "\nClient with account number [" << AccNumber << "] is not found!";
}

void ProgramEndScreen()
{
    system("cls");
    cout << "\n---------------------------------------------\n";
    cout << "           Program End Screen :-)\n";
    cout << "---------------------------------------------\n";
}

short ReadTransactionNumber()
{
    short Transaction;

    cout << "Choose what do you want to do [1 to 4]: ";
    cin >> Transaction;

    return Transaction;
}

void DepositScreen()
{
    system("cls");
    cout << "\n------------------------------------------------\n";
    cout << "\t\tDeposit Screen\n";
    cout << "------------------------------------------------\n";
}

void DepositAmountUsingAccNumber(vector <stClientData>& vClients, string AccNumber, double Amount)
{
    char Perform;

    Perform = ReadCharacter("\nAre you sure you want to perform this transaction? [y/n]: ");

    if (toupper(Perform) == 'Y')
    {
        for (stClientData& C : vClients)
        {
            if (C.AccountNumber == AccNumber)
            {
                C.AccountBalance += Amount;
                cout << "\nDone Successfully! New balance is " << C.AccountBalance;
                break;
            }
        }
    }
    SaveClientsVectorToFile(vClients);
}

void DepositPerform()
{
    string AccNumber;
    vector <stClientData> vClients = LoadAllClientsinVector();
    stClientData Client;
    double DepositAmount = 0;

    DepositScreen();
    AccNumber = ReadString("\nPlease enter account number: ");

    while (!FindClientDataUsingAccountNumber(AccNumber, vClients, Client))
    {
        cout << "\nClient with [" << AccNumber << "] is not found, Please enter another account number: ";
        cin >> AccNumber;
    }

    PrintClientCard(Client);

    cout << "\nPlease enter deposit amount: ";
    cin >> DepositAmount;

    DepositAmountUsingAccNumber(vClients, AccNumber, DepositAmount);
}

void BackToTransactionsMenu()
{
    cout << "\nPress any key to back to tansactions menu...";
    system("pause>0");
    TransactionsMenuScreen();
}

void WithdrawScreen()
{
    system("cls");
    cout << "\n-----------------------------------------------\n";
    cout << "\t\tWithdraw Screen\n";
    cout << "-----------------------------------------------\n";
}

void WithdrawPerform()
{
    string AccNumber;
    vector <stClientData> vClients = LoadAllClientsinVector();
    stClientData Client;
    float WithdrawAmount = 0;

    WithdrawScreen();
    AccNumber = ReadString("\nPlease enter account number: ");

    while (!FindClientDataUsingAccountNumber(AccNumber, vClients, Client))
    {
        cout << "\nClient with [" << AccNumber << "] is not found, Please enter another account number: ";
        cin >> AccNumber;
    }

    PrintClientCard(Client);

    cout << "\nPlease enter withdraw amount: ";
    cin >> WithdrawAmount;

    while (WithdrawAmount > Client.AccountBalance)
    {
        cout << "\nAmount exceeds the balance, you can withdraw up to: " << Client.AccountBalance;
        cout << "\nPlease enter another amount: ";
        cin >> WithdrawAmount;
    }
    DepositAmountUsingAccNumber(vClients, AccNumber, WithdrawAmount * (-1));
}

void TotalBalancesScreen(short ClientsNumber)
{
    system("cls");
    cout << "\n\t\tBalances List (" << ClientsNumber << ") Client(s)";
    cout << "\n____________________________________________________________________\n";

    cout << "\n| " << left << setw(15) << "Account Number";
    cout << " | " << left << setw(25) << "Client Name";
    cout << " | " << left << setw(10) << "Balance";
    cout << "\n____________________________________________________________________";
}

void PrintOneClientBalance(stClientData Client)
{
    cout << "\n| " << left << setw(15) << Client.AccountNumber;
    cout << " | " << left << setw(25) << Client.Name;
    cout << " | " << left << setw(10) << Client.AccountBalance;
}

void PrintTotalBalances()
{
    vector <stClientData> vClients = LoadAllClientsinVector();
    double Total = 0;

    TotalBalancesScreen(vClients.size());
    for (stClientData& C : vClients)
    {
        PrintOneClientBalance(C);
        Total += C.AccountBalance;
    }
    cout << "\n____________________________________________________________________\n";
    cout << "\n\t\tTotal Balances = " << Total << endl;
}

void PerformTransaction(enTransactions Transaction)
{
    switch (Transaction)
    {
    case Deposit:
        DepositPerform();
        BackToTransactionsMenu();
        break;
    case Withdraw:
        WithdrawPerform();
        BackToTransactionsMenu();
        break;
    case TotalBalances:
        PrintTotalBalances();
        BackToTransactionsMenu();
        break;
    case MainMenu:
        MainMenuScreen();
        break;
    }
}

void TransactionsMenuScreen()
{
    system("cls");
    cout << "========================================================\n";
    cout << "\t\tTransactions Menu Screen\n";
    cout << "========================================================\n";
    cout << "\t[1] Deposit.\n";
    cout << "\t[2] Withdraw.\n";
    cout << "\t[3] Total Balances.\n";
    cout << "\t[4] Main Menu.\n";
    cout << "========================================================\n";
    PerformTransaction((enTransactions)ReadTransactionNumber());
}

void PerformOption(enOptions Option)
{
    switch (Option)
    {
    case ClientsList:
        ShowClientsList();
        BackToMainMenu();
        break;
    case AddClient:
        AddClients();
        BackToMainMenu();
        break;
    case DeleteClient:
        DeleteClientByAccNumber();
        BackToMainMenu();
        break;
    case UpdateClient:
        UpdateClientDataUsingAccNumber();
        BackToMainMenu();
        break;
    case FindClient:
        FindClientByAccNumber();
        BackToMainMenu();
        break;
    case Transactions:
        TransactionsMenuScreen();
        break;
    case Exit:
        ProgramEndScreen();
        break;
    }
}

void MainMenuScreen()
{
    system("cls");
    cout << "================================================\n";
    cout << "               Main Menu Screen                 \n";
    cout << "================================================\n";
    cout << "     [1] Show Clients List.\n";
    cout << "     [2] Add A New Client.\n";
    cout << "     [3] Delete A Client.\n";
    cout << "     [4] Update A Client's Info.\n";
    cout << "     [5] Find A Client.\n";
    cout << "     [6] Transactios.\n";
    cout << "     [7] Exit.\n";
    cout << "================================================\n";
    PerformOption((enOptions)ReadOptionNumber());
}

int main()
{

    MainMenuScreen();
    return 0;
}