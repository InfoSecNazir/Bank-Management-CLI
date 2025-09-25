#include<iostream>
#include<iomanip>
#include<string >
#include<fstream>
#include<vector>
using namespace std;
struct StUser
{
	string UserName;
	string Password;
	int Permession;
	bool Mark_of_Delete = false;

};
enum enTranstionOptions {
	enDepostion = 1, enWithdrawe = 2, enShowTotalBalnce = 3,EnMaineMenu = 4
};
enum Mangment_User_Options {
	enList_User = 1, enAdd_New_User = 2, EnDelete_User = 3, EnUpadete_User = 4, EnSearch_For_User = 5, EnMaineMenu2 = 6
};
enum EnManeManueOption
{EnLiast_Cilent=1,En_Add_Cilent=2,En_Delelte_Cilent=3,EnUpadate_Cilent=4,En_Faind_Cilent=5,En_Transtion_Cilent=6,En_Mangmet_Cilent=7,En_Log_out=8

};
enum enMainMenuePermissions{ 
	eAll = -1, pListClients = 1, pAddNewClient = 2, pDeleteClient = 4, pUpdateClients = 8, pFindClient = 16, pTranactions = 32, pManageUsers = 64 };
const string UserFileName = "Users.txt";
const string ClientFileName = "Clients.txt";

StUser CurrentUser;
void showManueManueChoses();
void Show_Transtion_Choses();
void Show_Manue_User_Mangment();
void Login();


struct Stcilent
{
	string Acounter_Number;
	string Pin_Code;
	string Name;
	string Number_Phone;
	float Balnce_Number;
	bool Mark_of_Delete = false;
};

string Read_String(string Message)
{
	string Value = "";
	cout << Message;
	cin >> Value;
	return Value;
}
int Read_Numebr_i(string Massage) {
		int Value = 0;
	cout << Massage;
	cin >> Value;
	return Value;
}
float Read_Numebr_f(string Massage) {
	float Value = 0;
	cout << Massage;
	cin >> Value;
	return Value;
}
char Read_Char(string Massage) {
	char Value = ' ';
	cout << Massage;
	cin >> Value;
	return Value;
}

Stcilent Read_Cilent_Data()
{
	Stcilent Client;
	Client.Acounter_Number = Read_String("Please Enter Acounter Number : ");
	Client.Pin_Code = Read_String("Please Enter Pin Code : ");
	Client.Name = Read_String("Please Enter Name : ");
	Client.Number_Phone = Read_String("Please Enter Number Phone : ");
	Client.Balnce_Number = Read_Numebr_f("Please Enter Balnce Number : ");
	return Client;
}

Stcilent Read_Cilent_Data(string Acounter_Number)
{
	Stcilent Client;
	Client.Acounter_Number = Acounter_Number;
	Client.Pin_Code = Read_String("Please Enter Pin Code : ");
	Client.Name = Read_String("Please Enter Name : ");
	Client.Number_Phone = Read_String("Please Enter Number Phone : ");
	Client.Balnce_Number = Read_Numebr_f("Please Enter Balnce Number : ");
	return Client;
}

//=============================================================================
//                        fauction of String
//=============================================================================
string Convert_Recorde_ToL_in_Cilent(Stcilent Client, string Seperator = "||")
{
	string Recorde = "";
	Recorde += Client.Acounter_Number + Seperator;
	Recorde += Client.Pin_Code + Seperator;
	Recorde += Client.Name + Seperator;
	Recorde += Client.Number_Phone + Seperator;
	Recorde += to_string(Client.Balnce_Number) + Seperator;
	Recorde += to_string(Client.Mark_of_Delete);
	return Recorde;
}
string Convert_Recorde_ToL_in_User(StUser User, string Seperator = "||")
{
	string Recorde = "";
	Recorde += User.UserName + Seperator;
	Recorde += User.Password + Seperator;
	Recorde += to_string(User.Permession);
 	return Recorde;
}
vector<string>Split_string(string Recorde, string Seperator = "||")
{
	vector<string> VString;
	short Pos = 0;
	string sWord;
	while ((Pos = Recorde.find(Seperator)) != std::string::npos)
	{
		sWord = Recorde.substr(0, Pos);
		if (sWord != "")
			VString.push_back(sWord);
		Recorde.erase(0, Pos + Seperator.length());
	}
	if (Recorde != "")
		VString.push_back(Recorde);
	return VString;
}
// Corrected Function
Stcilent Convert_Lin_To_Recoede_Cilent(string Lin, string Sparator = "||") {
	Stcilent Cilent;
	vector<string> VString = Split_string(Lin, Sparator);

	// يجب التحقق من 6 حقول وليس 5
	if (VString.size() >= 5) { // نستخدم >= احتياطاً
		Cilent.Acounter_Number = VString[0];
		Cilent.Pin_Code = VString[1];
		Cilent.Name = VString[2];
		Cilent.Number_Phone = VString[3];
		Cilent.Balnce_Number = stof(VString[4]);
		// إذا كان حقل الحذف موجوداً، يمكنك قراءته أيضاً
		// if (VString.size() == 6) {
		//     Cilent.Mark_of_Delete = (VString[5] == "1");
		// }
	}
	return Cilent;
}
// Corrected Function
StUser Convert_Lin_To_Recorde_User(string Lin, string Sparator = "||") {
	StUser User;
	vector<string> VString = Split_string(Lin, Sparator);

	if (VString.size() == 3) {
		User.UserName = VString[0];   // تصحيح الإسناد
		User.Password = VString[1];   // تصحيح الإسناد
		User.Permession = stoi(VString[2]);
	}
	return User;
}void WriteDAtaToFaile(string FaileName, string Recorde) {
	fstream MyFile;
	MyFile.open(FaileName, ios::app);
	if (MyFile.is_open()) {
		MyFile << Recorde << endl;
		MyFile.close();
	}
}

