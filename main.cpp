#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <fstream>
#include <cctype>
#include <cstdlib> // For system("cls") and system("pause")

using namespace std;

// --- قواعد البيانات ---
const string CLIENTS_DATA_FILE = "Clients.txt";
const string USERS_DATA_FILE = "Users.txt";

// --- هياكل البيانات ---
struct stClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    float AccountBalance;
    bool MarkedForDeletion = false;
};

struct stUser
{
    string Username;
    string Password;
    int Permissions = 0;
    bool MarkedForDeletion = false;
};

// --- المتغيرات العامة ---
stUser CurrentUser;

// --- صلاحيات المستخدمين (Permissions Enum) ---
enum enPermissions
{
    pListClients = 1,
    pAddNewClient = 2,
    pDeleteClient = 4,
    pUpdateClient = 8,
    pFindClient = 16,
    pTransactions = 32,
    pManageUsers = 64,
    pAll = -1
};

// --- تصريح عن الدوال (Function Prototypes) ---
void LoginScreen();
void ShowMainMenu();
void ShowTransactionsMenu();
void ShowManageUsersMenu();

// =======================================================
// --- دوال الأدوات المساعدة (Utility Functions) ---
// =======================================================

string ReadString(string Message)
{
    string S;
    cout << Message;
    // Handles names with spaces
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    getline(cin, S);
    return S;
}


string ReadNonSpacedString(string Message)
{
    string S;
    cout << Message;
    cin >> S;
    return S;
}

float ReadFloat(string Message)
{
    float Num;
    cout << Message;
    cin >> Num;
    return Num;
}

char ReadChar(string Message)
{
    char C;
    cout << Message;
    cin >> C;
    return C;
}

void GoBackToMainMenu()
{
    cout << "\n\nPress any key to go back to Main Menu...";
    system("pause>0");
}

void GoBackToPreviousMenu()
{
    cout << "\n\nPress any key to go back...";
    system("pause>0");
}

// =======================================================
// --- دوال معالجة السلاسل النصية والملفات ---
// =======================================================

vector<string> SplitString(string s1, string Delimiter = "||")
{
    vector<string> vResult;
    string sWord;
    size_t pos = 0;
    while ((pos = s1.find(Delimiter)) != string::npos)
    {
        sWord = s1.substr(0, pos);
        if (sWord != "")
        {
            vResult.push_back(sWord);
        }
        s1.erase(0, pos + Delimiter.length());
    }
    if (s1 != "")
    {
        vResult.push_back(s1);
    }
    return vResult;
}

stClient ConvertClientLineToRecord(string Line)
{
    vector<string> vClientData = SplitString(Line);
    stClient Client;
    if (vClientData.size() >= 5)
    {
        Client.AccountNumber = vClientData[0];
        Client.PinCode = vClientData[1];
        Client.Name = vClientData[2];
        Client.Phone = vClientData[3];
        Client.AccountBalance = stof(vClientData[4]);
    }
    return Client;
}

stUser ConvertUserLineToRecord(string Line)
{
    vector<string> vUserData = SplitString(Line);
    stUser User;
    if (vUserData.size() >= 3)
    {
        User.Username = vUserData[0];
        User.Password = vUserData[1];
        User.Permissions = stoi(vUserData[2]);
    }
    return User;
}

vector<stClient> LoadClientsDataFromFile(string FileName)
{
    vector<stClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in); // Read Mode
    if (MyFile.is_open())
    {
        string Line;
        while (getline(MyFile, Line))
        {
            vClients.push_back(ConvertClientLineToRecord(Line));
        }
        MyFile.close();
    }
    return vClients;
}

vector<stUser> LoadUsersDataFromFile(string FileName)
{
    vector<stUser> vUsers;
    fstream MyFile;
    MyFile.open(FileName, ios::in); // Read Mode
    if (MyFile.is_open())
    {
        string Line;
        while (getline(MyFile, Line))
        {
            vUsers.push_back(ConvertUserLineToRecord(Line));
        }
        MyFile.close();
    }
    return vUsers;
}

