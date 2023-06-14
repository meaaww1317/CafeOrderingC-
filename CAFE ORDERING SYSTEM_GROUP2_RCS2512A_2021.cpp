#include<iostream>
#include<iomanip>
#include<fstream>
#include<string.h>
#include<conio.h>
using namespace std;

string giveMemberNo(string membername, string birthday);
char getMenu();
string fileName(char menuCode);
int menuNo(char menuCode);
void saveMenu(struct menuLayout layout[12], ifstream &menu, int itemNo);
void displayMenu(struct menuLayout layout[12], int itemNo);
double displaySum(int selections, int qty, struct menuLayout layout[12]);
void saveReceipt(struct receiptLayout r[100], int oNo,int selections, struct menuLayout layout[12], int qty, double total);
void write_n_display_info_NewMem(ofstream &file_out, char option1, struct memberCard card, double total2, double cash);
void write_n_display_info_OldMem(ofstream &file_out, char option1, string memberNo, double total2, double cash);
void write_n_display_info_Normal(ofstream &file_out, char option1, double total2, double cash);
void receiptType(ofstream &file_out, char option1, char member, char reg, string memberNo, double total2, double cash, string membername, string birthday, string email, int phone, int credit);

struct menuLayout{																					
	int no;
	char name[50];
	double price;
} layout[10];																//MENU LAYOUT

struct receiptLayout{
	int code;
	char name[50];
	int qty;
	double price;
	double sum;
}r[100];																	//RECEIPT LAYOUT									

int main()
{
	int phone, credit, oNo, itemNo, qty, num;
	int selections = 0;
	string memberNo, membername, birthday, email, file;
	char member, reg, option1, menuCode, ans;
	double total, total2, cash, change;

	cout << setw(30) <<"WELCOME TO OUR CAFE!\n"			
		 << "-----------------UwU-----------------\n"
		 << "Are you a member?(Y/N): ";
	cin  >> member;
	if (member=='Y'||member=='y')
	{
		cout<<"Hello! Welcome back!"<<endl
			<<"Please enter member card number: ";
		cin >>memberNo;
		cout << "Hello " << memberNo << "!\n" << endl;
	}
	else if(member=='N'||member=='n')
	{
		cout <<"Would you like to register as a member? (Y/N): ";
		cin>>reg;
		if (reg=='Y'||reg=='y')
		{
			cout<<"Please enter your "<<endl;
			cout<<"Name(only 1 word): ";
			cin>>membername;
			cout<<"Birthday (in numbers: DDMMYYYY, e.g: 07042000): ";
			cin>>birthday;
			cout<<"Email: ";
			cin>>email;
			cout<<"Phone no: ";
			cin>>phone;
			cout<<"Credit no: ";
			cin>>credit;
			memberNo=giveMemberNo(membername, birthday);
			cout<<"Here is your new member card number: "<<memberNo<<endl;;
		}
	}
	else
	{
		cout<< "Sorry you input the wrong code, please try again.";
		return 0;
	}
		
	cout<<endl<<"Where would you like to dine?"<<endl			
		<<"-------------\t---------------"<<endl
		<<"|Dine-in (D)|\t|Take Away(T)|"<<endl
		<<"-------------\t---------------"<<endl
		<<"Please enter D/T:";
	cin>> option1;															//get DINING AREA
	cout<<endl;
	
	do
	{
		menuCode=getMenu();														//get MENU CODE
		file=fileName(menuCode);												//get FILE NAME
		itemNo=menuNo(menuCode);												//get NUMBER OF ITEM IN MENU

		ifstream menu(file.c_str());											//OPEN FILE!
		if(!menu.is_open())
		{
			cout<<"Your menu cannot be displayed, sorry!";
			return 1;
		}
		while(!menu.eof())
		{
		saveMenu(layout, menu, itemNo);	
		}
		menu.close();
		
		displayMenu(layout, itemNo);											//DISPLAY menu
			cout << "\n\tEnter selection: ";
			cin >> selections;													//get MENU CODE
			cout<<"\tQuantity: ";
			cin>>qty;																//get QUANTITY
			total=displaySum(selections, qty, layout);
			oNo=++oNo;															//display SUM
			saveReceipt(r, oNo, selections, layout, qty, total);							//SAVE receipt
			cout<<"Do you want to proceed to pay?(Y/N): ";
			cin>>ans;																//PAY OR NOT?							
	}
	while (ans=='N'||ans=='n');
	
	system("cls");
	cout<<"\t********************************"<<endl;
	cout<<"\t**                                                                                          **"<<endl;
	cout<<"\t**                        THANK YOUR FOR CHOOSING OUR CAFE !!                               **"<<endl;
	cout<<"\t**                                                                                          **"<<endl;
	cout<<"\t********************************"<<endl;
	cout<<"\t**                                                                                          **"<<endl;
	cout<<"\t**       Selections\t Menu\t\t\t\tQty\t\tPrice(RM)"<<endl;
	for (int a=0;a<oNo; a++)
	{
	cout<<"\t**       "<<r[a].code<<"\t\t"<<r[a].name<<"\t"<<r[a].qty<<"\t\t"<<r[a].price<<endl;
	total2=total2+r[a].sum;
	}
	cout<<"\t**       Subtotal = RM"<<setw(5)<<total2<<setw(67)<<"**"<<endl;                                          
	cout<<"\t**       GST      = RM"<<setw(5)<<total2*0.08<<"                                                                 **"<<endl;
	total2=total2+total2*0.08;
	cout<<"\t**       Total    = RM"<<setw(5)<<total2<<"                                                                **"<<endl;
	cout<<"\n\tEnter the amount of cash : RM";
	cin>>cash;
	cout<<endl;
	while(!cash>=total2)
	{
		cout<<"\n\tSorry, the cash is not enough.";
		cout<<"Re-enter the amount of cash: RM" ;
		cin>>cash;
	}
	
	cout<<"\t**       Cash     = RM"<<setw(5)<<cash<<"                                                                 **"<<endl;     
	cout<<"\t**       Change   = RM"<<setw(5)<<cash-((total2)+total2*0.08)<<"                                                                 **"<<endl;
	cout<<"\t**                                                                                          **"<<endl;
	cout<<"\t********************************"<<endl;  
	ofstream file_out ("Receipt.txt",ios::app);
	receiptType(file_out, option1, member, reg, memberNo, total2, cash, membername, birthday, email, phone, credit);
	file_out.close();
	
	
	cout<<"\n\t1-Menu\n\t2-Exit"<<endl;
	num=getch();

	switch(num)
	{
		case'1':
			system("cls");
			main();
			break;
		case'2':
			return 0;
			break;
		default:
			system("cls");
			return 0;
			break;
			
	}
}