vector<Stcilent>Load_Data_From_File_Cilent(string FilleName) {
	vector<Stcilent>VClient;
	fstream MyFile;
	MyFile.open(FilleName, ios::in);
	if (MyFile.is_open()) {
		string Line;
		while (getline(MyFile, Line)) {
			VClient.push_back(Convert_Lin_To_Recoede_Cilent(Line));
		}
	}
	return  VClient;
}

vector<StUser>Load_Data_From_File_User(string FilleName) {
	vector<StUser>VClient;
	fstream MyFile;
	MyFile.open(FilleName, ios::in);
	if (MyFile.is_open()) {
		string Line;
		while (getline(MyFile, Line)) {
			VClient.push_back(Convert_Lin_To_Recorde_User(Line));
		}
	}
	return  VClient;
}

vector<Stcilent>Save_Data_on_File_Cilent( vector<Stcilent>&Vcilent, const string File_Name) {
 	fstream MyFile;
	MyFile.open(File_Name, ios::out|ios::app);
	if (MyFile.is_open()) {
	
		for (Stcilent& Cilent : Vcilent) {
			if (Cilent.Mark_of_Delete == false) {
				MyFile << Convert_Recorde_ToL_in_Cilent(Cilent) << endl;
			}
		}
		MyFile.close();
      }
	return Vcilent;
}

vector<StUser>Save_Data_on_File_user(vector<StUser>& Vcilent, const string File_Name) {
	fstream MyFile;
	MyFile.open(File_Name, ios::out|ios::app);
	if (MyFile.is_open()) {

		for (StUser& Cilent : Vcilent) {
			if (Cilent.Mark_of_Delete == false) {
				MyFile << Convert_Recorde_ToL_in_User(Cilent) << endl;
			}
		}
		MyFile.close();
	}
	return Vcilent;
}


bool Cilent_Exite_By_AconterNumber(string Aconter_Number, string FileName) {
	vector<Stcilent>Vcilent;
	fstream MyFile;
	MyFile.open(FileName, ios::in);
	if (MyFile.is_open()) {
		string Lin;
		Stcilent Cilent;
		while (getline(MyFile, Lin)) {
			Cilent = Convert_Lin_To_Recoede_Cilent(Lin);
			if (Cilent.Acounter_Number == Aconter_Number) {
				MyFile.close();
				return true;
			}
		}
	}
	return false;
}


bool User_Exite_By_AconterNumber(string User_Name, string FileName) {
	vector<StUser>Vcilent;
	fstream MyFile;
	MyFile.open(FileName, ios::in);
	if (MyFile.is_open()) {
		string Lin;
		StUser User;
		while (getline(MyFile, Lin)) {
			User = Convert_Lin_To_Recorde_User(Lin);
			if (User.UserName == User_Name) {
				MyFile.close();
				return true;
			}
		}
	}
	return false;
}