string ConvertClientRecordToLine(const stClient& Client, string Delimiter = "||")
{
    return Client.AccountNumber + Delimiter +
        Client.PinCode + Delimiter +
        Client.Name + Delimiter +
        Client.Phone + Delimiter +
        to_string(Client.AccountBalance);
}

string ConvertUserRecordToLine(const stUser& User, string Delimiter = "||")
{
    return User.Username + Delimiter +
        User.Password + Delimiter +
        to_string(User.Permissions);
}

void AppendDataLineToFile(string FileName, string DataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app); // Append Mode
    if (MyFile.is_open())
    {
        MyFile << DataLine << endl;
        MyFile.close();
    }
}

void SaveClientsDataToFile(string FileName, const vector<stClient>& vClients)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out); // Overwrite Mode
    if (MyFile.is_open())
    {
        for (const stClient& C : vClients)
        {
            if (!C.MarkedForDeletion)
            {
                MyFile << ConvertClientRecordToLine(C) << endl;
            }
        }
        MyFile.close();
    }
}

void SaveUsersDataToFile(string FileName, const vector<stUser>& vUsers)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out); // Overwrite Mode
    if (MyFile.is_open())
    {
        for (const stUser& U : vUsers)
        {
            if (!U.MarkedForDeletion)
            {
                MyFile << ConvertUserRecordToLine(U) << endl;
            }
        }
        MyFile.close();
    }
}

// =======================================================
// --- دوال التحقق من الصلاحيات ---
// =======================================================

bool CheckAccessPermission(enPermissions Permission)
{
    if (CurrentUser.Permissions == enPermissions::pAll)
    {
        return true;
    }
    if ((Permission & CurrentUser.Permissions) == Permission)
    {
        return true;
    }
    else
    {
        cout << "\n-----------------------------------\n";
        cout << "Access Denied! \nYou don't have permission to do this.\nPlease contact your Admin.";
        cout << "\n-----------------------------------\n";
        return false;
    }
}

// =======================================================
// --- دوال إدارة العملاء ---
// =======================================================

bool FindClientByAccountNumber(string AccountNumber, stClient& Client)
{
    vector<stClient> vClients = LoadClientsDataFromFile(CLIENTS_DATA_FILE);
    for (const stClient& C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }
    }
    return false;
}

void PrintClientCard(const stClient& Client)
{
    cout << "\nThe following are the client details:\n";
    cout << "-----------------------------------\n";
    cout << "Account Number: " << Client.AccountNumber << endl;
    cout << "Pin Code      : " << Client.PinCode << endl;
    cout << "Name          : " << Client.Name << endl;
    cout << "Phone         : " << Client.Phone << endl;
    cout << "Balance       : " << Client.AccountBalance << endl;
    cout << "-----------------------------------\n";
}

stClient ReadNewClient()
{
    stClient Client;
    Client.AccountNumber = ReadNonSpacedString("Enter Account Number: ");
    stClient tempClient; // To check if account number exists
    while (FindClientByAccountNumber(Client.AccountNumber, tempClient))
    {
        cout << "\nClient with [" << Client.AccountNumber << "] already exists, Enter another Account Number? ";
        cin >> Client.AccountNumber;
    }

    Client.PinCode = ReadNonSpacedString("Enter Pin Code: ");
    Client.Name = ReadString("Enter Name: ");
    Client.Phone = ReadNonSpacedString("Enter Phone: ");
    Client.AccountBalance = ReadFloat("Enter Account Balance: ");

    return Client;
}


void AddNewClient()
{
    stClient Client = ReadNewClient();
    AppendDataLineToFile(CLIENTS_DATA_FILE, ConvertClientRecordToLine(Client));
    cout << "\nClient Added Successfully.\n";
}