string giveMemberNo(string membername, string birthday)
{
	string memberNo;
	memberNo=membername+birthday;
	return memberNo;
}

char getMenu()
{
	char menuCode;
	cout <<endl<< setw(20)<< "MENU\n"
		 << "---------------\t\t----------"<<endl
		 << "|(B) BEVERAGES|\t\t|(F) FOOD|"<<endl
		 << "---------------\t\t----------"<<endl
		 << "-----------\t\t----------------"<<endl
		 << "|(M) COMBO|\t\t|(R) PROMOTIONS|"<<endl
		 <<" ----------\t\t----------------"<<endl
	 	 << "What would you like to order today?\n"
		 << "Menu code (B/F/M/R): ";
	cin  >> menuCode;														//get MENU CODE
	cout << endl;
	
	if (menuCode=='F'||menuCode=='f')
	{
	cout <<endl<< setw(20)<< "FOOD\n"
		 << "----------\t--------------\t--------------"<<endl
		 << "|(C) CAKE|\t|(P) CUPCAKE|\t|(I) ICECREAM|"<<endl
		 << "----------\t--------------\t--------------"<<endl
	 	 << "What would you like to order today?\n"
		 << "Menu code (C/P/I): ";
	cin  >> menuCode;														//get MENU CODE
	cout << endl;	
	}
	return menuCode;
}

string fileName(char menuCode)
{
	string file;
	if (menuCode=='B'||menuCode=='b')
		file="beverage.txt";
	else if (menuCode=='M'||menuCode=='m')
		file="combo.txt";
	else if (menuCode=='R'||menuCode=='r')
		file="promotions.txt";
	else if (menuCode=='C'||menuCode=='c')
		file="cakes.txt";
	else if(menuCode=='P'||menuCode=='p')
		file="cupcakes.txt";
	else if(menuCode=='I'||menuCode=='i')
		file="icecream.txt";	
	else
	{
		cout<<"Sorry you entered the wrong code. Try again.";
		return 0;
	}
	return file;															//get FILE NAME
}

int menuNo(char menuCode)
{
	int no;
	if (menuCode=='B'||menuCode=='b')
		no=9;
	else if (menuCode=='M'||menuCode=='m')
		no=4;
	else if (menuCode=='R'||menuCode=='r')
		no=4;
	else if (menuCode=='C'||menuCode=='c')
		no=6;
	else if(menuCode=='P'||menuCode=='p')
		no=5;
	else if(menuCode=='I'||menuCode=='i')
		no=10;
	else
	{
		cout<<"Sorry, wrong code!"<<endl;
		return 0;
	}
		
	return no;																//get NUMBER OF ITEM IN MENU
}

void saveMenu(struct menuLayout layout[12], ifstream &menu, int itemNo) 	//save MENU in STRUCT
{
	for (int a=0; a<itemNo; a++)
	{
		menu>>layout[a].no;
		menu.getline(layout[a].name, 50, ';');
		menu>>layout[a].price;
	}
}