//=============================================================================
//                        fauction of cilent
//=============================================================================
Stcilent Add_Cilent() {
	Stcilent Cilent;
	Cilent.Acounter_Number = Read_String("Please Enter Acounter Number : ");

	while(Cilent_Exite_By_AconterNumber(Cilent.Acounter_Number, ClientFileName))
	{
		cout << "This Acounter Number is Exite , Please Enter anther Acounter Number" << endl;
		Cilent.Acounter_Number = Read_String("Please Enter Acounter Number : ");
	}

	Cilent = Read_Cilent_Data(Cilent.Acounter_Number);

	
	return Cilent;
}

int ChosesPrmises() {
	int Peimase = 0;
	char C = 'n';
	char A = 'y';
	A = Read_Char("Do You Wante any  Permision (Y/N) : ");
	if (tolower(A) == 'y') {
		C = Read_Char("Do  You Want Show Lista Mainue (Y/N) :  ");
		if (tolower(C) == 'y')Peimase |= enMainMenuePermissions::pListClients;
		C = Read_Char("Do  You Want Show Add Cilent (Y/N) :  ");
		if (tolower(C) == 'y')Peimase |= enMainMenuePermissions::pAddNewClient;
		C = Read_Char("Do  You Want Show Faind Cilent  (Y/N) :  ");
		if (tolower(C) == 'y')Peimase |= enMainMenuePermissions::pFindClient;
		C = Read_Char("Do  You Want Show Delete Cilent (Y/N) :  ");
		if (tolower(C) == 'y')Peimase |= enMainMenuePermissions::pDeleteClient;
		C = Read_Char("Do  You Want Show Upadate Cilent (Y/N) :  ");
		if (tolower(C) == 'y')Peimase |= enMainMenuePermissions::pUpdateClients;
		C = Read_Char("Do  You Want Show Transtion Cilent (Y/N) :  ");
		if (tolower(C) == 'y')Peimase |= enMainMenuePermissions::pTranactions;
		C = Read_Char("Do  You Want Show Lista Mangment (Y/N) :  ");
		if (tolower(C) == 'y')Peimase |= enMainMenuePermissions::pManageUsers;
	}
	else {
		Peimase = enMainMenuePermissions::eAll;
	}

	return Peimase;


}

StUser Add_User() {
	StUser User;
	User.UserName = Read_String("Please Enter User Name : ");
	while (User_Exite_By_AconterNumber(User.UserName, UserFileName))
	{
		cout << "This User Name is Exite , Please Enter anther User Name" << endl;
		User.UserName = Read_String("Please Enter User Name : ");
	}
	User.Password = Read_String("Please Enter Password : ");
	User.Permession = ChosesPrmises();
	return User;
}

void Print_Cilent_Recorde(Stcilent& Cilent) {
	cout << "|" << setw(15) << left << Cilent.Acounter_Number;
	cout << "|" << setw(10) << left << Cilent.Pin_Code;
	cout << "|" << setw(40) << left << Cilent.Name;
	cout << "|" << setw(12) << left << Cilent.Number_Phone;
    cout << "|" << setw(12) << left << Cilent.Balnce_Number;

}
void Print_User_Recorde(StUser& User) {
	cout << "|" << setw(15) << left << User.UserName;
	cout << "|" << setw(10) << left << User.Password;
	cout << "|" << setw(40) << left << User.Permession;
}
void Print_Cilent_Recorde_Lin(Stcilent Cilent){
	cout << "|" << setw(15) << left << Cilent.Acounter_Number;
	cout << "|" << setw(40) << left << Cilent.Name;
	cout << "|" << setw(12) << left << Cilent.Balnce_Number;


}

void ShowAccessDeniedMessage() {
	cout << "\n------------------------------------\n";   
	cout << "Access Denied, \nYou dont Have Permission To Do this,\nPlease Conact Your Admin.";   
	cout << "\n------------------------------------\n"; 
}