void ShowAddNewClientsScreen()
{
    char addMore = 'Y';
    do
    {
        system("cls");
        cout << "-----------------------------------\n";
        cout << "\tAdd New Client Screen\n";
        cout << "-----------------------------------\n";
        AddNewClient();
        addMore = ReadChar("\nDo you want to add more clients? (Y/N)? ");
    } while (toupper(addMore) == 'Y');
}

stClient ChangeClientRecord(string AccountNumber)
{
    stClient Client;
    Client.AccountNumber = AccountNumber;
    Client.PinCode = ReadNonSpacedString("Enter Pin Code: ");
    Client.Name = ReadString("Enter Name: ");
    Client.Phone = ReadNonSpacedString("Enter Phone: ");
    Client.AccountBalance = ReadFloat("Enter Balance: ");
    return Client;
}


void ShowDeleteClientScreen()
{
    string AccountNumber = ReadNonSpacedString("Enter Account Number: ");
    vector<stClient> vClients = LoadClientsDataFromFile(CLIENTS_DATA_FILE);
    stClient client;

    if (FindClientByAccountNumber(AccountNumber, client))
    {
        PrintClientCard(client);
        char confirm = ReadChar("\nAre you sure you want to delete this client (Y/N)? ");
        if (toupper(confirm) == 'Y')
        {
            for (stClient& C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C.MarkedForDeletion = true;
                    break;
                }
            }
            SaveClientsDataToFile(CLIENTS_DATA_FILE, vClients);
            cout << "\nClient Deleted Successfully.\n";
        }
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") not found!\n";
    }
}

void ShowUpdateClientScreen()
{
    string AccountNumber = ReadNonSpacedString("Enter Account Number: ");
    vector<stClient> vClients = LoadClientsDataFromFile(CLIENTS_DATA_FILE);
    stClient client;

    if (FindClientByAccountNumber(AccountNumber, client))
    {
        PrintClientCard(client);
        char confirm = ReadChar("\nAre you sure you want to update this client (Y/N)? ");
        if (toupper(confirm) == 'Y')
        {
            for (stClient& C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C = ChangeClientRecord(AccountNumber);
                    break;
                }
            }
            SaveClientsDataToFile(CLIENTS_DATA_FILE, vClients);
            cout << "\nClient Updated Successfully.\n";
        }
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") not found!\n";
    }
}

void ShowFindClientScreen()
{
    string AccountNumber = ReadNonSpacedString("Enter Account Number: ");
    stClient client;
    if (FindClientByAccountNumber(AccountNumber, client))
    {
        PrintClientCard(client);
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") not found!\n";
    }
}

void ShowClientsList()
{
    vector<stClient> vClients = LoadClientsDataFromFile(CLIENTS_DATA_FILE);
    cout << "\n\t\t\t\tClient List (" << vClients.size() << ") Client(s).\n";
    cout << "------------------------------------------------------------------------------------------------\n";
    cout << "| " << left << setw(15) << "Account Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n------------------------------------------------------------------------------------------------\n";

    if (vClients.empty()) {
        cout << "\t\t\t\tNo clients available in the system." << endl;
    }
    else {
        for (const stClient& c : vClients)
        {
            cout << "| " << left << setw(15) << c.AccountNumber;
            cout << "| " << left << setw(10) << c.PinCode;
            cout << "| " << left << setw(40) << c.Name;
            cout << "| " << left << setw(12) << c.Phone;
            cout << "| " << left << setw(12) << c.AccountBalance;
            cout << endl;
        }
    }

    cout << "------------------------------------------------------------------------------------------------\n";
}

// =======================================================
// --- دوال المعاملات ---
// =======================================================

void ShowDepositScreen()
{
    cout << "-----------------------------------\n";
    cout << "\tDeposit Screen\n";
    cout << "-----------------------------------\n";
    string AccountNumber = ReadNonSpacedString("Enter Account Number: ");
    vector<stClient> vClients = LoadClientsDataFromFile(CLIENTS_DATA_FILE);
    stClient client;

    if (FindClientByAccountNumber(AccountNumber, client))
    {
        PrintClientCard(client);
        float amount = ReadFloat("Enter deposit amount: ");
        char confirm = ReadChar("Are you sure you want to perform this transaction (Y/N)? ");
        if (toupper(confirm) == 'Y')
        {
            for (stClient& C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C.AccountBalance += amount;
                    break;
                }
            }
            SaveClientsDataToFile(CLIENTS_DATA_FILE, vClients);
            cout << "\nDeposit completed successfully. New balance is: " << client.AccountBalance + amount << endl;
        }
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") not found!\n";
    }
}