void displayMenu(struct menuLayout layout[12], int itemNo)					//DISPLAY menu
{
	system("cls");
	cout<<"\t*************************************************"<<endl
		<<"\t*Selections\t Menu"<<setw(35)<<"Price(RM)"<<endl
		<<"\t*************************************************"<<endl;
	for (int a=0; a<itemNo; a++)
	{
		cout<<"\t*"<<layout[a].no<<"\t\t"
			<<layout[a].name<<"\t"
			<<layout[a].price<<endl;
	}
	cout<<"\t*************************************************"<<endl;		
}

double displaySum(int selections, int qty, struct menuLayout layout[12])			//display SUM
{
	double total=layout[selections-1].price*qty;
	cout<<endl<<"Your order is: "<<endl
		<<"Selections\t Menu\t\t\t\tQty\tPrice(RM)"<<endl
		<<selections<<"\t"<<layout[selections-1].name<<"\t"<<qty<<"\t"<<layout[selections-1].price<<endl
		<<"--------------------------------------------------------------------"<<endl
		<<"Total Amount\t\t\t\t\t"<<total<<endl<<endl;
	return total;
}

void saveReceipt(struct receiptLayout r[100], int oNo,int selections, struct menuLayout layout[12], int qty, double total)	//save RECEIPT as STRUCT
{
	int a=oNo-1;
	r[a].code=selections;
	strcpy(r[a].name, layout[selections-1].name);
	r[a].qty=qty;
	r[a].price=layout[selections-1].price;
	r[a].sum=total;
}


void write_n_display_info_NewMem(ofstream &file_out, char option1,  double total2, double cash, string memberNo, string membername, string birthday, string email, int phone, int credit)
{
	file_out<<"\n\t***************"<<endl
			<<"\tDining location: "<<option1<<endl
			<<"\tMember no: "<<memberNo<<endl
			<<"\tMr/Ms :"<<membername<<endl
			<<"\tBirthday : "<<birthday<<endl
			<<"\tEmail : "<<email<<endl
			<<"\tPhone no: "<<phone<<endl
			<<"\tCredit no : "<<credit<<endl
			<<"\t****************"<<endl
			<<"\tSubtotal : RM"<<total2<<endl
			<<"\tGST : RM"<<total2*0.08<<endl
			<<"\tTotal : RM"<<total2+total2*0.08<<endl
			<<"\tCash : RM"<<cash<<endl
			<<"\t*****************"<<endl
			<<"\tChange : RM"<<cash-((total2)+total2*0.08) <<endl
			<<"\t*****************"<<endl;
}

void write_n_display_info_OldMem(ofstream &file_out, char option1, string memberNo, double total2, double cash)
{
	file_out<<"\n\t***************"<<endl;
		file_out<<"\tDining location: "<<option1<<endl;
		file_out<<"\tMember no: "<<memberNo<<endl;
		file_out<<"\t****************"<<endl;
		file_out<<"\tSubtotal : RM"<<total2<<endl;
		file_out<<"\tGST : RM"<<total2*0.08<<endl;
		file_out<<"\tTotal : RM"<<total2+total2*0.08<<endl;
		file_out<<"\tCash : RM"<<cash<<endl;
		file_out<<"\t*****************"<<endl;
		file_out<<"\tChange : RM"<<cash-((total2)+total2*0.08) <<endl;
		file_out<<"\t*****************"<<endl;
}

void write_n_display_info_Normal(ofstream &file_out, char option1, double total2, double cash)
{
	file_out<<"\n\t***************"<<endl;
	file_out<<"\tDining location: "<<option1<<endl;
	file_out<<"\t****************"<<endl;
		file_out<<"\tSubtotal : RM"<<total2<<endl;
		file_out<<"\tGST : RM"<<total2*0.08<<endl;
		file_out<<"\tTotal : RM"<<total2+total2*0.08<<endl;
		file_out<<"\tCash : RM"<<cash<<endl;
		file_out<<"\t*****************"<<endl;
		file_out<<"\tChange : RM"<<cash-((total2)+total2*0.08) <<endl;
		file_out<<"\t*****************"<<endl;
}

void receiptType(ofstream &file_out, char option1, char member, char reg, string memberNo, double total2, double cash, string membername, string birthday, string email, int phone, int credit)
{
	if (member=='Y'|| member=='y')
	write_n_display_info_OldMem(file_out, option1, memberNo, total2, cash);
	else if (reg=='Y'|| reg=='y')
	write_n_display_info_NewMem(file_out, option1, total2, cash, memberNo, membername, birthday, email, phone, credit);
	else
	write_n_display_info_Normal(file_out, option1, total2, cash);
}