void ShowAllCilent() {
	vector<Stcilent>Vcilent = Load_Data_From_File_Cilent(ClientFileName);

	cout << "\n\t\t\t\t\t\tList All Cilent (" << Vcilent.size() << ")\n";
	cout << "____________________________________________________________________________________\n";
	cout << "|" << setw(15) << left << "Acounter Number";
	cout << "|" << setw(10) << left << "Pin Code";
	cout << "|" << setw(40) << left << "Name";
	cout << "|" << setw(12) << left << "Phone Num";
	cout << "|" << setw(12) << left << "Balnce Num";
	cout << "\n____________________________________________________________________________________\n";

	if (Vcilent.size() == 0) {
		cout << "No Cilent in the System\n";

	}
	else {
		for (Stcilent& Cilent : Vcilent) {
			Print_Cilent_Recorde(Cilent);
			cout << endl;

		}
	}


	       
	cout << "____________________________________________________________________________________\n";
}
void ShowAllUser() {
	vector<StUser>VUser = Load_Data_From_File_User(UserFileName);

	cout << "\n\t\t\t\t\t\tList All Cilent (" << VUser.size() << ")\n";
	cout << "____________________________________________________________________________________\n";
	cout << "|" << setw(15) << left << "User Name";
	cout << "|" << setw(10) << left << "Passoarde";
	cout << "|" << setw(40) << left << "Primsi";
	cout << "\n____________________________________________________________________________________\n";

	if (VUser.size() == 0) {
		cout << "No Cilent in the System\n";

	}
	else {
		for (StUser& Cilent : VUser) {
			Print_User_Recorde(Cilent);
			cout << endl;

		}
	}



	cout << "____________________________________________________________________________________\n";
}
void Show_Total_Balance() {
	vector<Stcilent>Vcilent = Load_Data_From_File_Cilent(ClientFileName);
	cout << "\n\t\t\t\t\t\tTotal Balance of All Cilent(" << Vcilent.size() << ")\n";
	cout << "_____________________________________________________________________________\n";
	cout << "|" << setw(15) << left << "Acounter_Number";
	cout << "|" << setw(40) << left << "Name";
	cout << "|" << setw(12) << left << "Balnce_Num";
	cout << "\n_____________________________________________________________________________\n";
	float Tital_Balnce = 0;
	if(Vcilent.size() == 0) {
		cout << "No Cilent in the System\n";
	}
	else {
		for (Stcilent& Cilent : Vcilent) {
			Print_Cilent_Recorde_Lin(Cilent);
			cout << endl;
			Tital_Balnce += Cilent.Balnce_Number;
		}
	}
	cout << "_____________________________________________________________________________\n";
	cout << "Tital Balance of All Cilent is : " << Tital_Balnce << endl;

}

void PrintCilentCard(Stcilent& Cilent) {
	cout << "\n\t\t\t\t\t\tCilent Card \n";
	cout << "____________________________________________________________________________________\n";
	cout << "Acounter Number : " << Cilent.Acounter_Number << endl;
	cout << "Pin Code       : " << Cilent.Pin_Code << endl;
	cout << "Name           : " << Cilent.Name << endl;
	cout << "Phone Num     : " << Cilent.Number_Phone << endl;
	cout << "Balnce Num    : " << Cilent.Balnce_Number << endl;
	cout << "____________________________________________________________________________________\n";

}

void PrintUserCard(StUser& User) {
	cout << "\n\t\t\t\t\t\tUser Card \n";
	cout << "____________________________________________________________________________________\n";
	cout << "User Name : " << User.UserName << endl;
	cout << "Password  : " << User.Password << endl;
	cout << "Permession: " << User.Permession << endl;
	cout << "____________________________________________________________________________________\n";
}

bool Find_CilentByAconterNumber(string Aconter_Number, Stcilent& Cilent) {
	vector<Stcilent>Vcilent = Load_Data_From_File_Cilent(ClientFileName);
	for (Stcilent& C : Vcilent) {
		if (Aconter_Number == C.Acounter_Number) {
			Cilent = C;
			return true;
		}
	}
	return false;
}

bool Find_User_Name(string Name_User, StUser& User) {
	vector<StUser>VUser = Load_Data_From_File_User(UserFileName);
	for (StUser& C : VUser) {
		if (Name_User == C.UserName) {
			User = C;
			return true;

		}
   }
	return false;

}

StUser Read_User_Data(string User_Naeme)
{
	StUser User;
	User.UserName = User_Naeme;
	User.Password = Read_String("Plaese Enter Passorde : ");
	User.Permession = ChosesPrmises();
	return User;

}

bool Mark_Cilent_By_AconterNumber(string Aconter_Number,vector<Stcilent>Vcilent) {
	for (Stcilent& Cilent : Vcilent) {
		if (Cilent.Acounter_Number == Aconter_Number) {
			Cilent.Mark_of_Delete = true;
			return true;

		}
	}
	return false;
}
bool Mark_User_By_UserNaem(string User_Name, vector<StUser>&VUser) {
	for (StUser User : VUser) {
		if (User.UserName == User_Name) {
			User.Mark_of_Delete = true;
			return true;
		}
	}
	return false;
}

void Add_Cilent_Fun(string FilleNaem, string StDline) {
	fstream MyFile;
	MyFile.open(FilleNaem, ios::app);
	if(MyFile.is_open())
	{
		MyFile << StDline << endl;
		MyFile.close();
	
	}
}