void ShowWithdrawScreen()
{
    cout << "-----------------------------------\n";
    cout << "\tWithdraw Screen\n";
    cout << "-----------------------------------\n";
    string AccountNumber = ReadNonSpacedString("Enter Account Number: ");
    vector<stClient> vClients = LoadClientsDataFromFile(CLIENTS_DATA_FILE);
    stClient client;

    if (FindClientByAccountNumber(AccountNumber, client))
    {
        PrintClientCard(client);
        float amount = ReadFloat("Enter withdraw amount: ");

        while (amount > client.AccountBalance)
        {
            cout << "Amount Exceeds the balance, you can withdraw up to : " << client.AccountBalance << endl;
            amount = ReadFloat("Enter another amount: ");
        }

        char confirm = ReadChar("Are you sure you want to perform this transaction (Y/N)? ");
        if (toupper(confirm) == 'Y')
        {
            for (stClient& C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C.AccountBalance -= amount;
                    break;
                }
            }
            SaveClientsDataToFile(CLIENTS_DATA_FILE, vClients);
            cout << "\nWithdraw completed successfully. New balance is: " << client.AccountBalance - amount << endl;
        }
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") not found!\n";
    }
}

void ShowTotalBalancesScreen()
{
    vector<stClient> vClients = LoadClientsDataFromFile(CLIENTS_DATA_FILE);
    cout << "\n\t\t\t\tBalances List (" << vClients.size() << ") Client(s).\n";
    cout << "------------------------------------------------------------------------------------------------\n";
    cout << "| " << left << setw(15) << "Account Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n------------------------------------------------------------------------------------------------\n";
    float TotalBalances = 0;

    if (vClients.empty()) {
        cout << "\t\t\t\tNo clients available in the system." << endl;
    }
    else {
        for (const stClient& c : vClients)
        {
            cout << "| " << left << setw(15) << c.AccountNumber;
            cout << "| " << left << setw(40) << c.Name;
            cout << "| " << left << setw(12) << c.AccountBalance;
            cout << endl;
            TotalBalances += c.AccountBalance;
        }
    }

    cout << "------------------------------------------------------------------------------------------------\n";
    cout << "\t\t\t\t\tTotal Balances = " << TotalBalances << endl;
}

// =======================================================
// --- دوال إدارة المستخدمين ---
// =======================================================

bool FindUserByUsername(string Username, stUser& User)
{
    vector<stUser> vUsers = LoadUsersDataFromFile(USERS_DATA_FILE);
    for (const stUser& U : vUsers)
    {
        if (U.Username == Username)
        {
            User = U;
            return true;
        }
    }
    return false;
}

int ReadPermissionsToSet()
{
    int Permissions = 0;
    char Answer;
    cout << "Do you want to give full access? (y/n)? ";
    cin >> Answer;
    if (tolower(Answer) == 'y')
    {
        return enPermissions::pAll;
    }

    cout << "\nDo you want to give access to:\n";

    cout << "Show Client List? (y/n)? ";
    cin >> Answer;
    if (tolower(Answer) == 'y') Permissions |= enPermissions::pListClients;

    cout << "Add New Client? (y/n)? ";
    cin >> Answer;
    if (tolower(Answer) == 'y') Permissions |= enPermissions::pAddNewClient;

    cout << "Delete Client? (y/n)? ";
    cin >> Answer;
    if (tolower(Answer) == 'y') Permissions |= enPermissions::pDeleteClient;

    cout << "Update Client? (y/n)? ";
    cin >> Answer;
    if (tolower(Answer) == 'y') Permissions |= enPermissions::pUpdateClient;

    cout << "Find Client? (y/n)? ";
    cin >> Answer;
    if (tolower(Answer) == 'y') Permissions |= enPermissions::pFindClient;

    cout << "Transactions? (y/n)? ";
    cin >> Answer;
    if (tolower(Answer) == 'y') Permissions |= enPermissions::pTransactions;

    cout << "Manage Users? (y/n)? ";
    cin >> Answer;
    if (tolower(Answer) == 'y') Permissions |= enPermissions::pManageUsers;

    return Permissions;
}


void ShowUsersList()
{
    vector<stUser> vUsers = LoadUsersDataFromFile(USERS_DATA_FILE);
    cout << "\n\t\t\t\tUser List (" << vUsers.size() << ") User(s).\n";
    cout << "------------------------------------------------------------------------------------------------\n";
    cout << "| " << left << setw(15) << "Username";
    cout << "| " << left << setw(20) << "Password";
    cout << "| " << left << setw(12) << "Permissions";
    cout << "\n------------------------------------------------------------------------------------------------\n";

    if (vUsers.empty()) {
        cout << "\t\t\t\tNo users available in the system." << endl;
    }
    else {
        for (const stUser& u : vUsers)
        {
            cout << "| " << left << setw(15) << u.Username;
            cout << "| " << left << setw(20) << u.Password;
            cout << "| " << left << setw(12) << u.Permissions;
            cout << endl;
        }
    }

    cout << "------------------------------------------------------------------------------------------------\n";
}


void AddNewUser()
{
    stUser User;
    User.Username = ReadNonSpacedString("Enter Username: ");
    stUser tempUser;
    while (FindUserByUsername(User.Username, tempUser))
    {
        cout << "\nUser with [" << User.Username << "] already exists, Enter another Username? ";
        cin >> User.Username;
    }

    User.Password = ReadNonSpacedString("Enter Password: ");
    User.Permissions = ReadPermissionsToSet();

    AppendDataLineToFile(USERS_DATA_FILE, ConvertUserRecordToLine(User));
    cout << "\nUser Added Successfully.\n";
}

void ShowAddNewUserScreen()
{
    system("cls");
    cout << "-----------------------------------\n";
    cout << "\tAdd New User Screen\n";
    cout << "-----------------------------------\n";
    AddNewUser();
}
// ... [Implement Delete, Update, Find for Users similarly to Clients]


// =======================================================
// --- القوائم الرئيسية (Main Menus) ---
// =======================================================

void ShowTransactionsMenu()
{
    short choice = 0;
    do
    {
        system("cls");
        cout << "===========================================\n";
        cout << "\t\tTransactions Menu Screen\n";
        cout << "===========================================\n";
        cout << "\t[1] Deposit.\n";
        cout << "\t[2] Withdraw.\n";
        cout << "\t[3] Total Balances.\n";
        cout << "\t[4] Main Menu.\n";
        cout << "===========================================\n";
        cout << "Choose what do you want to do? [1 to 4]? ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            system("cls");
            ShowDepositScreen();
            GoBackToPreviousMenu();
            break;
        case 2:
            system("cls");
            ShowWithdrawScreen();
            GoBackToPreviousMenu();
            break;
        case 3:
            system("cls");
            ShowTotalBalancesScreen();
            GoBackToPreviousMenu();
            break;
        case 4:
            // Will exit loop
            break;
        default:
            cout << "\nInvalid choice. Please try again.\n";
            system("pause");
        }
    } while (choice != 4);
}