void Add_New_Cilent() {
	Stcilent Cilent;
	Cilent = Add_Cilent();
	Add_Cilent_Fun(ClientFileName, Convert_Recorde_ToL_in_Cilent(Cilent));

}
void Add_New_User() {
	StUser User;
	User = Add_User();
	Add_Cilent_Fun(UserFileName, Convert_Recorde_ToL_in_User(User));

}

void Add_New_Cilent2() {
	char c = 'y';
	do{
		Add_New_Cilent();
		c = Read_Char("Add Cielnt Scssfule,Do You Want Add anther Cilent (Y/N)");
	
	} while (tolower(c) == 'y');

}
void Add_New_User2() {
	char C = 'y';
	do {
		Add_New_User();
		C = Read_Char("Add User Scsffule,Add User Scssfule,Do You Want Add anther User (Y/N)");
	} while (tolower(C) == 'y');
}


bool Delete_Cilent(string Aconter_Number,vector<Stcilent>Vcilent){
	Stcilent Cilent;
	char C = 'n';
	if (Find_CilentByAconterNumber(Aconter_Number, Cilent)) {
		PrintCilentCard(Cilent);
		char A = Read_Char("Are You Sure You Want to Delete This Cilent (Y/N) : ");
		if (tolower(A) == 'y') {
			Mark_Cilent_By_AconterNumber(Aconter_Number, Vcilent);
			Save_Data_on_File_Cilent(Vcilent, ClientFileName);
			Vcilent = Load_Data_From_File_Cilent(ClientFileName);


			cout << "The Cilent Deleted Scssfule\n";
			return true;
		}
		else {
			cout << "The Cilent Not Deleted \n";
			cout << "Error : You Chose Not to Delete the Cilent \n";
			return false;
		}

	}
	else {
		cout<< "Error : Cilent Not Found \n";
		return false;
	}


}



bool Delete_User(string Name_User, vector<StUser>VUser) {

	if (Name_User == "Admin") {
		cout<< "Error : You Can't Delete Admin User \n";
		return false;
}

	StUser User;
	char C = 'n';
	if (Find_User_Name(Name_User, User)) {
		PrintUserCard(User);
		char A = Read_Char("Are You Sure You Want to Delete This Cilent (Y/N) : ");
		if (tolower(A) == 'y') {
			Mark_User_By_UserNaem(UserFileName, VUser);
			Save_Data_on_File_user(VUser, UserFileName);
			VUser = Load_Data_From_File_User(UserFileName);


			cout << "The User Deleted Scssfule\n";
			return true;
		}
		else {
			cout << "The User Not Deleted \n";
			cout << "Error : You Chose Not to Delete the User \n";
			return false;
		}

	}
	else {
		cout << "Error : User Not Found \n";
		return false;
	}


}
bool Upadate_Cilent(string Aconter_Number, vector<Stcilent>Vcilent) {
	Stcilent Cilent;
	char Answer = 'n';
	if (Find_CilentByAconterNumber(Aconter_Number, Cilent)) {
		PrintCilentCard(Cilent);
		Answer = Read_Char("Are You Sure You Want to Upadate This Cilent (Y/N) : ");
		if (tolower(Answer) == 'y') {
			for (Stcilent& C : Vcilent) {
				if (C.Acounter_Number == Aconter_Number) {
					C = Read_Cilent_Data(C.Acounter_Number);
					break;


				}


			}
			Save_Data_on_File_Cilent(Vcilent, ClientFileName);
			cout << "The Cilent Upadate Scffule \n";
			return true;
		}
		else {

			cout << "The Cient Upadate Errore \n";
			return false;
		}


	}
	cout << "I am sorry The Aconter Number is Not Found in File \n";
	return false;
}

bool Upadate_User(string User, vector<StUser>VUser) {
	StUser User1;
	char Answer = 'n';
	if (Find_User_Name(ClientFileName,User1 )) {
		PrintUserCard(User1);
		Answer = Read_Char("Are You Sure You Want to Upadate This Cilent (Y/N) : ");
		if (tolower(Answer) == 'y') {
			for (StUser& C : VUser) {
				if (C.UserName == User) {
					C = Read_User_Data(C.UserName);
					break;


				}


			}
			Save_Data_on_File_user(VUser, ClientFileName);
			cout << "The User Upadate Scffule \n";
			return true;
		}
		else {

			cout << "The User Upadate Errore \n";
			return false;
		}


	}
	cout << "I am sorry The User-Name is Not Found in File \n";
	return false;
}