void ShowManageUsersMenu()
{
    short choice = 0;
    do
    {
        system("cls");
        cout << "===========================================\n";
        cout << "\t\tManage Users Menu Screen\n";
        cout << "===========================================\n";
        cout << "\t[1] List Users.\n";
        cout << "\t[2] Add New User.\n";
        cout << "\t[3] Delete User.\n";
        cout << "\t[4] Update User.\n";
        cout << "\t[5] Find User.\n";
        cout << "\t[6] Main Menu.\n";
        cout << "===========================================\n";
        cout << "Choose what do you want to do? [1 to 6]? ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            system("cls");
            ShowUsersList();
            GoBackToPreviousMenu();
            break;
        case 2:
            system("cls");
            ShowAddNewUserScreen();
            GoBackToPreviousMenu();
            break;
        case 3:
            // ShowDeleteUserScreen();
            cout << "\nThis feature is not implemented yet.\n";
            GoBackToPreviousMenu();
            break;
        case 4:
            // ShowUpdateUserScreen();
            cout << "\nThis feature is not implemented yet.\n";
            GoBackToPreviousMenu();
            break;
        case 5:
            // ShowFindUserScreen();
            cout << "\nThis feature is not implemented yet.\n";
            GoBackToPreviousMenu();
            break;
        case 6:
            // Will exit loop
            return;
        }
    } while (choice != 6);
}

void ShowMainMenu()
{
    short choice = 0;
    do
    {
        system("cls");
        cout << "===========================================\n";
        cout << "\t\tMain Menu Screen\n";
        cout << "===========================================\n";
        cout << "\t[1] Show Client List.\n";
        cout << "\t[2] Add New Client.\n";
        cout << "\t[3] Delete Client.\n";
        cout << "\t[4] Update Client Info.\n";
        cout << "\t[5] Find Client.\n";
        cout << "\t[6] Transactions.\n";
        cout << "\t[7] Manage Users.\n";
        cout << "\t[8] Logout.\n";
        cout << "===========================================\n";

        cout << "Choose what do you want to do? [1 to 8]? ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            if (CheckAccessPermission(enPermissions::pListClients))
            {
                system("cls");
                ShowClientsList();
                GoBackToMainMenu();
            }
            break;
        case 2:
            if (CheckAccessPermission(enPermissions::pAddNewClient))
            {
                system("cls");
                ShowAddNewClientsScreen();
            }
            break;
        case 3:
            if (CheckAccessPermission(enPermissions::pDeleteClient))
            {
                system("cls");
                ShowDeleteClientScreen();
                GoBackToMainMenu();
            }
            break;
        case 4:
            if (CheckAccessPermission(enPermissions::pUpdateClient))
            {
                system("cls");
                ShowUpdateClientScreen();
                GoBackToMainMenu();
            }
            break;
        case 5:
            if (CheckAccessPermission(enPermissions::pFindClient))
            {
                system("cls");
                ShowFindClientScreen();
                GoBackToMainMenu();
            }
            break;
        case 6:
            if (CheckAccessPermission(enPermissions::pTransactions))
            {
                ShowTransactionsMenu();
            }
            break;
        case 7:
            if (CheckAccessPermission(enPermissions::pManageUsers))
            {
                ShowManageUsersMenu();
            }
            break;
        case 8:
            // Will exit loop
            break;
        default:
            cout << "\nInvalid choice. Please try again.\n";
            system("pause");
        }
    } while (choice != 8);
}

// --- نظام تسجيل الدخول ---
bool AuthenticateUser(string Username, string Password)
{
    stUser User;
    if (FindUserByUsername(Username, User))
    {
        if (User.Password == Password)
        {
            CurrentUser = User;
            return true;
        }
    }
    return false;
}

void LoginScreen()
{
    bool LoginFailed = false;
    do
    {
        system("cls");
        cout << "-----------------------------------\n";
        cout << "\tLogin Screen\n";
        cout << "-----------------------------------\n";
        if (LoginFailed)
        {
            cout << "Invalid Username/Password. Please try again.\n";
        }
        string Username = ReadNonSpacedString("Enter Username: ");
        string Password = ReadNonSpacedString("Enter Password: ");

        LoginFailed = !AuthenticateUser(Username, Password);

    } while (LoginFailed);
}

int main()
{
    while (true)
    {
         ShowMainMenu();
    }
    return 0;
}