bool Depostion_Balnce(string Aconter_Number, float Amount, vector<Stcilent>& Vceint) {
	char C = 'n';
	C = Read_Char("Do You Want Sure Transtion ");
	if (tolower(C) == 'y') {
		for (Stcilent& C : Vceint) {
			if (C.Acounter_Number == Aconter_Number) {
				C.Balnce_Number += Amount;
				Save_Data_on_File_Cilent(Vceint, ClientFileName);
				cout << "Done Scssfuly Depostion .\n";
				return true;
			}
		}

		return false;
	}



}

void Show_User_Cilent_Screen() {
	ShowAllUser();
}
void Show_Add_User() {
	cout << "\n____________________________________________\n";
	cout << "\tAdd New Users";
	cout << "\n____________________________________________\n";
	Add_New_User2();
}
void Show_Add_Cielent() {
	cout << "\n____________________________________________\n";
	cout << "\tAdd New Cilent";
	cout << "\n____________________________________________\n";
	Add_New_Cilent2();
}
void Show_Delete_Screen_U() {
	cout << "\n___________________________________________________\n";
	cout << "\tDelete New User ";
	cout << "\n___________________________________________________\n";

	vector<StUser>VUser = Load_Data_From_File_User(UserFileName);
	string User_Name = Read_String("Enter The User Name Plaese : ");
	Delete_User(User_Name, VUser);

}
void Show_Upadate_Users_Screen(){
	cout << "\n___________________________________________________\n";
	cout << "\tUpqadate New User ";
	cout << "\n___________________________________________________\n";
	vector<StUser>VUser = Load_Data_From_File_User(UserFileName);
	string User_Name = Read_String("Enter The User Naem PLaese : ");
	Upadate_User(User_Name, VUser);
}

void Show_Delete_Screen_C() {
	cout << "\n______________________________________________\n";
	cout << "\tDelete Cilent Screen \n";
	cout << "\n______________________________________________\n";
	vector<Stcilent>Vcielnt = Load_Data_From_File_Cilent(ClientFileName);
	string Acontre_Name = Read_String("Enter The Aconter Number Plaese : ");
	Delete_Cilent(Acontre_Name, Vcielnt);


}
void Show_Upadate_Screen_C() {
	cout << "\n______________________________________________\n";
	cout << "\tDelete Cilent Screen \n";
	cout << "\n______________________________________________\n";
	vector<Stcilent>Vcilent = Load_Data_From_File_Cilent(ClientFileName);
	string Aconter_Namber = Read_String("Enter The Aconter Number Please : ");
	Upadate_Cilent(Aconter_Namber, Vcilent);

}
void Show_Feind_Cilent() {
	cout << "\n______________________________________________\n";
	cout << "\Faind Cilent Screen \n";
	cout << "\n______________________________________________\n";
	vector<Stcilent>Vcilent = Load_Data_From_File_Cilent(ClientFileName);
	Stcilent Cilent;
	string Name = Read_String("Enter The Aconter Number Plaese : ");
	if (Find_CilentByAconterNumber(Name, Cilent)) {
		PrintCilentCard(Cilent);

	}
	else {
		cout << "Cilent Withe Aconter Number [" << Name << "] is Nout Found !" << endl;
	}


}
void Show_Faind_User_Screen() {
	cout << "\n______________________________________________\n";
	cout << "\Faind User Screen \n";
	cout << "\n______________________________________________\n";
	vector<StUser>VUser = Load_Data_From_File_User(UserFileName);
	StUser User_Name;
	string Name_User = Read_String("Enter The User Name Please : ");
	if (Find_User_Name(Name_User, User_Name)) {
		PrintUserCard(User_Name);

	}
	else {
		cout << "The User Name is Not Found ! " << endl;
	}

}

void Showe_End_Screen() {
	cout << "__________________________________\n";
	cout << " \tThe Programeng End :) ";
	cout << "__________________________________\n";
}
void Show_Depostion_Screen() {
	cout << "\n______________________________________________\n";
	cout << "\Depostion Cilent Screen \n";
	cout << "\n______________________________________________\n";
	vector<Stcilent>Vcielnt = Load_Data_From_File_Cilent(ClientFileName);
	string Aconter_Number = Read_String("Enter The aconter Number Plaese : ");
	float Amount = Read_Numebr_f("Enter The Number Balnce : ");
	Depostion_Balnce(Aconter_Number, Amount, Vcielnt);
}

void Show_Withdrawe_screen() {
	cout << "\n______________________________________________\n";
	cout << "\tWhithdrawe Cilent  Screen \n";
	cout << "\n______________________________________________\n";
	Stcilent Cilent;
	vector<Stcilent >Vcilent = Load_Data_From_File_Cilent(ClientFileName);
	string Aconter_Number = Read_String("Enter The Acouter Number Plaese : ");

	PrintCilentCard(Cilent);
	float Amount = Read_Numebr_f("Enter The Amount Withe Drawe Plaese : ");
	while (Amount > Cilent.Balnce_Number) {
		cout << "Amount Exsit You Balnce  to " << Cilent.Balnce_Number << endl;
		Amount = Read_Numebr_f("Please Enter Amont Agine :  ");

	}
	Depostion_Balnce(Aconter_Number, Amount * -1, Vcilent);



}

void Show_All_Total_Balnce_Screen() {
	Show_Total_Balance();
}

void GoBackeToManue() {
	cout << "Press any Kay To  go Bake To Manue Maine !! " << endl;
	system("pause>0");
	showManueManueChoses();
}

void GoBackeToManeTranstion() {
	cout << "\nPress any Kay Go Bake To Manue Transtion \n";
	system("pause>0");
	Show_Transtion_Choses();

}

void GoBackeToManeMangment() {
	cout << "\nPress any Kay Go Backe To Mangment Manue \n";
	system("pause>0");
	Show_Manue_User_Mangment();
}

short Read_Transhiom_Num() {
	int a = 0;
	a = Read_Numebr_i("Chose,What do You Want to do [1 to 4 ] : ");
	return a;
}

void Perfrom_Transtion_Mane(enTranstionOptions  EnMaineMenu1) {

	switch (EnMaineMenu1)
	{
	case enDepostion:
		system("cls");
		Show_Depostion_Screen();
		GoBackeToManeTranstion();
		break;
	case enWithdrawe:
		system("cls");
		Show_Withdrawe_screen();
		GoBackeToManeTranstion();
		break;
	case enShowTotalBalnce:
		system("cls");
		Show_All_Total_Balnce_Screen();
		GoBackeToManeTranstion();
		break;
	case EnMaineMenu:
		showManueManueChoses();

		
		break;
	default:
		cout << "Sorry,The Choses is Not Found The Manue Plaese Choses Agine !\n\n ";

		break;
	}

}
void Show_Transtion_Choses() {
	system("cls");
	cout << "===========================================\n";    
	cout << "\t\tTransactions Menue Screen\n"; 
	cout << "===========================================\n";
	cout << "\t[1] Deposit.\n";  
	cout << "\t[2] Withdraw.\n";   
	cout << "\t[3] Total Balances.\n";    
	cout << "\t[4] Main Menue.\n";
	cout << "===========================================\n";
	Perfrom_Transtion_Mane(enTranstionOptions(Read_Transhiom_Num()));

}

short Read_Maine_Manue_Users() {
	short a = Read_Numebr_i(" Enter The  Chosese The Number [1,6] Plaese : ");
	return a;
}
short Read_Mane_Manue() {
	short R = Read_Numebr_i("Enter The Choses The Numebr [1,8] Plaese : ");
	return R;

}
void Perform_Mane_Manue_Mangmebent_Choses(Mangment_User_Options EnUser) {

	switch (EnUser)
	{
	case enList_User:
		system("cls");
		Show_User_Cilent_Screen();
		GoBackeToManeMangment();
		break;
	case enAdd_New_User:
		system("cls");
		Show_Add_User();
		GoBackeToManeMangment();
		break;
	case EnDelete_User:
		system("cls");
		Show_Delete_Screen_U();
		GoBackeToManeMangment();
		break;
	case EnUpadete_User:
		system("cls");
		Show_Delete_Screen_U();
		GoBackeToManeMangment();
		break;
	case EnSearch_For_User:
		system("cls");

		Show_Delete_Screen_U();
		GoBackeToManeMangment();
		break;
	case EnMaineMenu2:
		showManueManueChoses();

		break;
	default:
		cout << "Sorry, The Choses Number Is Not Found Plaese Enter agine " << endl;

		break;
	}


}
void Show_Manue_User_Mangment() {
	system("cls");
	cout << "===========================================\n";  
	cout << "\t\tManage Users Menue Screen\n";  
	cout << "===========================================\n";     
	cout << "\t[1] List Users.\n";  
	cout << "\t[2] Add New User.\n";	  
	cout << "\t[3] Delete User.\n";  
	cout << "\t[4] Update User.\n";   
	cout << "\t[5] Find User.\n";   
	cout << "\t[6] Main Menue.\n";    
	cout << "===========================================\n";
	Perform_Mane_Manue_Mangmebent_Choses(Mangment_User_Options(Read_Maine_Manue_Users()));

}

bool Fiend_User(string Acounter_Number, string PinDCode, StUser& Cilent) {
	vector<StUser>Vcilent = Load_Data_From_File_User(UserFileName);
	for (StUser& C : Vcilent) {
		if (C.UserName == Acounter_Number && C.Password == PinDCode) {
			Cilent = C;
			return true;
		}
	}
	return false;
}

bool Load_CilentInfo(string Acounter_Number, string PinDCode) {
	return Fiend_User(Acounter_Number, PinDCode, CurrentUser);
}



void Login() {
	bool Scusese = false;
	string User_Name = "";
	string Passoard = "";
	do {
		
		system("cls");
		cout << "\n=============================================================\n";
		cout << "\t\tLogin Scrren ";
		cout << "\n=============================================================\n";
		if (Scusese) {
			cout << "Invdiuele The UserName and Passoarde Please Enter Agine : " << endl;
			cout << "\n_________________________________________________________________________\n";

		}
		User_Name = Read_String("Enter The UserName Plaese : ");
		Passoard = Read_String("Enter The Passoarde Please : ");
		Scusese = !Load_CilentInfo(User_Name, Passoard);

	} while (Scusese);
	showManueManueChoses();
}
bool Chaek_Prissme(enMainMenuePermissions Op){
	if (CurrentUser.Permession == enMainMenuePermissions::eAll)return true;
	if ((Op & CurrentUser.Permession) == Op)return true;
	else {
		ShowAccessDeniedMessage();
	}
}



void Perfrom_Manue_Manue_Opstion(EnManeManueOption EnOption) {

	do {
		switch (EnOption)
		{
			if (Chaek_Prissme(enMainMenuePermissions::pListClients))
			{
		case EnLiast_Cilent:
			system("cls");
			ShowAllCilent();
			GoBackeToManue();
			break;
			}
		case En_Add_Cilent:
			if (Chaek_Prissme(enMainMenuePermissions(enMainMenuePermissions::pAddNewClient))) {
				system("cls");
				Show_Add_Cielent();
				GoBackeToManue();
				break;
			}
		case En_Delelte_Cilent:
			if (Chaek_Prissme(enMainMenuePermissions::pDeleteClient)) {
				system("cls");
				Show_Delete_Screen_C();
				GoBackeToManue();
				break;
			}
		case EnUpadate_Cilent:
			
			if (Chaek_Prissme(enMainMenuePermissions::pUpdateClients)) {
				system("cls");
				Show_Upadate_Screen_C();
				GoBackeToManue();
				break;
			}
		case En_Faind_Cilent:
			if (Chaek_Prissme(enMainMenuePermissions::pFindClient)) {
				system("cls");
				Show_Feind_Cilent();
				GoBackeToManue();


				break;
			}
		case En_Transtion_Cilent:
			if (Chaek_Prissme(enMainMenuePermissions::pTranactions)) {
				system("cls");
				Show_Transtion_Choses();
				GoBackeToManue();
				break;
			}
		case En_Mangmet_Cilent:
			if (Chaek_Prissme(enMainMenuePermissions::pManageUsers)) {
				system("cls");
				Show_Manue_User_Mangment();
				GoBackeToManue();
				break;
			}
		case En_Log_out:
			Login();
			break;
		default:
			cout << "The Choses is Not Found Please Enter Agine Choses : " << endl;
			break;
		}
	} while (EnOption!=8);
}
void showManueManueChoses() {
	system("cls");
	cout << "===========================================\n";
	cout << "\t\tManueManue Menue Screen\n";
	cout << "===========================================\n";
	cout << "\t[1] show List Cilent\n";
	cout << "\t[2] Add Cilent.\n";
	cout << "\t[3] Delete Cilent.\n";
	cout << "\t[4] Update Cilent.\n";
	cout << "\t[5] Find Cilent.\n";
	cout << "\t[6] show Transtion Cilent.\n";
	cout << "\t[7]show Mangment User\n";
	cout << "\t[8]Show Login \n";
	cout << "================================================\n";
	Perfrom_Manue_Manue_Opstion(EnManeManueOption(Read_Numebr_i("Enter The Choses between [1-8] Please : ")));

}


int main() {
 	Login();
	return 0;
}
